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
	srand(time(NULL));

	size_t ARRAY_SIZE = 1 << 20;
	size_t ARRAY_BYTES = ARRAY_SIZE * sizeof(int32_t);

	// COUNTER VARIABLES
	int i, j, lastIndex, fail;

	// TIMER VARIABLES
	// reference: https://cse.usf.edu/~kchriste/tools/timeitWin.c
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

	// array initialization
	int32_t* x, * y, * answerKey;
	x = (int32_t*)malloc(ARRAY_BYTES);
	y = (int32_t*)malloc(ARRAY_BYTES);
	answerKey = (int32_t*)malloc(ARRAY_BYTES);
	for (j = 0; j < ARRAY_SIZE; j++) {
		x[j] = rand() % 16;
		y[j] = 0;
	}

	printf("================== 2^20 ==================\n\n");

	// ----------- 2 ^ 20 -----------
	for (i = 0; i < 120; i++) {
		if (i < 30) {
			if (i == 0) {
				printf("=== C (2^20) ===\n");
				printf("Number of elements: %zd\n", ARRAY_SIZE);
			}

			c_stencil(ARRAY_SIZE, x, y); // disregard time of every first run

			QueryPerformanceCounter(&li);
			counterStart = li.QuadPart;

			c_stencil(ARRAY_SIZE, x, y);

			QueryPerformanceCounter(&li);
			time_taken = 1000.0 * ((li.QuadPart - counterStart) / pcFreq);
			printf("[%d] Time in C = %f ms\n", i+1, time_taken);

			average_time = average_time + time_taken;

			if (i == 29) {
				printf("Average Time in C (2^20): %f ms\n", average_time / 30);
				average_time = 0;

				// get last index
				for (j = (ARRAY_SIZE - 1);j > 0;j--) {
					if (y[j] != 0) {
						lastIndex = j;
						break;
					}
				};

				// copy y of C to answer key
				for (j = 0; j < lastIndex; j++) {
					answerKey[j] = y[j];
				}

				printf("First 10 elements of Y -> ");
				for (j = 0;j < 10;j++) {
					if (j <= lastIndex && y[j] != 0) // don't print beyond last index
						printf("%d ", y[j]);
				}
				printf("\nLast 10 elements of Y -> ");
				for (j = (lastIndex-10);j <= lastIndex;j++) {
					if (j >= 0 && y[j] != 0) // dont print beyond first index
						printf("%d ", y[j]);
				};

				printf("\n");
			}
		}

		// ============= NONSIMD VERSION =============
		else if (i >= 30 && i < 60) {
			if (i == 30) {
				printf("\n=== NON-SIMD (2^20) ===\n");
				printf("Number of elements: %zd\n", ARRAY_SIZE);

				for (j = 0; j < ARRAY_SIZE; j++) {
					y[j] = 0;
				}
			}

			nonsimd_stencil(ARRAY_SIZE, x, y); // disregard time of every first run

			QueryPerformanceCounter(&li);
			counterStart = li.QuadPart;

			nonsimd_stencil(ARRAY_SIZE, x, y);

			QueryPerformanceCounter(&li);
			time_taken = 1000.0 * ((li.QuadPart - counterStart) / pcFreq);
			printf("[%d] Time in NON-SIMD = %f ms\n", i-29, time_taken);

			average_time = average_time + time_taken;

			if (i == 59) {
				printf("Average Time in NON-SIMD (2^20): %f ms\n", average_time / 30);
				average_time = 0;

				// get last index
				for (j = (ARRAY_SIZE - 1);j > 0;j--) {
					if (y[j] != 0) {
						lastIndex = j;
						break;
					}
				};

				printf("First 10 elements of Y -> ");
				for (j = 0;j < 10;j++) {
					if (j <= lastIndex && y[j] != 0) // don't print beyond last index
						printf("%d ", y[j]);
				}
				printf("\nLast 10 elements of Y -> ");
				for (j = (lastIndex - 10);j <= lastIndex;j++) {
					if (j >= 0 && y[j] != 0) // dont print beyond first index
						printf("%d ", y[j]);
				};

				printf("\n");

				fail = 0;
				for (j = 0; j < lastIndex; j++) {
					if (answerKey[j] != y[j])
						fail += 1;
				}
				if (fail > 0)
					printf("NON-SIMD IS INCORRECT\n");
				else
					printf("NON-SIMD IS CORRECT\n");
			}
		}

		// ============= XMM VERSION =============
		else if (i >= 60 && i < 90) {
			if (i == 60) {
				printf("\n=== SIMD XMM (2^20) ===\n");
				printf("Number of elements: %zd\n", ARRAY_SIZE);

				for (j = 0; j < ARRAY_SIZE; j++) {
					y[j] = 0;
				}
			}

			simdxmm_stencil(ARRAY_SIZE, x, y); // disregard time of every first run

			QueryPerformanceCounter(&li);
			counterStart = li.QuadPart;

			simdxmm_stencil(ARRAY_SIZE, x, y);

			QueryPerformanceCounter(&li);
			time_taken = 1000.0 * ((li.QuadPart - counterStart) / pcFreq);
			printf("[%d] Time in SIMD XMM = %f ms\n", i-59, time_taken);

			average_time = average_time + time_taken;

			if (i == 89) {
				printf("Average Time in SIMD XMM (2^20): %f ms\n", average_time / 30);
				average_time = 0;

				// get last index
				for (j = (ARRAY_SIZE - 1);j > 0;j--) {
					if (y[j] != 0) {
						lastIndex = j;
						break;
					}
				};

				printf("First 10 elements of Y -> ");
				for (j = 0;j < 10;j++) {
					if (j <= lastIndex && y[j] != 0) // don't print beyond last index
						printf("%d ", y[j]);
				}
				printf("\nLast 10 elements of Y -> ");
				for (j = (lastIndex - 10);j <= lastIndex;j++) {
					if (j >= 0 && y[j] != 0) // dont print beyond first index
						printf("%d ", y[j]);
				};

				printf("\n");

				fail = 0;
				for (j = 0; j < lastIndex; j++) {
					if (answerKey[j] != y[j])
						fail += 1;
				}
				if (fail > 0)
					printf("SIMD XMM IS INCORRECT\n");
				else
					printf("SIMD XMM IS CORRECT\n");
			}
		}

		// ============= YMM VERSION =============
		else if (i >= 90 && i < 120) {
			if (i == 90) {
				printf("\n=== SIMD YMM (2^20) ===\n");
				printf("Number of elements: %zd\n", ARRAY_SIZE);

				for (j = 0; j < ARRAY_SIZE; j++) {
					y[j] = 0;
				}
			}

			simdymm_stencil(ARRAY_SIZE, x, y); // disregard time of every first run

			QueryPerformanceCounter(&li);
			counterStart = li.QuadPart;

			simdymm_stencil(ARRAY_SIZE, x, y);

			QueryPerformanceCounter(&li);
			time_taken = 1000.0 * ((li.QuadPart - counterStart) / pcFreq);
			printf("[%d] Time in SIMD YMM = %f ms\n", i-89, time_taken);

			average_time = average_time + time_taken;

			if (i == 119) {
				printf("Average Time in SIMD YMM (2^20): %f ms\n", average_time / 30);
				average_time = 0;

				// get last index
				for (j = (ARRAY_SIZE - 1);j > 0;j--) {
					if (y[j] != 0) {
						lastIndex = j;
						break;
					}
				};

				printf("First 10 elements of Y -> ");
				for (j = 0;j < 10;j++) {
					if (j <= lastIndex && y[j] != 0) // don't print beyond last index
						printf("%d ", y[j]);
				}
				printf("\nLast 10 elements of Y -> ");
				for (j = (lastIndex - 10);j <= lastIndex;j++) {
					if (j >= 0 && y[j] != 0) // dont print beyond first index
						printf("%d ", y[j]);
				};

				printf("\n");

				fail = 0;
				for (j = 0; j < lastIndex; j++) {
					if (answerKey[j] != y[j])
						fail += 1;
				}
				if (fail > 0)
					printf("SIMD YMM IS INCORRECT\n");
				else
					printf("SIMD YMM IS CORRECT\n");
			}
		}
	}

	srand(time(NULL)+5);

	free(x);
	free(y);
	free(answerKey);

	ARRAY_SIZE = 1 << 26;
	ARRAY_BYTES = ARRAY_SIZE * sizeof(int32_t);

	// array re-initialization
	x = (int32_t*)malloc(ARRAY_BYTES);
	y = (int32_t*)malloc(ARRAY_BYTES);
	answerKey = (int32_t*)malloc(ARRAY_BYTES);
	for (j = 0; j < ARRAY_SIZE; j++) {
		x[j] = rand() % 16;
		y[j] = 0;
	}

	printf("\n\n================== 2^26 ==================\n\n");

	// ----------- 2 ^ 26 -----------
	for (i = 0; i < 120; i++) {
		if (i < 30) {
			if (i == 0) {
				printf("=== C (2^26) ===\n");
				printf("Number of elements: %zd\n", ARRAY_SIZE);
			}

			c_stencil(ARRAY_SIZE, x, y); // disregard time of every first run

			QueryPerformanceCounter(&li);
			counterStart = li.QuadPart;

			c_stencil(ARRAY_SIZE, x, y);

			QueryPerformanceCounter(&li);
			time_taken = 1000.0 * ((li.QuadPart - counterStart) / pcFreq);
			printf("[%d] Time in C = %f ms\n", i + 1, time_taken);

			average_time = average_time + time_taken;

			if (i == 29) {
				printf("Average Time in C (2^26): %f ms\n", average_time / 30);
				average_time = 0;

				// get last index
				for (j = (ARRAY_SIZE - 1);j > 0;j--) {
					if (y[j] != 0) {
						lastIndex = j;
						break;
					}
				};

				// copy y of C to answer key
				for (j = 0; j < lastIndex; j++) {
					answerKey[j] = y[j];
				}

				printf("First 10 elements of Y -> ");
				for (j = 0;j < 10;j++) {
					if (j <= lastIndex && y[j] != 0) // don't print beyond last index
						printf("%d ", y[j]);
				}
				printf("\nLast 10 elements of Y -> ");
				for (j = (lastIndex - 10);j <= lastIndex;j++) {
					if (j >= 0 && y[j] != 0) // dont print beyond first index
						printf("%d ", y[j]);
				};

				printf("\n");
			}
		}

		// ============= NONSIMD VERSION =============
		else if (i >= 30 && i < 60) {
			if (i == 30) {
				printf("\n=== NON-SIMD (2^26) ===\n");
				printf("Number of elements: %zd\n", ARRAY_SIZE);

				for (j = 0; j < ARRAY_SIZE; j++) {
					y[j] = 0;
				}
			}

			nonsimd_stencil(ARRAY_SIZE, x, y); // disregard time of every first run

			QueryPerformanceCounter(&li);
			counterStart = li.QuadPart;

			nonsimd_stencil(ARRAY_SIZE, x, y);

			QueryPerformanceCounter(&li);
			time_taken = 1000.0 * ((li.QuadPart - counterStart) / pcFreq);
			printf("[%d] Time in NON-SIMD = %f ms\n", i - 29, time_taken);

			average_time = average_time + time_taken;

			if (i == 59) {
				printf("Average Time in NON-SIMD (2^26): %f ms\n", average_time / 30);
				average_time = 0;

				// get last index
				for (j = (ARRAY_SIZE - 1);j > 0;j--) {
					if (y[j] != 0) {
						lastIndex = j;
						break;
					}
				};

				printf("First 10 elements of Y -> ");
				for (j = 0;j < 10;j++) {
					if (j <= lastIndex && y[j] != 0) // don't print beyond last index
						printf("%d ", y[j]);
				}
				printf("\nLast 10 elements of Y -> ");
				for (j = (lastIndex - 10);j <= lastIndex;j++) {
					if (j >= 0 && y[j] != 0) // dont print beyond first index
						printf("%d ", y[j]);
				};

				printf("\n");

				fail = 0;
				for (j = 0; j < lastIndex; j++) {
					if (answerKey[j] != y[j])
						fail += 1;
				}
				if (fail > 0)
					printf("NON-SIMD IS INCORRECT\n");
				else
					printf("NON-SIMD IS CORRECT\n");
			}
		}

		// ============= XMM VERSION =============
		else if (i >= 60 && i < 90) {
			if (i == 60) {
				printf("\n=== SIMD XMM (2^26) ===\n");
				printf("Number of elements: %zd\n", ARRAY_SIZE);

				for (j = 0; j < ARRAY_SIZE; j++) {
					y[j] = 0;
				}
			}

			simdxmm_stencil(ARRAY_SIZE, x, y); // disregard time of every first run

			QueryPerformanceCounter(&li);
			counterStart = li.QuadPart;

			simdxmm_stencil(ARRAY_SIZE, x, y);

			QueryPerformanceCounter(&li);
			time_taken = 1000.0 * ((li.QuadPart - counterStart) / pcFreq);
			printf("[%d] Time in SIMD XMM = %f ms\n", i - 59, time_taken);

			average_time = average_time + time_taken;

			if (i == 89) {
				printf("Average Time in SIMD XMM (2^26): %f ms\n", average_time / 30);
				average_time = 0;

				// get last index
				for (j = (ARRAY_SIZE - 1);j > 0;j--) {
					if (y[j] != 0) {
						lastIndex = j;
						break;
					}
				};

				printf("First 10 elements of Y -> ");
				for (j = 0;j < 10;j++) {
					if (j <= lastIndex && y[j] != 0) // don't print beyond last index
						printf("%d ", y[j]);
				}
				printf("\nLast 10 elements of Y -> ");
				for (j = (lastIndex - 10);j <= lastIndex;j++) {
					if (j >= 0 && y[j] != 0) // dont print beyond first index
						printf("%d ", y[j]);
				};

				printf("\n");

				fail = 0;
				for (j = 0; j < lastIndex; j++) {
					if (answerKey[j] != y[j])
						fail += 1;
				}
				if (fail > 0)
					printf("SIMD XMM IS INCORRECT\n");
				else
					printf("SIMD XMM IS CORRECT\n");
			}
		}

		// ============= YMM VERSION =============
		else if (i >= 90 && i < 120) {
			if (i == 90) {
				printf("\n=== SIMD YMM (2^26) ===\n");
				printf("Number of elements: %zd\n", ARRAY_SIZE);

				for (j = 0; j < ARRAY_SIZE; j++) {
					y[j] = 0;
				}
			}

			simdymm_stencil(ARRAY_SIZE, x, y); // disregard time of every first run

			QueryPerformanceCounter(&li);
			counterStart = li.QuadPart;

			simdymm_stencil(ARRAY_SIZE, x, y);

			QueryPerformanceCounter(&li);
			time_taken = 1000.0 * ((li.QuadPart - counterStart) / pcFreq);
			printf("[%d] Time in SIMD YMM = %f ms\n", i - 89, time_taken);

			average_time = average_time + time_taken;

			if (i == 119) {
				printf("Average Time in SIMD YMM (2^26): %f ms\n", average_time / 30);
				average_time = 0;

				// get last index
				for (j = (ARRAY_SIZE - 1);j > 0;j--) {
					if (y[j] != 0) {
						lastIndex = j;
						break;
					}
				};

				printf("First 10 elements of Y -> ");
				for (j = 0;j < 10;j++) {
					if (j <= lastIndex && y[j] != 0) // don't print beyond last index
						printf("%d ", y[j]);
				}
				printf("\nLast 10 elements of Y -> ");
				for (j = (lastIndex - 10);j <= lastIndex;j++) {
					if (j >= 0 && y[j] != 0) // dont print beyond first index
						printf("%d ", y[j]);
				};

				printf("\n");

				fail = 0;
				for (j = 0; j < lastIndex; j++) {
					if (answerKey[j] != y[j])
						fail += 1;
				}
				if (fail > 0)
					printf("SIMD YMM IS INCORRECT\n");
				else
					printf("SIMD YMM IS CORRECT\n");
			}
		}
	}
	
	srand(time(NULL) + 10);

	free(x);
	free(y);
	free(answerKey);

	ARRAY_SIZE = 1 << 30;
	ARRAY_BYTES = ARRAY_SIZE * sizeof(int32_t);

	// array re-initialization
	x = (int32_t*)malloc(ARRAY_BYTES);
	y = (int32_t*)malloc(ARRAY_BYTES);
	answerKey = (int32_t*)malloc(ARRAY_BYTES);
	for (j = 0; j < ARRAY_SIZE; j++) {
		x[j] = rand() % 16;
		y[j] = 0;
	}

	printf("\n\n================== 2^30 ==================\n\n");

	// ----------- 2 ^ 30 -----------
	for (i = 0; i < 120; i++) {
		if (i < 30) {
			if (i == 0) {
				printf("=== C (2^30) ===\n");
				printf("Number of elements: %zd\n", ARRAY_SIZE);
			}

			c_stencil(ARRAY_SIZE, x, y); // disregard time of every first run

			QueryPerformanceCounter(&li);
			counterStart = li.QuadPart;

			c_stencil(ARRAY_SIZE, x, y);

			QueryPerformanceCounter(&li);
			time_taken = 1000.0 * ((li.QuadPart - counterStart) / pcFreq);
			printf("[%d] Time in C = %f ms\n", i + 1, time_taken);

			average_time = average_time + time_taken;

			if (i == 29) {
				printf("Average Time in C (2^26): %f ms\n", average_time / 30);
				average_time = 0;

				// get last index
				for (j = (ARRAY_SIZE - 1);j > 0;j--) {
					if (y[j] != 0) {
						lastIndex = j;
						break;
					}
				};

				// copy y of C to answer key
				for (j = 0; j < lastIndex; j++) {
					answerKey[j] = y[j];
				}

				printf("First 10 elements of Y -> ");
				for (j = 0;j < 10;j++) {
					if (j <= lastIndex && y[j] != 0) // don't print beyond last index
						printf("%d ", y[j]);
				}
				printf("\nLast 10 elements of Y -> ");
				for (j = (lastIndex - 10);j <= lastIndex;j++) {
					if (j >= 0 && y[j] != 0) // dont print beyond first index
						printf("%d ", y[j]);
				};

				printf("\n");
			}
		}

		// ============= NONSIMD VERSION =============
		else if (i >= 30 && i < 60) {
			if (i == 30) {
				printf("\n=== NON-SIMD (2^30) ===\n");
				printf("Number of elements: %zd\n", ARRAY_SIZE);

				for (j = 0; j < ARRAY_SIZE; j++) {
					y[j] = 0;
				}
			}

			nonsimd_stencil(ARRAY_SIZE, x, y); // disregard time of every first run

			QueryPerformanceCounter(&li);
			counterStart = li.QuadPart;

			nonsimd_stencil(ARRAY_SIZE, x, y);

			QueryPerformanceCounter(&li);
			time_taken = 1000.0 * ((li.QuadPart - counterStart) / pcFreq);
			printf("[%d] Time in NON-SIMD = %f ms\n", i - 29, time_taken);

			average_time = average_time + time_taken;

			if (i == 59) {
				printf("Average Time in NON-SIMD (2^30): %f ms\n", average_time / 30);
				average_time = 0;

				// get last index
				for (j = (ARRAY_SIZE - 1);j > 0;j--) {
					if (y[j] != 0) {
						lastIndex = j;
						break;
					}
				};

				printf("First 10 elements of Y -> ");
				for (j = 0;j < 10;j++) {
					if (j <= lastIndex && y[j] != 0) // don't print beyond last index
						printf("%d ", y[j]);
				}
				printf("\nLast 10 elements of Y -> ");
				for (j = (lastIndex - 10);j <= lastIndex;j++) {
					if (j >= 0 && y[j] != 0) // dont print beyond first index
						printf("%d ", y[j]);
				};

				printf("\n");

				fail = 0;
				for (j = 0; j < lastIndex; j++) {
					if (answerKey[j] != y[j])
						fail += 1;
				}
				if (fail > 0)
					printf("NON-SIMD IS INCORRECT\n");
				else
					printf("NON-SIMD IS CORRECT\n");
			}
		}

		// ============= XMM VERSION =============
		else if (i >= 60 && i < 90) {
			if (i == 60) {
				printf("\n=== SIMD XMM (2^30) ===\n");
				printf("Number of elements: %zd\n", ARRAY_SIZE);

				for (j = 0; j < ARRAY_SIZE; j++) {
					y[j] = 0;
				}
			}

			simdxmm_stencil(ARRAY_SIZE, x, y); // disregard time of every first run

			QueryPerformanceCounter(&li);
			counterStart = li.QuadPart;

			simdxmm_stencil(ARRAY_SIZE, x, y);

			QueryPerformanceCounter(&li);
			time_taken = 1000.0 * ((li.QuadPart - counterStart) / pcFreq);
			printf("[%d] Time in SIMD XMM = %f ms\n", i - 59, time_taken);

			average_time = average_time + time_taken;

			if (i == 89) {
				printf("Average Time in SIMD XMM (2^30): %f ms\n", average_time / 30);
				average_time = 0;

				// get last index
				for (j = (ARRAY_SIZE - 1);j > 0;j--) {
					if (y[j] != 0) {
						lastIndex = j;
						break;
					}
				};

				printf("First 10 elements of Y -> ");
				for (j = 0;j < 10;j++) {
					if (j <= lastIndex && y[j] != 0) // don't print beyond last index
						printf("%d ", y[j]);
				}
				printf("\nLast 10 elements of Y -> ");
				for (j = (lastIndex - 10);j <= lastIndex;j++) {
					if (j >= 0 && y[j] != 0) // dont print beyond first index
						printf("%d ", y[j]);
				};

				printf("\n");

				fail = 0;
				for (j = 0; j < lastIndex; j++) {
					if (answerKey[j] != y[j])
						fail += 1;
				}
				if (fail > 0)
					printf("SIMD XMM IS INCORRECT\n");
				else
					printf("SIMD XMM IS CORRECT\n");
			}
		}

		// ============= YMM VERSION =============
		else if (i >= 90 && i < 120) {
			if (i == 90) {
				printf("\n=== SIMD YMM (2^30) ===\n");
				printf("Number of elements: %zd\n", ARRAY_SIZE);

				for (j = 0; j < ARRAY_SIZE; j++) {
					y[j] = 0;
				}
			}

			simdymm_stencil(ARRAY_SIZE, x, y); // disregard time of every first run

			QueryPerformanceCounter(&li);
			counterStart = li.QuadPart;

			simdymm_stencil(ARRAY_SIZE, x, y);

			QueryPerformanceCounter(&li);
			time_taken = 1000.0 * ((li.QuadPart - counterStart) / pcFreq);
			printf("[%d] Time in SIMD YMM = %f ms\n", i - 89, time_taken);

			average_time = average_time + time_taken;

			if (i == 119) {
				printf("Average Time in SIMD YMM (2^30): %f ms\n", average_time / 30);
				average_time = 0;

				// get last index
				for (j = (ARRAY_SIZE - 1);j > 0;j--) {
					if (y[j] != 0) {
						lastIndex = j;
						break;
					}
				};

				printf("First 10 elements of Y -> ");
				for (j = 0;j < 10;j++) {
					if (j <= lastIndex && y[j] != 0) // don't print beyond last index
						printf("%d ", y[j]);
				}
				printf("\nLast 10 elements of Y -> ");
				for (j = (lastIndex - 10);j <= lastIndex;j++) {
					if (j >= 0 && y[j] != 0) // dont print beyond first index
						printf("%d ", y[j]);
				};

				printf("\n");

				fail = 0;
				for (j = 0; j < lastIndex; j++) {
					if (answerKey[j] != y[j])
						fail += 1;
				}
				if (fail > 0)
					printf("SIMD YMM IS INCORRECT\n");
				else
					printf("SIMD YMM IS CORRECT\n");
			}
		}
	}

	return 0;
}