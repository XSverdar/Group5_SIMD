# Deep Dive: SIMD programming project
## Group 5
- CAI, Edison B.
- DEQUICO, Beverly Joyce P.
- LA'O, Erin Denise C.
- RELUCIO, Jan Jhezaree L.

## Results
i.) screenshot of the program output with execution time for all cases

ii.) comparative table of execution time as well as analysis of the performance of different kernels (how many times faster, why is it faster, etc.)
| Array Size    | Method        | Average Time | Times faster than C | Times faster than Non-SIMD | Times faster than SIMD XMM | Times faster than SIMD YMM |
| ------------- | ------------- | ------------ | ------------------- | -------------------------- | -------------------------- | -------------------------- |
| 2^20          | C             |              | -                   |                            |                            |                            |
|               | Non-SIMD      |              |                     | -                          |                            |                            |
|               | SIMD XMM      |              |                     |                            | -                          |                            |
|               | SIMD YMM      |              |                     |                            |                            | -                          |
| 2^26          | C             |              | -                   |                            |                            |                            |
|               | Non-SIMD      |              |                     | -                          |                            |                            |
|               | SIMD XMM      |              |                     |                            | -                          |                            |
|               | SIMD YMM      |              |                     |                            |                            | -                          |
| 2^30          | C             |              | -                   |                            |                            |                            |
|               | Non-SIMD      |              |                     | -                          |                            |                            |
|               | SIMD XMM      |              |                     |                            | -                          |                            |
|               | SIMD YMM      |              |                     |                            |                            | -                          |
| 2^31          | C             |              | -                   |                            |                            |                            |
|               | Non-SIMD      |              |                     | -                          |                            |                            |
|               | SIMD XMM      |              |                     |                            | -                          |                            |
|               | SIMD YMM      |              |                     |                            |                            | -                          |


iii.) screenshot of the program output with correctness check (C)

iv.) screenshot of the program output, including correctness check (x86-64)

v.) screenshot of the program output, including correctness check (SIMD XMM register)

vi.) screenshot of the program output, including correctness check (SIMD, YMM register)

vii.) Discuss the problems encountered and solutions made, unique methodology used, AHA moments, etc.

**For Non-SIMD Implementation:** Implementing the non-SIMD version of the stencil computation in x86-64 assembly posed minimal challenges in conceptualizing and translating the problem into low-level code. During debugging, **issues with accessing invalid indices in the input array**, which leads to segmentation faults and incorrect results, were encountered. To address this, **boundary checks were added to ensure the index in the rcx register was valid**, skipping iterations when rcx was less than 3 or greater than n - 4. Ensuring **accurate input arguments n, x, and y** required a review of calling conventions and proper register usage. Loop and index management difficulties were mitigated by **initializing the loop counter (rcx) and output index (r8) with xor**, preventing unintended carry-over from previous values. **Understanding memory offsets for accurate array access and prioritizing boundary checks were the "AHA" realizations**. Incremental development, thorough testing, and manual offset calculations with extensive comments enhanced control over low-level data manipulation. Effective register usage, particularly employing rcx for looping and r8 for indexing, streamlined the function and reduced complexity significantly. **What's unique in the non-SIMD methodology is its reliance on scalar operations, where each computation is performed sequentially without parallelization**. This approach may be simpler to implement compared to SIMD (Single Instruction, Multiple Data) techniques but may lack the performance benefits gained from parallel processing.

