#### Files

- hjl-info/memcpy-new.S
    - New implementation
    - EVEX only
    - Does not implement non-temporal store portion but does include branch to it (outside of size range for the benchmarks)
    
- hjl-info/memcpy-new-obj.S
    - Objdump of new implementation

- hjl-info/memcpy-glibc.S
    - GLIBC implementation before your most recent patch when it was in "fast" state

- hjl-info/memcpy-glibc-obj.S
    - Objdump of "fast" GLIBC implementation        

- hjl-info/results.txt
    - CSV of results from all runs
    - Run on Tigerlake

- hjl-info/memcpy-results-summary.pdf
    - Table with median of geometric mean number / nconfs for each configuration (trials = 34)


#### Notes
- Given that the new implementation wins with all min/max sizes and all scales **I DO NOT believe this is an artifact of the benchmark, but instead that some internals of the implementations**.
    
- I believe that a combination Uop Cache thrashing, Uop Cache - MITE transitions, Uop Cache - LSD transitions, and BPU thrashing where related to the "anomilies" seen when trying to change alignment.
    
- In multiple places "awkward" nop padding is added. This was always informed by some related performance counter. Often branches had to be seperated to different 16 byte region, or seemingly random NOP padding had to be added before a set of branches to avoid having them apparently conflict with other hot branches (see the objdumps for more info on exact layouts).
    
- The AVX2 and non-movsb implementations will need to be written seperately and will likely require different arrangements.

- At the moment the new implementation is based primarily on trial and error + some vague trends seen in the performance counters. I really don't know enough about the internals to make well-informed decisions. While I know this is probably impossible, is there any way I can get some internal info, particularly on the BPU and FE? I can likely sign an NDA.

- Currently there are two things holding us back from reimplementing the branch heavy sections of the code with masked EVEX instructions:
    1. A masked store from a perspective of dependency tracking between loads/store will appear to affect the entire operand size of the memory destination, not just the portions included in the mask. I.e
            ```
            movl    $1, %eax
            kmovd   %eax, %k1
            vmovdqu8 %ymm16, (%rdi){%k1}
            movb    1(%rdi), %cl
            // movb has a false dependency on the prior
            // masked store.
        ```
        As far as I can tell this is because store/load serialization
        is based on the results from a CAM match on the lower 12 bits
        in the SB (also AFAICT the reason for 4k aliasing). While in 
        practice I don't know how much a concern this would be. It very
        seriously pessmimizes memmove benchmarks as it essentially 
        serializes them. If it's not too forward, is there a reason the
        CAM matches cant verify the full address and if its ready/exists
        any mask operands before serializing?
        
        
    2. A masked load/store whose operand sizes splits a 4k page boundary but mask value does not include any of the results from 1 (or both pages) still stalls on the TLB misses necessary to complete the instructions had the mask been all 1s. I.e
        ```
            movl    $1, %eax
            kmovd   %eax, %k1
            // With %rdi % 4096 == 4095
            vmovdqu8 (%rdi), %ymm16{%k1}
            // Will wait on TLB result for both pages even
            // though instruction is logically retirable
            // with only the lower page
        ```
        This is not a serious issue if both pages are in the TLB, but if one
        page is missing is will increase the latency of the instruction by an
        order of magnitude and apparently NOT update the TLB even if the page
        was valid. The current evex memset handles this by entirely skipping
        evex if there is a page cross. This is only doable in memcpy but is
        more difficult (either 2 branches in with false positives).
    
    Both of these concerns ultimately are judgement calls. As well, the page split 
    can be avoided at minimal cost on *most* reasonable benchmarks. Do you have an
    opinion on either of these?
    
- This implementation and any implementation that may follow will by its nature be incredibly brittle. Even a small change, if it either adds a branch, affects the 16 byte alignment of a branch, or affects the destination of a larget, could dramatically affect performance. In an effort to keep this too a minimum I kept all targets 16 byte aligned so there is generally a bit of padding to work with. If memmove was given up and this was only meant to implement memcpy, a number of the branches could be removed and much of the brittleness with it.

- My guess is there is another ~5-15% that could be gained with further reorganization (particularly between the branches for copy in `[0, 31]` range and the branches for the `[65, MAX]` range). I didn't dive as deeply into this yet.

#### In GLIBC

I am generally unsure 1) if this has a place in glibc given how brittle it is and 2) how would be best to add it given how CPU specific it is. I was wondering what your thoughts where on this.
