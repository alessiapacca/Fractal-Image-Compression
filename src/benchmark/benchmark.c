#include "benchmark.h"
#include "tsc_x86.h"
#define CPU_FREQUENCY 2.3

unsigned long long int transferred_bytes_float_op;
unsigned long long int transferred_bytes_int_op;
unsigned long long int int_mul;
unsigned long long int int_adds;
unsigned long long int int_subs;
unsigned long long int int_divs;
unsigned long long int fp_mul;
unsigned long long int fp_adds;
unsigned long long int fp_subs;
unsigned long long int fp_divs;
unsigned long long int total_flops;
char* img_name;
char* FILE_NAME;


int exists_file(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0; // if not exist
}

void setParameters(char* imgName, char* benchmark_file){
    img_name = imgName;
    FILE_NAME = benchmark_file;
}

/**
 * Initialize benchmark parameters
 * @param funcName the name of the function to benchmark
 */
void initializeBenchmark(char* funcName){
    DEBUG_PRINT("\n----------------------------------------------\n");
    DEBUG_PRINT(("INITIALIZE BENCHMARK FOR %s \n", funcName));
    int_adds = 0;
    int_mul = 0;
    int_subs = 0;
    int_divs = 0;
    fp_adds = 0;
    fp_mul = 0;
    fp_subs = 0;
    fp_divs = 0;
    total_flops = 0;
    transferred_bytes_float_op = 0;
    transferred_bytes_int_op = 0;
}

/**
 * Increment the flop count for each operation type
 * @param i_add number of integer adds to increment
 * @param i_mul number of integer mul to increment
 * @param f_add number of floating point adds to increment
 * @param f_mul number of floating point mul to increment
 * @param print_details if true print current details for each operation type
 */
void incrementFlopCount(int i_add, int i_mul, int i_sub, int i_div, int f_add, int f_mul, int f_sub, int f_div, bool print_details){
    int_adds += i_add;
    int_mul += i_mul;
    int_subs += i_sub;
    int_divs += i_div;

    fp_adds += f_add;
    fp_mul += f_mul;
    fp_subs += f_sub;
    fp_divs += f_div;

    total_flops =  int_adds + int_mul + int_subs + int_divs;
    if(print_details){
        DEBUG_PRINT("+++++++ DETAILS +++++++\n");
        DEBUG_PRINT(("TOTAL OPS: %llu\n", total_flops));

        DEBUG_PRINT(("- CURRENT OPS PER OPERATION -\n"));
        DEBUG_PRINT(("INTEGER ADDS: %d\n", int_adds));
        DEBUG_PRINT(("INTEGER MUL: %d\n", int_mul));
        DEBUG_PRINT(("INTEGER SUBS: %d\n", int_subs));
        DEBUG_PRINT(("INTEGER DIVS: %d\n", int_divs));

        DEBUG_PRINT(("FLOAT ADDS: %d\n", fp_adds));
        DEBUG_PRINT(("FLOAT MUL: %d\n", fp_mul));
        DEBUG_PRINT(("FLOAT SUBS: %d\n", fp_subs));
        DEBUG_PRINT(("FLOAT DIVS: %d\n", fp_divs));
        DEBUG_PRINT(("++++++++++++++++++++++++++++\n"));
    }
}

/**
 * Increment the counter of transferred bytes for floating point and integer operations.
 * @param bytes_fp bytes for floating point operations
 * @param bytes_int bytes for integer operations
 * @param print_details if true print current details of transferred bytes
 */
void incrementTransferredBytes(int bytes_fp, int bytes_int, bool print_details){
    transferred_bytes_float_op += bytes_fp;
    transferred_bytes_int_op += bytes_int;
    if(print_details){
        DEBUG_PRINT("+++++++ DETAILS +++++++\n");
        DEBUG_PRINT("- CURRENT TRANSFERRED BYTES -\n");
        DEBUG_PRINT(("BYTES FOR FP OPERATIONS: %d\n", transferred_bytes_float_op));
        DEBUG_PRINT(("BYTES FOR INT OPERATIONS: %d\n", transferred_bytes_int_op));
        DEBUG_PRINT("++++++++++++++++++++++++++++\n");
    }
}
/**
 * Start benchmark timer for CPU cycles
 * @return start time
 */
myInt64 startBenchmark(){
    printf("START BENCHMARK\n");
    myInt64 startTime = start_tsc();
    return startTime;
}

/**
 * Stop benchmark timer for CPU cycles.
 * @param startTime the start time of the benchmark
 * @param funcName the name of the benchmarked function
 * @param firstFile if true write also the csv column indexes
 */
void stopBenchmark(myInt64 startTime, char* funcName){
    myInt64 cycles = stop_tsc(startTime);
    printf("STOP BENCHMARK \n");

    DEBUG_PRINT(("TOTAL CYCLES: %llu , CPU FREQUENCY: %lf MHz\n", cycles, CPU_FREQUENCY));
    DEBUG_PRINT(("TOTAL OPS: %llu\n", total_flops));

    DEBUG_PRINT(("- CURRENT OPS PER OPERATION -\n"));
    DEBUG_PRINT(("INTEGER ADDS: %d\n", int_adds));
    DEBUG_PRINT(("INTEGER MUL: %d\n", int_mul));
    DEBUG_PRINT(("INTEGER SUBS: %d\n", int_subs));
    DEBUG_PRINT(("INTEGER DIVS: %d\n", int_divs));
    DEBUG_PRINT(("FLOAT ADDS: %d\n", fp_adds));
    DEBUG_PRINT(("FLOAT MUL: %d\n", fp_mul));
    DEBUG_PRINT(("FLOAT SUBS: %d\n", fp_subs));
    DEBUG_PRINT(("FLOAT DIVS: %d\n", fp_divs));

    DEBUG_PRINT("- CURRENT TRANSFERRED BYTES -\n");
    DEBUG_PRINT(("BYTES FOR FP OPERATIONS: %d\n", transferred_bytes_float_op));
    DEBUG_PRINT(("BYTES FOR INT OPERATIONS: %d\n", transferred_bytes_int_op));

    bool exist = exists_file(FILE_NAME);
    FILE * file = NULL;
    file = fopen(FILE_NAME, "a");

    if (!exist) {
        fprintf(file, "IMG_NAME,FUNC_NAME,TOT_CYCLES,TOT_FLOPS,I_ADDS,I_MULS,I_SUBS,I_DIVS,FP_ADDS,FP_MULS,FP_SUBS,FP_DIVS,CPU_FREQUENCY,BYTES_FP,BYTES_INT\n");
    }
    //IMG_NAME, FUNC_NAME, TOT_CYCLES, TOT_FLOPS, I_ADDS, I_MULS, I_SUBS, I_DIVS, FP_ADDS, FP_MULS, FP_SUBS, FP_DIVS, CPU_FREQUENCY, BYTES_FP, BYTES_INT
    fprintf(file, "%s,%s,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%f,%llu,%llu\n", img_name, funcName, cycles, total_flops, int_adds, int_mul, int_subs, int_divs, fp_adds, fp_mul, fp_subs, fp_divs, CPU_FREQUENCY, transferred_bytes_float_op, transferred_bytes_int_op);
    fclose(file);
}