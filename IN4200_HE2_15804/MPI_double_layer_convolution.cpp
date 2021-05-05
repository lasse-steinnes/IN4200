#include "convolution.hpp" // header file
#include <mpi.h> // needed for communicator, scatterv and gatherv (MPI in general)
#include <stdio.h> //printf
#include <stdlib.h> //malloc

void alloc2dfloat_conv(float ***mat, int m, int n){
    /* Allocates a 2D matrix contiguosly in memory

    Input:
    - mat: matrix for which memory is to be allocated
    - m: rows
    - n: columns
    */


    float *p = (float *)malloc((m*n) * sizeof(float));
    (*mat) = (float **)malloc(m * sizeof(float*));
    /* set up the pointers into the contiguous memory */
    for (int i=0; i<m; i++){
       (*mat)[i] = &(p[i*n]);
     }
}

void divide_work(int **out_send_counts, int **rows, int **send_counts, int **send_disp, int **gather_disp,
                  int **subrowsB_arr, int K1, int K2, int M, int N, int procs, int my_rank){
  /* Divides the work for parallel computation, arrays needed for scatterv and gatterv

  Input:
  - out_send_counts: array with number of elements to be gathered
                    to output matrix
  - rows: array with number of rows to be sent from input matrix
  - send_counts: array with number counter of elements to
                  be sent from input matrix
  - send_disp: array with indices from where to start sending elements
                  from input matrix
  - gather_disp: array with how displacement is in gathering
                  output matrix
  - subrowsB_arr: number of subrows in the intermediate matrix.
                  Needed for memory allocation

  - K1: Dim of kernel1 (K1xK1)
  - K2: Dim of kernel2 (K2xK2)
  - M: Number of rows in input matrix.
  - N: Number of columns in input matrix.
  - procs: total number of processes
  - my_rank: rank of process (in range 0-procs-1)
            (given simply for print purposes)
  */

  // -----------------------------------------
   //     allocate memory and objects
  // -----------------------------------------
  // array for  number of rows for each process.
  *rows = (int*) malloc(procs * sizeof (*rows));
  // array for how large each memory segment is (number of elements)
  *send_counts = (int*) malloc(procs * sizeof (*send_counts));

  // array for where to start accessing these elements
  *send_disp = (int*) malloc(procs * sizeof (*send_disp));

  // array for where to start accesing gathering output (new dims remember)
  // need to multiply with number of cols in the output matrix
  *gather_disp = (int*) malloc(procs*sizeof (*gather_disp));

  // array for number of elements in gathering output
  *out_send_counts = (int*) malloc(procs*sizeof *out_send_counts);

  // array for number of rows in intermediate B matrix
  *subrowsB_arr = (int*) malloc(procs*sizeof *subrowsB_arr);

  int cols_out = N-K1-K2+2;
  int out_rows = M - K1 - K2 + 2;
  int sub_rows_out = out_rows/procs;
  int remaind = out_rows%procs;

  //printf("out_rows: %d, remaind: %d, rank: %d procs: %d \n", out_rows, remaind, my_rank, procs);

  // -----------------------------------------
  //          Divide Work
  //-----------------------------------------

  (*send_disp[0]) = 0;
  (*gather_disp[0]) = 0;
  // divide work by filling in arrays to be send to scatterv and gatherv
  // Last remainder processes gets an extra row.
  int tot_rows = 0,sub_rows_B = 0, check_remaind = 0;
  for (int rankid = 0; rankid < procs-1; rankid++){
    check_remaind = ((rankid >= (procs - remaind)) ? 1:0);
    sub_rows_B = K2 + 1*(sub_rows_out + check_remaind - 1);
    (*rows)[rankid] =  K1 + 1*(sub_rows_B-1);        // number of rows to be sent
    (*subrowsB_arr)[rankid] = sub_rows_B;             // number of rows in intermediate matrix
    (*send_counts)[rankid] = (*rows)[rankid]*N;     //send numbercounter of elements
    tot_rows += sub_rows_out + ((rankid >= (procs - remaind)) ? 1:0); // counter how to distr
    (*send_disp)[rankid+1] = tot_rows*N;            // send the index to start
    (*gather_disp)[rankid+1] = ((*gather_disp)[rankid] + sub_rows_out  // how displacement is in gather
                                +  ((rankid >= (procs - remaind)) ? 1:0))*cols_out; // while gathering output results
    (*out_send_counts)[rankid] = (sub_rows_out                       // number of elements to be gathered
                                 + ((rankid >= (procs - remaind)) ? 1:0))*(cols_out); // to outputmatrix
  }

  // fix the last ones
  check_remaind =  ((procs-1 >= (procs - remaind)) ? 1:0);
  sub_rows_B = K2 + 1*(sub_rows_out + check_remaind - 1);
  (*subrowsB_arr)[procs-1] = sub_rows_B;
  (*rows)[procs-1] =  K1 + 1*(sub_rows_B-1);

  (*send_counts)[procs-1] = (*rows)[procs-1]*N;
  (*out_send_counts)[procs-1] = (sub_rows_out                      // number of elements to be gathered
                            + ((procs-1 >= (procs - remaind)) ? 1:0))*(cols_out); // to outputmatrix

  /*if (my_rank == 0){
    for (int rankid = 0; rankid < procs; rankid++){
      printf("%d\n", rankid);
      printf("rows A %d\n",(*rows)[rankid]);
      printf("subrows B %d\n",(*subrowsB_arr)[rankid]);
      printf("send counts %d\n",(*send_counts)[rankid]);
      printf("send disp %d\n",(*send_disp)[rankid]);
      printf("gather disp %d\n",(*gather_disp)[rankid]);
    }
  }*/

}


