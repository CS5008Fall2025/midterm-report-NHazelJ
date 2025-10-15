#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "fibonacci.h"
#include <windows.h>

// methods suffixed with c
#define CSV_FILENAME "fibResultsC.csv"
#define SKIP_RECURSION_AFTER_N 30
#define MAX_SAFE_N_FOR_VALUE 93

// static variable freq stores the number of ticks per second
// static makes the variable keep its value between calls
// queryperformancecounter only call once
static inline double now_ms(void) {
    static LARGE_INTEGER freq = {0};
    // checks if it's initialized
    if (freq.QuadPart == 0) {
        // getting ticks per second filling freq
        QueryPerformanceFrequency(&freq);
    }
    // creating variable t to store current ticks
    LARGE_INTEGER t;
    // getting current ticks
    QueryPerformanceCounter(&t);
    // return ms
    return (1000.0 * (double)t.QuadPart) / (double)freq.QuadPart;
}
// checking if a file exists so we know whether to write the header
static bool file_exists(const char *path) {
    FILE *f = fopen(path, "r");
    if (f) { fclose(f); 
        return true; 
    }
    return false;
}

int main(void) {
    unsigned int n;
    printf("Enter your n: ");
    if (scanf("%u", &n) != 1) {
        fprintf(stderr, "invalid input.\n");
        return 1;
    }

    // open csv file for appending
    bool exists = file_exists(CSV_FILENAME);
    FILE *csv = fopen(CSV_FILENAME, "a");
    if (!csv) {
        fprintf(stderr, "could not open %s for writing.\n", CSV_FILENAME);
        return 1;
    }
    if (!exists) {
        fprintf(csv, "n,method,value,ops,time_ms\n");
    }

    // print 1..N values using iterative
    printf("\nfibonacci series from 1..N values for iterative function:\n");
    for (unsigned int i = 1; i <= n; ++i) {
        FibResult r = fibonacci_iterative(i);
        printf("%llu ", r.value);
    }
    printf("\n");

    // for each i, time each method and append a csv row
    for (unsigned int i = 1; i <= n; ++i) {
        // so small that we need to repeat to get accurate timing so we called them many times
        // and take the average time
        // This helps balance accuracy and runtime.
        // because function are fast for small n, but slow for large n
        int repeats_iter;
        if (i < 200)
            repeats_iter = 5000;
        else if (i < 1000)
            repeats_iter = 1000;
        else
            repeats_iter = 200;

        // same logic for memoization version
        int repeats_memo;
        if (i < 200)
            repeats_memo = 2000;
        else if (i < 1000)
            repeats_memo = 500;
        else
            repeats_memo = 100;

        // recursion, we keep the repeats small because
        // recursion is much slower and could take a long time.
        int repeats_rec;
        if (i < 20)
            repeats_rec = 100;
        else
            repeats_rec = 10;

        // iterative
        // record current time in ms
        double start = now_ms();
        // create a result struct to hold initial value
        FibResult it = {0,0};
        // run fib iteration
        for (int r = 0; r < repeats_iter; ++r) {
            it = fibonacci_iterative(i);
        }
        // measure end time
        double end = now_ms();
        // average time per call
        double time_iter_ms = (end - start) / repeats_iter;
        // write a csv row
        fprintf(csv, "%u,iterative,%llu,%llu,%.9f\n", i, it.value, it.ops, time_iter_ms);

        // memoization
        start = now_ms();
        FibResult dp = {0,0};
        for (int r = 0; r < repeats_memo; ++r) {
            dp = fibonacci_memoization(i);
        }
        end = now_ms();
        double time_memo_ms = (end - start) / repeats_memo;
        fprintf(csv, "%u,memoization,%llu,%llu,%.9f\n", i, dp.value, dp.ops, time_memo_ms);

        // recursive
        if (i <= SKIP_RECURSION_AFTER_N) {
            start = now_ms();
            FibResult rc = {0,0};
            for (int r = 0; r < repeats_rec; ++r) {
                rc = fibonacci_recursive(i);
            }
            end = now_ms();
            double time_rec_ms = (end - start) / repeats_rec;
            fprintf(csv, "%u,recursive,%llu,%llu,%.9f\n", i, rc.value, rc.ops, time_rec_ms);
        }
    }


    fclose(csv);

    // console summary for final n
    printf("\nIterative Fibonacci\n");
    double t0 = now_ms();
    FibResult itN = fibonacci_iterative(n);
    double t1 = now_ms();
    printf("Result = %llu\nOps = %llu\nTime = %.6f ms\n", itN.value, itN.ops, (t1 - t0));

    printf("\nrecursive Fibonacci\n");
    if (n > SKIP_RECURSION_AFTER_N) {
        printf("Skipping for n > %d (too slow).\n", SKIP_RECURSION_AFTER_N);
    } else {
        t0 = now_ms();
        FibResult rcN = fibonacci_recursive(n);
        t1 = now_ms();
        printf("result = %llu\nOps = %llu\nTime = %.6f ms\n", rcN.value, rcN.ops, (t1 - t0));
    }

    printf("\nDP memoization Fibonacci\n");
    t0 = now_ms();
    FibResult dpN = fibonacci_memoization(n);
    t1 = now_ms();
    printf("result = %llu\nOps = %llu\nTime = %.6f ms\n", dpN.value, dpN.ops, (t1 - t0));

    printf("\n csv appended: \"%s\"\n", CSV_FILENAME);

    if (n > MAX_SAFE_N_FOR_VALUE) {
        printf("\nlookout for n > %d, unsigned long long overflows, times/ops are still valid, but values wrap.\n", MAX_SAFE_N_FOR_VALUE);
    }
    return 0;
}