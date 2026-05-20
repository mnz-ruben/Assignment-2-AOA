# CMPS 400: Analysis of Algorithms - Matrix Workbench

## Problem 1: Matrix Workbench

This program implements a menu-driven Matrix Workbench in C++. It allows the user to create Matrix A and Matrix B, display the current matrices, check multiplication compatibility, perform matrix multiplication, multiply a matrix by a scalar, and restore matrices back to their original values.

The program stores each matrix in two arrays:

- `values[][]`: the current working copy of the matrix
- `original[][]`: the original copy saved when the matrix is first created

Each matrix also stores its own name, ID, number of rows, number of columns, and whether it has been created.

### Main Features

1. Create Matrix A
2. Create Matrix B
3. Display current matrices
4. Check multiplication compatibility
5. Perform A x B
6. Perform B x A
7. Multiply a matrix by a scalar
8. Restore original matrices
0. Exit

### How to Compile and Run

```bash
g++ main.cpp -o matrix_workbench
./matrix_workbench
```

## Problem 2: Determine the Order of Growth

### 1.

```cpp
for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
        cout << A[i][j] << " ";
    }
}
```

**Time Complexity: O(r x c)**

The outer loop runs `r` times and the inner loop runs `c` times for each row. Since every element of the matrix is printed once, the total number of operations grows with the number of rows multiplied by the number of columns.

### 2.

```cpp
for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        C[i][j] = A[i][j] + B[i][j];
    }
}
```

**Time Complexity: O(n^2)**

There are two nested loops, and each loop runs `n` times. The statement inside the inner loop performs a constant-time addition and assignment, so the total work is `n x n`, which gives O(n^2).

### 3.

```cpp
for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        int sum = 0;
        for (int k = 0; k < n; k++) {
            sum += A[i][k] * B[k][j];
        }
        C[i][j] = sum;
    }
}
```

**Time Complexity: O(n^3)**

This code has three nested loops, and each loop runs `n` times. The innermost loop performs the multiplication and addition used in standard matrix multiplication, so the total number of operations grows as `n x n x n`, which is O(n^3).

### 4.

```cpp
int i = 1;
while (i < n) {
    i *= 2;
}
```

**Time Complexity: O(log n)**

The variable `i` doubles during each iteration. Because the value grows exponentially, the loop only needs about `log2(n)` iterations before `i` reaches or passes `n`. Therefore, the time complexity is O(log n).

### 5.

```cpp
for (int mask = 0; mask < (1 << n); mask++) {
    for (int j = 0; j < n; j++) {
        if (mask & (1 << j)) {
            cout << arr[j] << " ";
        }
    }
}
```

**Time Complexity: O(n x 2^n)**

The outer loop runs `2^n` times because `(1 << n)` represents all possible subsets of `n` elements. For every value of `mask`, the inner loop runs `n` times to check each element. Therefore, the total time complexity is O(n x 2^n).
