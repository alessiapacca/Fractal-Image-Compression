#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdio.h>
#include <stdbool.h>
#define myInt64 unsigned long long
//#define OPS
//#define IDX_OPS
#define COMPRESS_CYCLES

#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) 
#endif


void initializeBenchmark(char* funcName);
void incrementFlopCount(int i_add, int i_mul, int i_sub, int i_div, int f_add, int f_mul, int f_sub, int f_div, bool print_details);
void incrementTransferredBytes(int bytes_fp, int bytes_int, bool print_details);
myInt64 startBenchmark();
void stopBenchmark(myInt64 startTime, char* funcName);
void setParameters(char* imgName, char* benchmark_file);
#endif