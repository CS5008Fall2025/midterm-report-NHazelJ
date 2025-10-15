# importing the time module to measure execution time
import time
# importing the fibonacci functions from fibonacciThroughPython.py
import fibonacciThroughPython as fib
import csv
import os
import sys

sys.setrecursionlimit(20000)
SKIP_RECURSION = 30
SKIP_MEMO_TOPDOWN_AFTER_N = 5000
CSV_FILENAME = "fibResultsPython2000.csv"

if __name__ == "__main__":
    # ask user for input
    n = int(input("Enter your n: "))
    
    print("\nFibonacci series from 1..N, values only:")
    for i in range(1, n + 1):
        # get the fibonacci value using the iterative method
        val_i, ops = fib.fibonacci_iterative(i)
        print(val_i, end=" ")
    print("\n") 
    
    write_header = not os.path.exists(CSV_FILENAME)
    with open(CSV_FILENAME, mode="a", newline="") as f:
        writer = csv.writer(f)
        if write_header:
            writer.writerow(["n", "method", "value", "ops", "time_ms"])
        # collect per n rows for each method
        for i in range(1, n + 1):
    
            # iterative
            start = time.perf_counter()
            iter_val, iter_ops = fib.fibonacci_iterative(i)
            end = time.perf_counter()
            time_iter_ms = (end - start) * 1000.0
            writer.writerow([i, "iterative", iter_val, iter_ops, f"{time_iter_ms:.6f}"])

            # memoization DP, fresh memo each i for fair per-n timing
            if i <= SKIP_MEMO_TOPDOWN_AFTER_N:
                start = time.perf_counter()
                val_m, ops_m = fib.fibonacci_memoization(i)
                end = time.perf_counter()
                time_memo_ms = (end - start) * 1000.0
                writer.writerow([i, "memoization", val_m, ops_m, f"{time_memo_ms:.6f}"])

            # recursion
            if i <= SKIP_RECURSION:
                start = time.perf_counter()
                rec_val, rec_ops = fib.fibonacci_recursive(i)
                end = time.perf_counter()
                time_rec_ms = (end - start) * 1000.0
                writer.writerow([i, "recursive", rec_val, rec_ops, f"{time_rec_ms:.6f}"])

    # final N run with printout
    print("\nIterative Fibonacci")
    start = time.perf_counter()
    iter_val, iter_ops = fib.fibonacci_iterative(n)
    end = time.perf_counter()
    print(f"Result = {iter_val}")
    print(f"Ops = {iter_ops}")
    print(f"Time = {(end - start) * 1000:.6f} ms")

    print("\nRecursive Fibonacci")
    if n > SKIP_RECURSION:
        print(f"Skipping recursive version for n > {SKIP_RECURSION}, very slow.")
    else:
        start = time.perf_counter()
        rec_val, rec_ops = fib.fibonacci_recursive(n)
        end = time.perf_counter()
        print(f"Result = {rec_val}")
        print(f"Ops = {rec_ops}")
        print(f"Time = {(end - start) * 1000:.6f} ms")

    print("\nDynamic Programming with memoization Fibonacci")
    start = time.perf_counter()
    dp_val, dp_ops = fib.fibonacci_memoization(n)
    end = time.perf_counter()
    print(f"Result = {dp_val}")
    print(f"Ops = {dp_ops}")
    print(f"Time = {(end - start) * 1000:.6f} ms")

    print(f'\nwritten to "{CSV_FILENAME}".')