void MPI_double_layer_convolution(int M, int N, float **input, int K1,
  float **kernel1, int K2, float **kernel2, float **output){
    /* Parallel computation of double layer convolution

    Input:
    - M: Number of rows in input matrix.
    - N: Number of columns in input matrix.
    - input: 2D input matrix.
    - K1: Dim of kernel1 (K1xK1)
    - kernel1: 2D kernel matrix for first convolution
    - K2: Dim of kernel2 (K2xK2)
    - kernel2: 2D kernel matrix for second convolution
    - output: Output matrix after two convolutions
    */

  int procs, my_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &procs); // get number of processes
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank); // get ID of current process

  // Declare as NULL to avoid unexpected behavior
  int *out_send_counts = NULL,*rows = NULL,
      *send_counts = NULL, *send_disp = NULL,
      *gather_disp = NULL, *sub_rows_B = NULL;

  // only divide work once
  divide_work(&out_send_counts,&rows, &send_counts, &send_disp, &gather_disp,
              &sub_rows_B,K1, K2, M, N, procs,my_rank);

  if (my_rank > 0){ // allocate input and output
    //allocate input
    alloc2dfloat_conv(&input,rows[my_rank],N);

    // allocate output matrix
    int cols_out = N-K1-K2+2;
    int rows_out = out_send_counts[my_rank]/((int)cols_out);
    alloc2dfloat_conv(&output,rows_out,cols_out);
  }

  // allocate intermediate matrix on all processes
  int cols_B = N-K1+1;
  float **matB = NULL;
  alloc2dfloat_conv(&matB, sub_rows_B[my_rank], cols_B);

  // Scatter input matrix to local processes
  MPI_Scatterv(input[0],
              send_counts,
              send_disp,
              MPI_FLOAT,
              input[0],
              N*rows[my_rank],
              MPI_FLOAT,
              0,
              MPI_COMM_WORLD);

// -----------------------------------------
//          Do Local Calculations
//-----------------------------------------

  // first convolution
  float temp;
  int i, j, ii, jj;
  for (i=0; i < sub_rows_B[my_rank]; i++){ // changed to match rank
  for (j=0; j < cols_B; j++) { // all columns in matB
    temp = 0.0f;

    for (ii=0; ii < K1; ii++){
    for (jj=0; jj < K1; jj++){
      temp += input[i+ii][j+jj]*kernel1[ii][jj];
      }
    }
    (matB)[i][j] = temp;
    }
  }

  // second convolution
  int cols_out = N-K1-K2+2;
  int rows_out = out_send_counts[my_rank]/((int)cols_out);
  for (i=0; i< rows_out ; i++){ // changed to match rank
  for (j=0; j< cols_out; j++) { // all columns in output
    temp = 0.0f;

    for (ii=0; ii < K2; ii++){
    for (jj=0;jj < K2;jj++){
      temp += matB[i+ii][j+jj]*kernel2[ii][jj];
      }
    }
    output[i][j] = temp;
    }
  }

//-----------------------------------------

  // free the memory
  free(&(matB[0][0]));
  free(matB);

  // Gather the results
    MPI_Gatherv(output[0],
                out_send_counts[my_rank],
                MPI_FLOAT,
                output[0],
                out_send_counts,
                gather_disp,
                MPI_FLOAT,
                0,
                MPI_COMM_WORLD);
}
