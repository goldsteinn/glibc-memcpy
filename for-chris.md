Hi Chris,

H.J. mentioned you where working on a new version of `rep movsb` that
would be a be-all end-all for `memcpy`. Exciting!

Regarding current issues I see with `FSRM`:

**Note: These observations are only regarding `rep movsb` with `FSRM`
using forward copy with that correctly implements
`memmove`/`memcpy`**:

## General Notes

1. `rep movsb` does not appear to handle 4k aliasing. This means that
  in the 4k aliasing case (roughly defined as `0 < (dst - src) % 4096
  < [256, 512]` backward vector copy is better for all medium - large
  sizes.

2. `rep movsb` performance is worse than non-temporal store + prefetch
  loops for large memcpy. I'm not sure if there is some constraint in
  the microcode I'm unaware of, but since `rep movsb` has more fine
  grain control over prefetching is is able to optimizing `RFO` ->
  `iToM` this shouldnt be the case.

3. It still is beneficial to align `dst` before `rep movsb` in certain
  cases. This really should never be the case.

## Data From Recent Runs

#### Attachment: TGL-VECTOR-COPY.pdf

The three things I notice are:

1. Fantatistic work for page copies!

2. `FSRM` is clearly missing an logic for 4k alias detecting.

3. Performance seems to drop off around the `32768` size range. This
   fundementally doesn't make sense as again. With `rep movsb` ability
   to more finely control prefetching and cross-core traffic, it
   should only "win" by more as size gets larger. Possibly I have a
   misunderstanding of the constraints on microcode, but it seems that
   `rep movsb` should never lose due to backend related issues.
   
#### Attachment: TGL-ALIGN-MOVSB.pdf

The results are much more mixed here, but it is a bit strange that
aligning first ever is advantageous. Stranger yet this advantage
increases dramatically once size > `4096`. It seems that there is some
special case for `size > 4096` which was written for page copies by
the kernel, but which suffers as a generic memcpy.


#### Random Musing

As a side note [the automemcpy
paper](https://research.google/pubs/pub50338/) suggests that for a
given distribution using a Z3 solver (or
[rosette](https://emina.github.io/rosette/) which appears to been
taking over Z3) to find the optimal branch ordering could be
effective. I've been thinking this may be particularly powerful for
microcode. Since there is no branch predictor, the branch
instructions and taken/not-taken paths might also have a fixed cost so
they could be included in the model. (Although I obviously don't know
enough about microcode to know if there are other factors that make
predicting the cost a branch difficult).


Truthfully I'm a bit disappointed I won't get to help work on this
project. If there is any way an exception could be made I believe I
could be a valuable asset to your team. I would absolutely love to
contribute as either an intern or possibly under as a research
collaborator through an NDA / my PHD program.

