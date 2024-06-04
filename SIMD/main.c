// ---- GROUP 5 ---- //
// CAI, Edison       //
// DEQUICO, Beverly  //
// LA'O, Erin        //
// RELUCIO, Jan      //
// ----------------- //

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <time.h>

extern void nonsimd_stencil(size_t n, int32_t* x, int32_t* y);
extern void simdxmm_stencil(size_t n, int32_t* x, int32_t* y);
extern void simdymm_stencil(size_t n, int32_t* x, int32_t* y);

void c_stencil(size_t n, int32_t* x, int32_t* y) {
	int i;
	int index = 0;

	for (i = 0; i < n; i++) {
		// make sure all indeces are valid
		if ((i - 3 >= 0) && ((i + 3) <= (n - 1))) {
			y[index] = x[i - 3] + x[i - 2] + x[i - 1] + x[i] + x[i + 1] + x[i + 2] + x[i + 3];
			index++;
		}
	}
}

int main(int argc, char* argv[]) {
	size_t ARRAY_SIZE = 2 << 20;
	size_t ARRAY_BYTES = ARRAY_SIZE * sizeof(int32_t);
	int i, j; // counter variables

	// timer variables
	double pcFreq; // Counter frequency (timer resolution)
	__int64 counterStart; // Timer value
	LARGE_INTEGER li; // Large interger for timer value
	double time_taken; // seconds
	double average_time = 0; // average time
	int retcode; // Return code

	// Get frequency of counter
	retcode = QueryPerformanceFrequency(&li);
	if (retcode == 0)
		printf("ERROR: QueryPerformanceFrequency() failed \n");
	pcFreq = li.QuadPart;

	int32_t* x, * y;
	x = (int32_t*)malloc(ARRAY_BYTES);
	y = (int32_t*)malloc(ARRAY_BYTES);

	// array initialization
	for (j = 0; j < ARRAY_SIZE; j++) {
		x[j] = rand();
		y[j] = 0;
	}

	// ----------- 2 ^ 20 -----------
	for (i = 0; i <= 120; i++) {
		if (i <= 30) {
			if (i == 0) {
				printf("=== C (2^20) ===\n");
				printf("Number of elements: %zd\n", ARRAY_SIZE);
			}

			QueryPerformanceCounter(&li);
			counterStart = li.QuadPart;

			c_stencil(ARRAY_SIZE, x, y);

			QueryPerformanceCounter(&li);
			time_taken = 1000.0 * ((li.QuadPart - counterStart) / pcFreq);
			printf("Time in C = %f ms\n", time_taken);

			average_time = average_time + time_taken;

			if (i == 30) {
				printf("Average Time in C (2^20): %f ms\n", average_time / 30);
				average_time = 0;

				// TO-DO 1: fix these print statements so they dont print out garbage after the actual answer (ex. 28, 35, 342323523, 32355223)
				printf("First 10 elements of Y -> ");
				for (j = 0;j < 10;j++)
				{
					if (y[j] != 0)
						printf("%d  ", y[j]);
				}
				printf("\nLast 10 elements of Y -> ");
				for (j = (ARRAY_SIZE - 10);j < ARRAY_SIZE;j++)
				{
					if (y[j] != 0)
						printf("%d  ", y[j]);
				}
				printf("\n");
			}
		}

		// ============= NONSIMD VERSION =============
		else if (i > 30 && i <= 60) {
			if (i == 31) {
				printf("\n=== NON-SIMD (2^20) ===\n");
				printf("Number of elements: %zd\n", ARRAY_SIZE);

				for (j = 0; j < ARRAY_SIZE; j++) {
					x[j] = rand();
					y[j] = 0;
				}
			}

			QueryPerformanceCounter(&li);
			counterStart = li.QuadPart;

			nonsimd_stencil(ARRAY_SIZE, x, y);

			QueryPerformanceCounter(&li);
			time_taken = 1000.0 * ((li.QuadPart - counterStart) / pcFreq);
			printf("Time in NON-SIMD = %f ms\n", time_taken);

			average_time = average_time + time_taken;

			if (i == 60) {
				printf("Average Time in NON-SIMD (2^20): %f ms\n", average_time / 30);
				average_time = 0;

				// -30 indeces
				printf("First 10 elements of Y -> ");
				//
				printf("\nLast 10 elements of Y -> ");
				//
				printf("\n");
			}
		}

		// ============= XMM VERSION =============
		else if (i > 60 && i <= 90) {
			if (i == 61) {
				printf("\n=== SIMD XMM (2^20) ===\n");
				printf("Number of elements: %zd\n", ARRAY_SIZE);

				for (j = 0; j < ARRAY_SIZE; j++) {
					x[j] = rand();
					y[j] = 0;
				}
			}

			QueryPerformanceCounter(&li);
			counterStart = li.QuadPart;

			simdxmm_stencil(ARRAY_SIZE, x, y);

			QueryPerformanceCounter(&li);
			time_taken = 1000.0 * ((li.QuadPart - counterStart) / pcFreq);
			printf("Time in SIMD XMM = %f ms\n", time_taken);

			average_time = average_time + time_taken;

			if (i == 90) {
				printf("Average Time in SIMD XMM (2^20): %f ms\n", average_time / 30);
				average_time = 0;

				//-60 indeces
				printf("First 10 elements of Y -> ");
				//
				printf("\nLast 10 elements of Y -> ");
				//
				printf("\n");
			}
		}

		// ============= YMM VERSION =============
		else if (i > 90 && i <= 120) {
			if (i == 91) {
				printf("\n=== SIMD YMM (2^20) ===\n");
				printf("Number of elements: %zd\n", ARRAY_SIZE);

				for (j = 0; j < ARRAY_SIZE; j++) {
					x[j] = rand();
					y[j] = 0;
				}
			}

			QueryPerformanceCounter(&li);
			counterStart = li.QuadPart;

			simdymm_stencil(ARRAY_SIZE, x, y);

			QueryPerformanceCounter(&li);
			time_taken = 1000.0 * ((li.QuadPart - counterStart) / pcFreq);
			printf("Time in SIMD YMM = %f ms\n", time_taken);

			average_time = average_time + time_taken;

			if (i == 120) {
				printf("Average Time in SIMD YMM (2^20): %f ms\n", average_time / 30);
				average_time = 0;

				//-90 indeces
				printf("First 10 elements of Y -> ");
				//
				printf("\nLast 10 elements of Y -> ");
				//
				printf("\n");
			}
		}
	}

	// TO-DO 2: error checking

	// TO-DO 3: for easy testing, we'll only have 2^20 first but once all functions are 100% working, we'll include 2^26, 2^30, and 2^31

	//free(x);
	//free(y);

	return 0;
}