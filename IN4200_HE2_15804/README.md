# IN4200 - HE2
Git repository for Home Exam 2 in the course High Performance Computing (IN4200) at UiO.

### Main overview
* The programs in this repository aim at performing a double convolution of a given input matrix. Convolution is a often used in machine learning. More on that [here](https://anhreynolds.com/blogs/cnn.html). Also, see the problem description (PDF).

* The main challenge is to perform a double convolution with parallel processes and work division using MPI. MPI is here accesses through Open MPI - A High Performance Message Passing Library.

* The dimensions of the input matrix (let's call it A) is M x N (rows x columns), so that if kernel1 is a K1 x K1 matrix, single_convolution(A,K1) results in a matrix B ((M-K1+1) x (N-K1+1)). If kernel2 is a K2 x K2 matrix, then single_convolution(B,K2) results in an output matrix with dimensions (M-K1-K2+2) x (N-K1-K2+2). Thus a function double_convolution(A,K1,B,K2) should give the same output matrix ((M-K1-K2+2) x (N-K1-K2+2)) as the two successive single convolutions described above.

* In the code, work division occurs by rows for simplicity, and not by having a cartesian grid of processors/workloads (a possibility through ```MPI_Cartesian()```). Work division is based upon the dimensions of the output matrix with M-K1-K2+2 number of rows, which if divisible by number of processes, will give an equal work division between processes. If not, an extra row is given to the last process. This work division is chosen to only scatter the input and gather output once, and avoiding any sends' and receives' in between ```MPI_Scatterv``` and ```MPI_Gatherv```.

* The work division chosen, allows for overlapping regions (rows) in the intermediate matrix B, and the input matrix, between processes. Now, assume out_subrows are the number of rows given to an arbitrary process. Notice that the row elements with indices (e_r,:) in the resulting matrix (out) after convolution always are a result of K+1*(out_subrows-1) rows from the input matrix, with starting row index e_r in input matrix.  Thus, if the work division is performed for an output matrix from a double convolution, then  in the convolution conv(B,K2), there is B_subrows = K2+1*(out_subrows-1) rows from B needed to calculate row e_r in the output matrix. In the first convolution then, the number of rows needed from A to calculate B, is K1+1*(B_subrows-1) rows.

* The work division decides how the input matrix is scattered, and how the output matrix is gathered. Since MPI_Scatterv needs contiguous memory allocation, the 2D matrices are allocated contiguously. When scattered, one must not send the row displacement, but the element displacement. Thus, the displacement in rows are multiplied with the correct number of columns.


### Code: Description of programmes
- ```main.cpp```: Runs the other programmes and provide user options through terminal.

- ```makefile```: Compiles and executes cpp files with optimization flags.

-  ```convolution.hpp```: Headerfile for the class Shared_NN, listing all functions relevant for HE1.

- ```helper_functions.hpp```: Provides the 2D method for reading a chosen textfile. It   allocates memory for table2D, to be used in creating SNN_table.

- ```create_SNN_graph1.cpp```: Creates an SNN grid, which shows how many SNNs an edged node-pair has. The SNN graph is stored as a 2D matrix.


- ```read_graph_from_file2.cpp```: Programme which reads a chosen textfile, and stores the information of edged node-pairs in 1D CRS format.

- ```create_SNN_graph2.cpp```: Creates a 1D compact SNN_val array, which can be accessed through row_ptr and col_idx (from CRS format connectivity storage).
- ```check_node.cpp```: Clusters edged node-pairs with number of SNNs more or equal to a threshold value. Has a given node ID as search key, and tau as threshold.


The files can be compiled with ```make all``` in terminal. Number of threads can be exported through the terminal:

```
  export OMP_NUM_THREADS=<number of threads to use>
```

How to run the programmes: The menu provides user input for which method to use. It also asks the user to provide a search key (node ID) and a cluster threshold tau. All tasks is run though main after compiling: ./main.out

Note that the candidate had trouble accessing [login.ifi.uio.no](login.ifi.uio.no). The programmes were compiled and executed on macOS Catalina (v. 10.15.7). Compilation relies on mpiCC, to access Open MPI.

### Links and packages

- Documentation on parallelization with OpenMP can be found [here](https://www.openmp.org/wp-content/uploads/OpenMP-4.5-1115-CPP-web.pdf) or for more versions [here](https://www.openmp.org/resources/refguides/)

- Documentation on fgets [here](https://www.cplusplus.com/reference/cstdio/fgets/) and sscanf [here](https://www.cplusplus.com/reference/cstdio/sscanf/?kw=sscanf).
