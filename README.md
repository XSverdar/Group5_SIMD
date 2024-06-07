# Deep Dive: SIMD programming project
## Group 5 Members
- CAI, Edison B.
- DEQUICO, Beverly Joyce P.
- LA'O, Erin Denise C.
- RELUCIO, Jan Jhezaree L.

## Results
#### i.) screenshot of the program output with execution time for all cases

#### ii.) comparative table of execution time as well as analysis of the performance of different kernels (how many times faster, why is it faster, etc.)
| Array Size    | Method        | Average Time | Times faster than C | Times faster than Non-SIMD | Times faster than SIMD XMM | Times faster than SIMD YMM |
|     :---:     | :---:         | :---:        | :---:               | :---:                      | :---:                      | :---:                      |
| 2^20          | C             |              | **-**                   |                            |                            |                            |
|               | Non-SIMD      |              |                     | **-**                          |                            |                            |
|               | SIMD XMM      |              |                     |                            | **-**                          |                            |
|               | SIMD YMM      |              |                     |                            |                            | **-**                          |
| 2^26          | C             |              | **-**                   |                            |                            |                            |
|               | Non-SIMD      |              |                     | **-**                          |                            |                            |
|               | SIMD XMM      |              |                     |                            | **-**                          |                            |
|               | SIMD YMM      |              |                     |                            |                            | **-**                          |
| 2^30          | C             |              | **-**                   |                            |                            |                            |
|               | Non-SIMD      |              |                     | **-**                          |                            |                            |
|               | SIMD XMM      |              |                     |                            | **-**                          |                            |
|               | SIMD YMM      |              |                     |                            |                            | **-**                          |
| 2^31          | C             |              | **-**                   |                            |                            |                            |
|               | Non-SIMD      |              |                     | **-**                          |                            |                            |
|               | SIMD XMM      |              |                     |                            | **-**                          |                            |
|               | SIMD YMM      |              |                     |                            |                            | **-**                          |


#### iii.) screenshot of the program output with correctness check (C)

#### iv.) screenshot of the program output, including correctness check (x86-64)

#### v.) screenshot of the program output, including correctness check (SIMD XMM register)

#### vi.) screenshot of the program output, including correctness check (SIMD, YMM register)

#### vii.) Discuss the problems encountered and solutions made, unique methodology used, AHA moments, etc.
- **For C Implementation:** The most important part to make the C implementation work is **validation**. Since the C version is the answer key to check if the other assembly outputs are correct, it was imperative to ensure that the C implementation was correct. Invalid indeces were avoided by making sure that operations will only be done to the elements within the given array. The function will not perform any operation if the iterated index will go beyond the start or end of the array. The first roadblock was met when the function refused to save elements into the variable y, though the solution was fairly simple because both x and y must be declared as int32_t*, asterisk included. The first version of the C version only had x with the asterisk but both variables must be pointers. Afterwards, when all the implementations were done, the different results were grouped with corresponding print statements so that anyone viewing the output window may see the performance of the methods used. For example, there are print statements to indicate the different array sizes, different coding methods, number of runs, and so on.

- **For Non-SIMD Implementation:** Implementing the non-SIMD version of the stencil computation in x86-64 assembly posed minimal challenges in conceptualizing and translating the problem into low-level code. During debugging, **issues with accessing invalid indices in the input array**, which leads to segmentation faults and incorrect results, were encountered. To address this, **boundary checks were added to ensure the index in the rcx register was valid**, skipping iterations when rcx was less than 3 or greater than n - 4. Ensuring **accurate input arguments n, x, and y** required a review of calling conventions and proper register usage. Loop and index management difficulties were mitigated by **initializing the loop counter (rcx) and output index (r8) with xor**, preventing unintended carry-over from previous values. **Understanding memory offsets for accurate array access and prioritizing boundary checks were the "AHA" realizations**. Incremental development, thorough testing, and manual offset calculations with extensive comments enhanced control over low-level data manipulation. Effective register usage, particularly employing rcx for looping and r8 for indexing, streamlined the function and reduced complexity significantly. **What's unique in the non-SIMD methodology is its reliance on scalar operations, where each computation is performed sequentially without parallelization**. This approach may be simpler to implement compared to SIMD (Single Instruction, Multiple Data) techniques but may lack the performance benefits gained from parallel processing.

- **For SIMD XMM Implementation:** There were little problems encountered when trying to implement the process in SIMD XMM, the biggest hurdle was thinking of how to create the solution, oftentimes the first thought when trying to implement a certain operation is to just translate it one to one into code, so for better visualization the operation of the process was simulated in MS Excel, that's when the "AHA moment" occurred, at first the main concern was how to work around checking if the array accessing would be out of bounds (i.e. x[i-3] where i-3 < 0), which seemed like a hassle to do since it had to be done twice, once in the beginning and another at the end, but upon further observation of the simulation, there was no need for checking this.
  ![image](https://github.com/XSverdar/Group5_SIMD/assets/108528279/88e9f633-2373-4af0-9dd8-e881fef6b053)
  As shown in the image above, the process could simply just start at the first index without having to avoid it and add the next 6 values and then the invalid cases can just simply be cut from the operation entirely from the end. So instead of Y[i] = X[i-3] + X[i-2] + X[i-1] + X[i] + X[i+1] + X[i+2] + X[i+3], the process is reformatted into Y[i+3] = X[i] + X[i+1] + X[i+2] + X[i+3] + X[i+4] + X[i+5] + X[i+6], which then means that the process can start immediately as soon as the assembly function is called and does not require any checking whatsoever.


- **For SIMD YMM Implementation:** text
