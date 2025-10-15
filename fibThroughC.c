#include "fibonacci.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>


// iterative function
// returns F(n) and the number of additions performed one per loop
FibResult fibonacci_iterative(unsigned int n) {
    if (n <= 1) {
        // for n = 0 or 1 value = n and no additions needed
        // 0ULL means 0 unsigned long long and respectively
        FibResult r = { (unsigned long long)n, 0ULL };
        return r;
    }

    // start from F(0)=0 F(1)=1 and go to n
    // 0ULL means 0 unsigned long long and respectively
    unsigned long long a = 0ULL;
    unsigned long long b = 1ULL;
    unsigned long long ops = 0ULL;

    // each next = a + b, one addition), then shift forward
    for (unsigned int i = 2; i <= n; ++i) {
        unsigned long long tmp = a + b;
        a = b;
        b = tmp;
         // count this addition
        ops += 1ULL;
    }

    FibResult r = { b, ops };
    return r;
}

// recursive function

// helper returns f(k) increments *ops each time we do F(k-1) + F(k-2)
static unsigned long long fib_recursive_helper(unsigned int k, unsigned long long *ops) {
    if (k <= 1) {
        // base case F(0)=0, F(1)=1
        return (unsigned long long)k;
    }
    // add two subresults toward count one addition
    (*ops) += 1ULL;
    return fib_recursive_helper(k - 1, ops) + fib_recursive_helper(k - 2, ops);
}

// wrap that returns FibResult
FibResult fibonacci_recursive(unsigned int n) {
    unsigned long long ops = 0ULL;
    unsigned long long val = fib_recursive_helper(n, &ops);
    FibResult r = { val, ops };
    return r;
}

// memoization DP function
// store memoization values in an array ULLONG_MAX means empty
static const unsigned long long MEMO_EMPTY = ULLONG_MAX;

// helper compute F(k) with memo array, count one addition when combining
static unsigned long long fib_memo_helper(int k, unsigned long long *memo, unsigned long long *ops) {
    if (memo[k] != MEMO_EMPTY) {
        // already computed so return saved value, no new addition needed
        return memo[k];
    }
    if (k <= 1) {
        // base case, store and return 0 or 1
        memo[k] = (unsigned long long)k;
        return memo[k];
    }
    // recursively solve subproblems of the fibonacci
    unsigned long long left = fib_memo_helper(k - 1, memo, ops);
    unsigned long long right = fib_memo_helper(k - 2, memo, ops);

    // combining left + right is one addition
    (*ops) += 1ULL;
    memo[k] = left + right;
    return memo[k];
}

// public memoized function, allocates a fresh memo per call so per n timing is fair.
FibResult fibonacci_memoization(unsigned int n) {
    if (n <= 1) {
        FibResult r = { (unsigned long long)n, 0ULL };
        return r;
    }

    // allocate memo table of size n+1 and mark as empty
    unsigned long long *memo = (unsigned long long*)malloc((n + 1) * sizeof(unsigned long long));
    if (!memo) {
        // allocation failed, fall back to iterative so program still works
        return fibonacci_iterative(n);
    }
    for (unsigned int i = 0; i <= n; ++i) {
        memo[i] = MEMO_EMPTY;
    }

    unsigned long long ops = 0ULL;
    unsigned long long val = fib_memo_helper((int)n, memo, &ops);

    // free memo to avoid memory leaks
    free(memo);

    FibResult r = { val, ops };
    return r;
}
