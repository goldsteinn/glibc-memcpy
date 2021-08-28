#### BPU Affect on Tigerlake, Icelake, and Skylake

- See testing conditions described in previous writeup
- [Tigerlake CPU](https://ark.intel.com/content/www/us/en/ark/products/208921/intel-core-i7-1165g7-processor-12m-cache-up-to-4-70-ghz-with-ipu.html)
- [Icelake CPU]()
- [Skylake CPU]()

#### Tigerlake & Icelake

**Observed nearly identical pattern of performance vs `ALIGN_ENTRY` and `PADDING % 64` on *Tigerlake* and *Icelake*.

**Results for `sz` in `[0, 32]` implementation on Tigerlake:**

![$> python3 graph-results.py -a -f ../combined/tgl-icl-32.txt --levents cycles --alignments 16,32,48,64 --pmod64 16,32,48,64 --pmod2 0 --pmax 129 --save --cpu tgl](figs/tgl-16-32-48-64--1_129_16_32_48_64_0-1-cycles.png "$> python3 graph-results.py -a -f ../combined/tgl-icl-32.txt --levents cycles --alignments 16,32,48,64 --pmod64 16,32,48,64 --pmod2 0 --pmax 129 --save --cpu tgl")

**Results for `sz` in `[0, 32]` implementation on Icelake:**

![$> python3 graph-results.py -a -f ../combined/tgl-icl-32.txt --levents cycles --alignments 16,32,48,64 --pmod64 16,32,48,64 --pmod2 0 --pmax 129 --save --cpu icl](figs/icl-16-32-48-64--1_129_16_32_48_64_0-1-cycles.png "$> python3 graph-results.py -a -f ../combined/tgl-icl-32.txt --levents cycles --alignments 16,32,48,64 --pmod64 16,32,48,64 --pmod2 0 --pmax 129 --save --cpu icl")

Both have the same pattern for performance:

|ALIGN_ENTRY|0       |16      |32      |48      |
|-----------|--------|--------|--------|--------|
|0          |FAST    |SLOWER  |SLOWEST |FAST    |
|16         |SLOWER  |SLOWEST |FAST    |FAST    |
|32         |FAST    |SLOWER  |SLOWEST |FAST    |
|48         |SLOWER  |SLOWEST |FAST    |FAST    |


The above table adjust for `ALIGN_ENTRY` and `address_of(L(less_vec)) % 64` are as follows:

|ALIGN_ENTRY|0       |16      |32      |48      |
|-----------|--------|--------|--------|--------|
|0          |SLOWER  |SLOWEST |FAST    |FAST    |
|16         |SLOWER  |SLOWEST |FAST    |FAST    |
|32         |FAST    |FAST    |SLOWER  |SLOWEST |
|48         |FAST    |FAST    |SLOWER  |SLOWEST |

Which again boils down to the following formula: `32 & (address_of(L(less_vec)) - address_of(memcpy))`


Final **the reason for the variance on both Tigerlake and Icelake appears to be branch misses**:

Note: Icelake appears to experience a proportionately more significant slowdown and greater increase in branch misses:

![$> python3 graph-results.py -e -f ../combined/tgl-icl-32.txt --levents cycles --revents MISPREDICTED_BRANCH_RETIRED --alignments 64 --pmod64 16,32,48,64 --pmod2 0 --pmax 129 --save --cpu tgl,icl](figs/tgl-icl-64--1_129_16_32_48_64_0-2-cycles-MISPREDICTED_BRANCH_RETIRED.png "$> python3 graph-results.py -e -f ../combined/tgl-icl-32.txt --levents cycles --revents MISPREDICTED_BRANCH_RETIRED --alignments 64 --pmod64 16,32,48,64 --pmod2 0 --pmax 129 --save --cpu tgl,icl")


#### Skylake

Skylake appears to be a bit different, however **does have distinct "fast" and "slow" modes** though **the modes have a less significant performance difference**.


