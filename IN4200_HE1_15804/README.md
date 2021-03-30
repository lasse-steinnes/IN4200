# IN4200 - HE1
Git repository for Home Exam 1 in the course High Performance Computing (IN4200) at UiO.

### Main overview
* The programs in this repository aim at reading a textfile in ASCII format, to create a connectivity graph and perform cluster analysis.

* The main challenge is to read and store information in either a 2D format, or a compressed row storage (CRS) 1D format. From the stored information, shared nearest neighbours (SNN) between nodes are found for both the 2D and 1D case. In addition, a cluster analysis is performed from the 1D CRS case.

* Textfile examples to be read, can be found in the folder "data".

### Code: Description of programmes
- ```main.cpp```: Runs the other programmes and provide user options through terminal.

- ```makefile```: Compiles and executes cpp files with optimization flags.

-  ```shared_nn.hpp```: Headerfile for the class Shared_NN, listing all functions relevant for HE1.

- ```read_graph_from_file1.cpp```: Provides the 2D method for reading a chosen textfile. It   allocates memory for table2D, to be used in creating SNN_table.

- ```create_SNN_graph1.cpp```: Creates an SNN grid, which shows how many SNNs an edged node-pair has. The SNN graph is stored as a 2D matrix.


- ```read_graph_from_file2.cpp```: Programme which reads a chosen textfile, and stores the information of edged node-pairs in 1D CRS format.

- ```create_SNN_graph2.cpp```: Creates a 1D compact SNN_val array, which can be accessed through row_ptr and col_idx (from CRS format connectivity storage).
- ```check_node.cpp```: Clusters edged node-pairs with number of SNNs more or equal to a threshold value. Has a given node ID as search key, and tau as threshold.


The files can be compiled with ```make all``` in terminal. Number of threads can be exported through the terminal:

```
  export OMP_NUM_THREADS=<number of threads to use>
```

How to run the programmes: The menu provides user input for which method to use. It also asks the user to provide a search key (node ID) and a cluster threshold tau. All tasks is run though main after compiling: ./main.out

Note that the candidate had trouble accessing [login.ifi.uio.no](login.ifi.uio.no). The programmes were compiled and executed on macOS Catalina (v. 10.15.7). 

### Links and packages

- Documentation on parallelization with OpenMP can be found [here](https://www.openmp.org/wp-content/uploads/OpenMP-4.5-1115-CPP-web.pdf) or for more versions [here](https://www.openmp.org/resources/refguides/)

- Documentation on fgets [here](https://www.cplusplus.com/reference/cstdio/fgets/) and sscanf [here](https://www.cplusplus.com/reference/cstdio/sscanf/?kw=sscanf).
