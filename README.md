# Deep Dive: SIMD programming project
## Group 5 Members
- CAI, Edison B.
- DEQUICO, Beverly Joyce P.
- LA'O, Erin Denise C.
- RELUCIO, Jan Jhezaree L.

## Results
#### i.) screenshot of the program output with execution time for all cases

##### Debug
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/1a992e49-c64a-4133-a8c5-3d034bb3da98)
##### Release
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/0f3a27ad-5e56-4c1e-9680-7a59fb0ae5a4)


#### ii.) comparative table of execution time as well as analysis of the performance of different kernels (how many times faster, why is it faster, etc.)
| Array Size    | Method        | Average Time | Times faster than C (Debug) | Times faster than C (Release) | Times faster than Non-SIMD | Times faster than SIMD XMM | Times faster than SIMD YMM |
|     :---:     | :---:         | :---:        | :---:                       | :---:                      | :---:                      | :---:                      |  :---:                      |
| 2^20          | C (Debug)     | 4.502713 ms  | **-**                       |                            |                            |                            |
|               | C (Release)   | 1.952240 ms  |                             | **-**                          |                            |                            |
|               | Non-SIMD      | 2.256923 ms  |                             |                           | **-**                          |                            |
|               | SIMD XMM      | 0.818280  ms |                              |                            |                          | **-**                           |
|               | SIMD YMM      | 0.483967 ms  |                     |                            |                            |                           | **-** |
| 2^26          | C (Debug)     | 255.368430 ms | **-**                   |                            |                            |                            |
|               | C (Release)   | 104.901537 ms |                    | **-**                          |                            |                            |
|               | Non-SIMD      | 70.931690 ms |                     |                           | **-**                          |                            |
|               | SIMD XMM      | 49.222237 ms |                     |                            |                           | **-**                          |
|               | SIMD YMM      | 53.066987 ms |                     |                            |                            |                           | **-** |
| 2^30          | C (Debug)     | 4585.426280 ms | **-**             |                            |                            |                            |
|               | C (Release)   | 1627.885453 ms |                    | **-**                          |                            |                            |
|               | Non-SIMD      | 1448.693463 ms |                     |                           |  **-**                          |                            |
|               | SIMD XMM      | 1103.127543 ms |                     |                            |                           | **-**                          |
|               | SIMD YMM      | 981.895367 ms |                     |                            |                            |                          | **-**  |


#### iii.) screenshot of the program output with correctness check (C)

##### Debug
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/7736148a-ce10-4d71-a7e8-faca9ccdf9b1)
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/a2e7ee30-2ed5-4c53-99a4-e3ecd4c5d311)
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/49a79599-a3c7-4e49-921e-768a858525eb)

##### Release
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/a12315bc-2516-468a-a521-1078a015af35)
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/f6c6066a-9959-401a-b5aa-8433685565e1)
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/70261c9b-6af1-4e0d-a268-6346c1fb4267)

#### iv.) screenshot of the program output, including correctness check (x86-64)

#### Debug
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/9adca635-674e-4480-97e0-42623f57a0a1)
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/f9823972-0ed8-4aa7-9f4b-e6436a1dd779)
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/35101521-280f-4dbc-b0fa-1d5037de18db)

#### v.) screenshot of the program output, including correctness check (SIMD XMM register)

#### Debug
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/6ad9ad9a-b631-4948-848b-6a8b5899154f)
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/c1c934de-7921-4788-8311-d32e4cfde027)
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/c3050a99-b6aa-47e7-87d9-915dea51a177)

#### vi.) screenshot of the program output, including correctness check (SIMD, YMM register)

