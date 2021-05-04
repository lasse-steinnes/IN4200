// function for MPI version of convolution
#include "convolution.hpp"
#include <mpi.h>
#include <stdio.h> //printf
#include <stdlib.h> //malloc

void divide_work(int **out_send_counts, int **rows, int **send_counts, int **send_disp, int **gather_disp,
                  int **subrowsB_arr, int K1, int K2, int M, int N, int procs, int my_rank){

// divide work //
//allocate arrays
// array for  number of rows for each process.
*rows = (int*) malloc(procs * sizeof (*rows));
// array for how large each memory segment is (number of elements)
*send_counts = (int*) malloc(procs * sizeof (*send_counts));

// array for where to start accessing these elements
*send_disp = (int*) malloc(procs * sizeof (*send_disp));

// array for where to start accesing gathering output (new dims remember)
*gather_disp = (int*) malloc(procs*sizeof (*gather_disp));

// array for number of elements in gathering output
*out_send_counts = (int*) malloc(procs*sizeof *out_send_counts);

// array for number of rows in intermediate B matrix
*subrowsB_arr = (int*) malloc(procs*sizeof *subrowsB_arr);

//int rowsA = M;
//int rowsB = M - K1 + 1;
int cols_out = N-K1-K2+2;

int out_rows = M - K1 - K2 + 2;
int sub_rows_out = out_rows/procs;
int remaind = out_rows%procs;

int  sub_B =  K1 + 1*(sub_rows_out-1); // number of rows in intermediate matrix
//int  sub_rows_input = K2 + 1*(sub_rows_B-1); // sub rows in input array

(*send_disp[0]) = 0;
(*gather_disp[0]) = 0;

printf("sub_rows: %d, remaind: %d, rank: %d \n", sub_rows_out, remaind, my_rank);

// divide work by filling in arrays to be send to scatterv and gatherv
// Last remainder processes gets an extra row.
//int placeholder1, placeholder2;
// gatherdisp must be suited to output
int tot_rows = 0,sub_rows_B = 0;
for (int rankid = 0; rankid < procs-1; rankid++){
  sub_rows_B = sub_B + ((rankid >= (procs - remaind)) ? 1:0);
  (*rows)[rankid] =  K2 + 1*(sub_rows_B-1);        // number of rows to be sent
  (*subrowsB_arr)[rankid] = sub_rows_B;             // number of rows in intermediate matrix
  (*send_counts[rankid]) = (*rows)[rankid]*N;     //send numbercounter of elements
  tot_rows += sub_rows_out + ((rankid >= (procs - remaind)) ? 1:0); // counter how to distr
  (*send_disp)[rankid+1] = tot_rows*N;            // send the index to start
  (*gather_disp)[rankid+1] = (*gather_disp)[rankid] + sub_rows_out  // how displacement is in gather
                              +  ((rankid >= (procs - remaind)) ? 1:0); // while gathering output results
  (*out_send_counts)[rankid] = (sub_rows_out                       // number of elements to be gathered
                               + ((rankid >= (procs - remaind)) ? 1:0))*(cols_out); // to outputmatrix
}

// fix the last ones
sub_rows_B = sub_B + ((procs-1 >= (procs - remaind)) ? 1:0);
(*rows)[procs-1] =  K2 + 1*(sub_rows_B-1)
                  + ((procs-1) >= (procs - remaind) ? 1:0);
(*send_counts)[procs-1] = (*rows)[procs-1]*N;
(*out_send_counts)[procs-1] = (sub_rows_out                      // number of elements to be gathered
                          + ((procs-1 >= (procs - remaind)) ? 1:0))*(cols_out); // to outputmatrix
}

void MPI_double_layer_convolution(int M, int N, float **input, int K1,
  float **kernel1, int K2, float **kernel2, float **output){

int procs, my_rank;
MPI_Comm_size(MPI_COMM_WORLD, &procs); // get number of processes
MPI_Comm_rank (MPI_COMM_WORLD, &my_rank); // get ID of current process

int *out_send_counts = NULL,*rows = NULL,
    *send_counts = NULL, *send_disp = NULL,
    *gather_disp = NULL, *sub_rows_B = NULL;

// only divide work once
divide_work(&out_send_counts,&rows, &send_counts, &send_disp, &gather_disp,
            &sub_rows_B,K1, K2, M, N, procs,my_rank);

// allocate local buffers and scatterv (input and output)
float *intermediate;

if (my_rank > 0){ // allocate input and output
  //allocate input
  float *p = (float *)malloc((rows[my_rank]*N)*sizeof(*p));
  input = (float **)malloc((rows[my_rank])*sizeof(float*));
  /* set up the pointers into the contiguous memory */
  for (int i=0; i < rows[my_rank]; i++){
     input[i] = &(p[i*N]);
   }
}

// allocate a matrix to hold
//1) intermediate result
  // which should hold 1 (or more?) extra row to be received

//2) output matrix


/*MPI_Scatterv(A,                 // Sendbuff, matters only for root process.
             sendcounts,
             Sdispls,
             MPI_DOUBLE,
             A,                 // Recieve buff is the same as sendbuf here.
             N*n_rows[myrank],
             MPI_DOUBLE,
             0,
             MPI_COMM_WORLD);*/

MPI_Scatterv(input[0],
            send_counts,
            send_disp,
            MPI_FLOAT,
            input[0],
            N*rows[my_rank],
            MPI_FLOAT,
            0,
            MPI_COMM_WORLD);

// what is senddisp?
for (int i = 0; i < procs; i++){
  printf("send counts: %d senddisp: %d ", send_counts[i],send_disp[i]);
}


// printing the scattered array
printf("rank %d rows[my_rank] %d  N %d \n", my_rank, rows[my_rank], N);
for (int i = 0; i < rows[my_rank]; i++){
  for (int j = 0; j < N; j++){
    printf("%f ", input[i][j]);
  }
  printf("\n");
}

//int MPI_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs,
//                 MPI_Datatype sendtype, void *recvbuf, int recvcount,
//                 MPI_Datatype recvtype, int root, MPI_Comm comm)
// scatter


/*
// Allocate local buffers.
double *A;
double *x = malloc(N * sizeof *x);
} else (not root) {
    A = malloc(M*rows[myrank] * sizeof *A);
}*/

/*
  MPI_Scatterv(A,                 // Sendbuff, matters only for root process.
               sendcounts,
               Sdispls,
               MPI_DOUBLE,
               A,                 // Recieve buff is the same as sendbuf here.
               M*rows[myrank],
               MPI_DOUBLE,
               0,
               MPI_COMM_WORLD);
*/



/*    // Gather the results
    MPI_Gatherv(y,
                n_rows[myrank],
                MPI_DOUBLE,
                y,              // Matters only at root,
                n_rows,
                Gdispls,
                MPI_DOUBLE,
                0,
                MPI_COMM_WORLD);
*/
}
