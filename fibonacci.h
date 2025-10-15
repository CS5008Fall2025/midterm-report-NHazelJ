#ifndef FIBONACCI_H
#define FIBONACCI_H

// fibonacci result and how many additions ops performed
typedef struct {
    // F(n) will overflow > ~93 for 64 bit integers
    unsigned long long value;
    // number of addition operations
    unsigned long long ops;
} FibResult;

// prototypes
FibResult fibonacci_iterative(unsigned int n);
FibResult fibonacci_recursive(unsigned int n);
FibResult fibonacci_memoization(unsigned int n);

#endif // FIBONACCI_H