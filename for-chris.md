Hi Chris,

H.J. mentioned you where working on a new version of `rep movsb` that
would be a be-all end-all for `memcpy`. Exciting!

Here is some data from data from recent runs on the GLIBC memcpy
benchmark suite.

## Notes on Benchmarks

- These benchmarks are inherently a bit inprecice. They are written
  in C so precise code alignment is difficult to come by.
    - As a result, I will exprapolate a bit on points based on other
      testing I've done on personal benchmarking suites.
      
- These benchmarks **do not cover all cases**.

- Code alignment % PAGE_SIZE is controlled. This is true for both the
  `memcpy` and benchmark code.
    - Transparent huge pages are turned off as well.
    
- All benchmarks are run on dedicated core one after another.
    - **Frequency is not fixed**.

- All runtime configuration values are replaced with `imm32` values.

- There are two benchmarks. `fsrm-comparison.pdf` and
  `fsrm-comparison-lat.pdf`. The only difference is the `-lat`
  extension version has an `lfence` between benchmark iterations.
    - `length` -> copy size
    - `align1` -> src alignment % 4096
    - `align2` -> dst alignment % 4096
    - `dst > src` -> value of `dst > src`

  
- Configurations are as follows:
    1. `ALIGN` -> 64 byte align `rdi` before `rep movsb`
    2. `ALIAS` -> use backward vector copy if `0 < (rdi - rsi) % 4096 < 512`
        - Note this includes additional overhead for `memmove`
          correctness checks.
    3. `ALIGN + ALIGN` -> 2 and 3.
      
- All times are reported as `(Raw rep movsb memcpy Time) / (Config
  Time memcpy Time)`
    - All times are from geomean of N = 20 runs.
    - Each benchmark configurations runs 65536 back to back iterations
      of the configuration after 16384 warmup runs.
    - RED -> `raw rep movsb memcpy` is **slower**
    - GREEN -> `raw rep movsb memcpy` is **faster**
      

## Summary

**Note: These observations are only regarding `rep movsb` with `FSRM`
using forward copy with that correctly implements
`memmove`/`memcpy`**:


1. `rep movsb` does not appear to handle 4k aliasing. This means that
  in the 4k aliasing case (roughly defined as `0 < (dst - src) % 4096
  < [256, 512]` backward vector copy is better for all medium - large
  sizes.

2. `rep movsb` performance is worse than non-temporal store + prefetch
  loops for large memcpy. I'm not sure if there is some constraint in
  the microcode I'm unaware of, but since `rep movsb` has more fine
  grain control over prefetching is is able to optimizing `RFO` ->
  `iToM` I fundementally don't understand why this is the case.

3. It still is beneficial to align `dst` before `rep movsb` in certain
  cases. This really should never be the case as aligning does not
  involve branches.
  
  
## 4k Aliasing.

- Clearly `FSRM` is not checking for 4k aliasing. **There is never a
  regression in handling a 4k aliasing case with backwards copy**.
    - There is a slight regression in the cost of the extra checks.

- Tests only show up to `dst - src % 4096 == 80`. In other experiments
  I see this issue going away around `dst - src % 4096 > 512`.
  
- As copy size becomes larger this becomes less of an issue. This
  likely is because L1 / L2 bandwidth is becoming the primary
  bottleneck. 
    - This can be seen in the speedup of handling the alias cases
      becoming incrementally lower for sizes = `4096, 8192, 16384,
      32768, and 65536`.
    
- `dst - src % 4096 < 64` has much lower performance impact than `64
  <= dst - src % 4096 < 128`.
    - Generally the issue appears to be worst for `[128, 256]`.
  
- The follow configurations in `fsrm-comparison.pdf`:

```
4096	0	32	0		157.291	0.991	1.875	1.867
4096	0	32	1		156.613	0.991	1.868	1.877
4096	0	64	0		91.492	0.969	1.087	1.087
4096	0	64	1		92.055	0.973	1.092	1.09
```

1. This configuration appears to buck the trend of the 4k aliasing
   issue becoming worst in the `[128, 256]` range.
   
2. I believe the reason the `32` dst alignment case is so much worse
   is actually related to the internals of `FSRM`. 
    - In other testing I've found `4096` size to behave fundementally
      differently from the perspective of aligning as well.
    - My intuition is there is some threshold around the `4096` size
      (or possibly is special cased for the Linux Kernels sake).


## Aligning

- Aligning for `FSRM` was done **exclusively**. That means already
  aligned `dst` was not changed.
    - Thus there is a moderate regression (~5%) when `dst` is already
      aligned (wasted work).
    
- In general 64 byte aligning appears to help more than it hurts.
    - **I am still uncertain of the exact rules for when it is
      beneficial / harmful**.

- Unlike `ERMS` where aligning is always an improvement or as good,
   aligning before `FSRM` is a mixed bag.
    - The `4096` discussed earlier can lead to wild regressions.
    - In general there are wild "fast" and "slow" paths for `FSRM`
      that I am still not 100% certain of.

- There are some edge cases when aligning seems to push `FSRM` to a
  slow path such as:
  
```
4032	0	31	0		137.808	0.876	1.284	1.269
4032	0	31	1		138.482	0.886	1.291	1.286
4032	0	63	0		138.479	0.878	1.291	1.286
4032	0	63	1		138.486	0.881	1.276	1.294
...
4032	1	31	0		136.956	0.87	1.254	1.255
4032	1	31	1		135.995	0.861	1.242	1.25
4032	1	63	0		136.297	0.869	1.238	1.242
4032	1	63	1		138.249	0.872	1.259	1.258
```

- Here was can see aligning `dst` being harmful. ~13% regression.

- Given that both cases have a regression **I don't think** the
  aligning has to do with 4k aliasing -> load bottleneck -> better to
  align src.
  - Instead I think it has to do with general weirdness I see around
    the PAGE threshold.
  - We do not see regressions like these anywhere else.
  


## Musing about Small Copy

As a side note [the automemcpy
paper](https://research.google/pubs/pub50338/) suggests that for a
given distribution using a Z3 solver (or
[rosette](https://emina.github.io/rosette/) which appears to been
taking over Z3 in usefulness) to find the optimal branch ordering
could be effective. I've been thinking this may be particularly
powerful for microcode. Since there is no branch predictor, the branch
instructions and taken/not-taken paths might also have a fixed cost so
they could be included in the model. This would allow you to do more
than just decide on the block sizes, but solve for an optimal branch
pattern (including jump to block sizes that require more branches).

Depending on the cost of a taken/non-taken branch in micro-code it
also may be useful to use a branch method similiar to what [linux
does](https://elixir.bootlin.com/linux/latest/source/arch/x86/lib/memcpy_64.S#L125).

Or something that minimizes total branch depth such as for `size = 32`

```
L(16_31):
if(size < 8)
    goto L(0_7)

if(size < 16)
    goto copy(8, 15)
    
copy(16, 31)

L(0_7):
if (size < 4)
    goto L(0_3)
copy(4, 7)

L(0_3):
if(size <= 1)
    if (less)
        goto copy(0)
    copy(1) // This should be included in copy 2/3 in block pattern
copy(2)


...
```



## About Me
Truthfully I'm a bit disappointed I won't get to help work on this
project. If there is any way an exception could be made I believe I
could be a valuable asset to your team. I would absolutely love to
contribute as either an intern or possibly under as a research
collaborator through an NDA / my PHD program.

