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