#### Debug
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/d1a65d79-0085-497a-868e-9125307cde1f)
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/14534fba-09e1-447e-a67e-57a49c5b55aa)
![image](https://github.com/XSverdar/Group5_SIMD/assets/57384457/c177fce2-2222-4f8a-8a78-07eb9334dc85)

#### vii.) Discuss the problems encountered and solutions made, unique methodology used, AHA moments, etc.
- **For C Implementation:** The most important part to make the C implementation work is **validation**. Since the C version is the answer key to check if the other assembly outputs are correct, it was imperative to ensure that the C implementation was correct. Invalid indeces were avoided by making sure that operations will only be done to the elements within the given array. The function will not perform any operation if the iterated index will go beyond the start or end of the array. The first roadblock was met when the function refused to save elements into the variable y, though the solution was fairly simple because both x and y must be declared as int32_t*, asterisk included. The first version of the C version only had x with the asterisk but both variables must be pointers. The second roadblock was that the rand() function wasn't randomizing the input for each array size. The rand() function in C pseudo-random such that the same values will be given even when rerunning the program. So, to go around this, srand() was used to set the "seed" of the rand() function to time(NULL) such that different values would be given for every run and then a small value was added to the next calls of rand() to randomize the data for each array size. Afterwards, when all the implementations were done, the different results were grouped with corresponding print statements so that anyone viewing the output window may see the performance of the methods used. For example, there are print statements to indicate the different array sizes, different coding methods, number of runs, and so on.

- **For Non-SIMD Implementation:** Implementing the non-SIMD version of the stencil computation in x86-64 assembly posed minimal challenges in conceptualizing and translating the problem into low-level code. During debugging, **issues with accessing invalid indices in the input array**, which leads to segmentation faults and incorrect results, were encountered. To address this, **boundary checks were added to ensure the index in the rcx register was valid**, skipping iterations when rcx was less than 3 or greater than n - 4. Ensuring **accurate input arguments n, x, and y** required a review of calling conventions and proper register usage. Loop and index management difficulties were mitigated by **initializing the loop counter (rcx) and output index (r8) with xor**, preventing unintended carry-over from previous values. **Understanding memory offsets for accurate array access and prioritizing boundary checks were the "AHA" realizations**. Incremental development, thorough testing, and manual offset calculations with extensive comments enhanced control over low-level data manipulation. Effective register usage, particularly employing rcx for looping and r8 for indexing, streamlined the function and reduced complexity significantly. **What's unique in the non-SIMD methodology is its reliance on scalar operations, where each computation is performed sequentially without parallelization**. This approach may be simpler to implement compared to SIMD (Single Instruction, Multiple Data) techniques but may lack the performance benefits gained from parallel processing.

- **For SIMD XMM Implementation:** There were little problems encountered when trying to implement the process in SIMD XMM, the biggest hurdle was thinking of how to create the solution, oftentimes the first thought when trying to implement a certain operation is to just translate it one to one into code, so for better visualization the operation of the process was simulated in MS Excel, that's when the "AHA moment" occurred, at first the main concern was how to work around checking if the array accessing would be out of bounds (i.e. x[i-3] where i-3 < 0), which seemed like a hassle to do since it had to be done twice, once in the beginning and another at the end, but upon further observation of the simulation, there was no need for checking this.
  ![image](https://github.com/XSverdar/Group5_SIMD/assets/108528279/88e9f633-2373-4af0-9dd8-e881fef6b053)
  As shown in the image above, the process could simply just start at the first index without having to avoid it and add the next 6 values and then the invalid cases can just simply be cut from the operation entirely from the end. So instead of Y[i] = X[i-3] + X[i-2] + X[i-1] + X[i] + X[i+1] + X[i+2] + X[i+3], the process is reformatted into Y[i+3] = X[i] + X[i+1] + X[i+2] + X[i+3] + X[i+4] + X[i+5] + X[i+6], which then means that the process can start immediately as soon as the assembly function is called and does not require any checking whatsoever.


- **For SIMD YMM Implementation:** The immediate challenge in programming this implementation is to base it on the XMM implementation. Initially, the plan was to use horizontal addition to solve the project (this is without the basis of XMM implementation). However, after studying the XMM implementation, it became clear that there was a simpler way to code the project. Drawing from the XMM implementation, a problem was encountered regarding how much should be added to the address index of YMM. There was some experimentation with numbers to determine the index until a groupmate recalled that the index is the same as XMM; however, only the increment in the address after the operation is to be changed - the AHA moments. Comparing this algorithm to the previous one, this one is more straightforward, easier to code, and less complex to trace.
