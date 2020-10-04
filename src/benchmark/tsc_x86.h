/* ==================== GNU C and possibly other UNIX compilers ===================== */
#ifndef TSC_H
#define TSC_H

#ifndef WIN32

#if defined(__GNUC__) || defined(__linux__)
#define VOLATILE __volatile__
#define ASM __asm__
#endif

#define myInt64 unsigned long long
#define INT32 unsigned int

#endif

/* This is the RDTSC timer.
 * RDTSC is an instruction on several Intel and compatible CPUs that Reads the
 * Time Stamp Counter. The Intel manuals contain more information.
 */
#define COUNTER_VAL(a) ((a).int64)

/* ==================== GNU C and possibly other UNIX compilers ===================== */
#ifndef WIN32

typedef union
{       myInt64 int64;
    struct {INT32 lo, hi;} int32;
} tsc_counter;

#define RDTSC(cpu_c) \
	  ASM VOLATILE ("rdtsc" : "=a" ((cpu_c).int32.lo), "=d"((cpu_c).int32.hi))
#define CPUID() \
		ASM VOLATILE ("cpuid" : : "a" (0) : "bx", "cx", "dx" )

#endif


void init_tsc() {
    ; // no need to initialize anything for x86
}

myInt64 start_tsc(void) {
    tsc_counter start;
    CPUID();
    RDTSC(start);
    return COUNTER_VAL(start);
}

myInt64 stop_tsc(myInt64 start) {
    tsc_counter end;
    RDTSC(end);
    CPUID();
    return COUNTER_VAL(end) - start;
}

#endif