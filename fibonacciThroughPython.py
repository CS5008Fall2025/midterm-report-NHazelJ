# used to measure how long the function takes to run
import time

# Finding fibonacci number iteratively
# finding Nth fibonacci number through iteration
def fibonacci_iterative(n):
    #Returns both the Fibonacci value and number of operations.
    # if n is 0 or 1 then return n
    if n <= 1:
        return n, 0
    # initializing the first two numbers of Fibonacci sequence
    # F(0)
    a = 0
    # F(1)
    b = 1
    # keep track through count how many additions we do
    # operations - ops
    ops= 0
    # Loop from 2 up to n inclusive
    for i in range(2, n + 1):
        # temp will hold the next Fibonacci number
        temp = a + b
        # One addition operation just happened
        ops += 1
        # Move forward in the sequence
        # The old b becomes the new a
        a = b
        # The temp newly computed number becomes new b
        b = temp
    # when the loop ends b holds F(n)
    # return the final Fibonacci number
    return b, ops


# Recursive function to find Nth Fibonacci number
def fibonacci_recursive(n):
    
    # helper function to count operations
    def helper(k, ops):
        # base case
        if k <= 1:
            return k
        # count the addition operation
        ops[0] += 1
        return helper(k - 1, ops) + helper(k - 2, ops)
    # using a list to hold ops so it can be modified in helper
    ops = [0]
    result = helper(n, ops)
    return result, ops[0]

# Dynamic Programming
# Fibonacci with memoization
def fibonacci_memoization(n, memo=None):
    # initialize memo dictionary on first call
    if memo is None:
        memo = {}
    # to count operations
    ops = [0]
    
    def helper(k):
        # check if the value is already computed
        if k in memo:
            return memo[k]
        # base case
        if k <= 1:
            memo[k] = k
            return k
        # recursive compute F(k-1) + F(k-2)
        left = helper(k - 1)
        right = helper(k - 2)
        # count the addition operation
        ops[0] += 1
        # save in memo and return
        memo[k] = left + right
        return memo[k]
    
    # fibonacci computation
    result = helper(n)
    # return value and operations count
    return result, ops[0]
    