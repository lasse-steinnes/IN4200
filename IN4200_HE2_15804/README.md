# IN4200 - HE2
Git repository for Home Exam 2 in the course High Performance Computing (IN4200) at UiO.

### Main Overview
* The programs in this repository aim at performing a double convolution of a given input matrix. Convolution is a often used in machine learning. More on that [here](https://anhreynolds.com/blogs/cnn.html). Also, see the problem description (PDF).

* The main challenge is to perform a double convolution with parallel processes and work division using MPI. MPI is here accesses through Open MPI - A High Performance Message Passing Library.

* The dimensions of the input matrix (let's call it A) is M x N (rows x columns), so that if kernel1 is a K1 x K1 matrix, single_convolution(A,K1) results in a matrix B ((M-K1+1) x (N-K1+1)). If kernel2 is a K2 x K2 matrix, then single_convolution(B,K2) results in an output matrix with dimensions (M-K1-K2+2) x (N-K1-K2+2). Thus a function double_convolution(A,K1,B,K2) should give the same output matrix ((M-K1-K2+2) x (N-K1-K2+2)) as the two successive single convolutions described above.

* In the code, work division occurs by rows for simplicity, and not by having a cartesian grid of processors/workloads (a possibility through ```MPI_Cartesian()```). Work division is based upon the dimensions of the output matrix with M-K1-K2+2 number of rows, which if divisible by number of processes, will give an equal work division between processes. If not, an extra row is given to the last process. This work division is chosen to only scatter the input and gather output once, and avoiding any sends' and receives' in between ```MPI_Scatterv``` and ```MPI_Gatherv```. Communication directives produces overhead. Thus, given a short overlap in rows required for the convolution computation between processes, this strategy should be both the easier and more efficient option.

* The work division chosen, allows for overlapping regions (rows) in the intermediate matrix B, and the input matrix (A), between processes. Now, assume out_subrows are the number of rows given to an arbitrary process. Notice that the row elements with indices (e_r,:) in the resulting matrix (out) after convolution always are a result of K+1*(out_subrows-1) rows from the input matrix, with starting row index e_r in input matrix.  Thus, if the work division is performed for an output matrix from a double convolution, then  in the convolution conv(B,K2), there is B_subrows = K2+1*(out_subrows-1) rows from B needed to calculate row e_r in the output matrix. In the first convolution then, the number of rows needed from A to calculate B, is K1+1*(B_subrows-1) rows.

* The work division decides how the input matrix is scattered, and how the output matrix is gathered. Since MPI_Scatterv needs contiguous memory allocation, the 2D matrices are allocated contiguously. Thus, when scattering the input matrix, one must not send the row displacement, but the element displacement. Thus, the displacement in rows are multiplied with the correct number of columns.


### Code: Description of Programs
- ```main.cpp```: Runs the other programs and provide user options through terminal.

- ```makefile```: Compiles cpp/c files with optimization flag -Ofast (compiler mpiCC).

-  ```convolution.hpp```: Headerfile for the MPI_double_layer_convolution function.

- ```helper_functions.hpp```: Provides helper functions to be used in main.cpp.
    1. ```alloc2dfloat```: Allocates a 2D matrix contiguously in memory.
    2. ```free2dfloat```: Free the contiguously allocated 2D matrix.
    3. ```mean_squared_error```: Finds the means squared error between matrix elements of two matrices.
    4. ```double_layer_convolution```: Serial code solution of the double convolution.
    5. ```allocate_and_initiate```: Allocates and initiates input matrix and kernels.
    6. ```print_matrix```: Prints a 2D matrix.

- ```MPI_double_layer_convolution.cpp```: The parallel version of double convolution.


### Compilation and Execution

The files can be compiled with ```make all``` in terminal. In the directory of the executable, the program can be run either by
```
mpirun -n [number of MPI processes] ./main.out
```
where the dimensions of the input matrix (MxN) = (8x8) and kernels (K1xK1), (K2xK2) = (3x3) are predetermined. Another option is user provided dimensions

```
mpirun -n [number of MPI processes] ./main.out [M] [N] [K1] [K2]
```

The programs were compiled and executed on macOS Catalina (v. 10.15.7). Compilation relies on mpiCC, to access Open MPI (v4.1.1).

### Links and Packages

- Documentation on parallelization with Open MPI can be found [here](https://www.open-mpi.org/doc/). The version applied to test the code was  v4.1.1. Compatibility issues may arise if another version is used.
