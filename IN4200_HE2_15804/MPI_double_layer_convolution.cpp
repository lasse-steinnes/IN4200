// function for MPI version of convolution
#include "convolution.hpp"
#include <mpi.h>
#include <stdio.h> //printf
#include <stdlib.h> //malloc

void divide_work(int **out_send_counts, int **rows, int **send_counts, int **send_disp, int **gather_disp,
                  int K1, int K2, int M, int N, int procs, int my_rank){

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


int intermediate_rows = M - K1 + 1; // number of rows in intermediate matrix
int intermediate_sub_rows = intermediate_rows/procs; // integer division
int remaind = intermediate_rows%procs; // with remainder
int sub_rows = K1 + 1*(intermediate_sub_rows-1); // sub rows in input array

// to be used to gathering the final result
int output_rows = M - K1 - K2 + 2;
int output_sub_rows = output_rows/procs;
int remaind2 = output_rows%procs;


(*send_disp[0]) = 0;
(*gather_disp[0]) = 0;

printf("sub_rows: %d, remaind: %d, rank: %d \n", sub_rows, remaind, my_rank);

// divide work by filling in arrays to be send to scatterv and gatherv
// Last remainder processes gets an extra row.
//int placeholder1, placeholder2;
// gatherdisp must be suited to output
for (int rankid = 0; rankid < procs-1; rankid++){

  (*rows)[rankid] = sub_rows + ((rankid >= (procs - remaind)) ? 1:0); // number of rows to be sent
  (*send_counts[rankid]) = (*rows)[rankid]*N; //send numbercounter of elements
  (*send_disp)[rankid+1] = (*send_disp)[rankid] + (*rows)[rankid]*N; // send the index to start
  (*gather_disp)[rankid+1] = (*gather_disp)[rankid] + output_sub_rows  // how displacement is in gather
                              +  ((rankid >= (procs - remaind2)) ? 1:0); // while gathering output results
  (*out_send_counts)[rankid] = (output_sub_rows                        // number of elements to be gathered
                               + ((rankid >= (procs - remaind2)) ? 1:0))*(N-K1-K2+2); // to outputmatrix
}

// fix the last ones
(*rows)[procs-1] = sub_rows + ((procs-1) >= (procs - remaind) ? 1:0);
(*send_counts)[procs-1] = (*rows)[procs-1]*N;
(*out_send_counts)[procs-1] = (output_sub_rows                        // number of elements to be gathered
                          + ((procs-1 >= (procs - remaind2)) ? 1:0))*(N-K1-K2+2); // to outputmatrix
}

void MPI_double_layer_convolution(int M, int N, float **input, int K1,
  float **kernel1, int K2, float **kernel2, float **output){

int procs, my_rank;
MPI_Comm_size(MPI_COMM_WORLD, &procs); // get number of processes
MPI_Comm_rank (MPI_COMM_WORLD, &my_rank); // get ID of current process

int *out_send_counts = NULL,*rows = NULL,
    *send_counts = NULL, *send_disp = NULL,
    *gather_disp = NULL;

// only divide work once
divide_work(&out_send_counts,&rows, &send_counts, &send_disp, &gather_disp,
            K1, K2, M, N, procs,my_rank);

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
printf("rank %d rows[my_rank] %d  N %d \n", my_rank, rows[my_rank], N);
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
