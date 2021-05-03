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


int output_rows = M - K1 +1; //- K2 + 2 ; // number of rows in output matrix
int output_sub_rows = output_rows/procs; // integer division
int remaind = output_rows%procs; // with remainder
int sub_rows = 3 + 1*(output_sub_rows-1); // sub rows in input array

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
  (*send_disp)[rankid+1] = (*send_disp)[rankid] + (*rows)[rankid]; // send the index to start
  (*gather_disp)[rankid+1] = (*gather_disp)[rankid] + output_sub_rows  // how displacement is in gather
                              +  ((rankid >= (procs - remaind)) ? 1:0); // while gathering output results
  (*out_send_counts)[rankid] = (output_sub_rows                        // number of elements to be gathered
                                + ((rankid >= (procs - remaind)) ? 1:0))*K1; // to outputmatrix
}

// fix the last ones
(*rows)[procs-1] = sub_rows + ((procs-1) >= (procs - remaind) ? 1:0);
(*send_counts)[procs-1] = (*rows)[procs-1]*N;
(*out_send_counts)[procs-1] = (output_sub_rows                        // number of elements to be gathered
                          + ((procs-1 >= (procs - remaind)) ? 1:0))*K1; // to outputmatrix
}

void MPI_double_layer_convolution(int M, int N, float **input, int K1,
  float **kernel1, int K2, float **kernel2, float **output){

int procs, my_rank;
MPI_Comm_size(MPI_COMM_WORLD, &procs); // get number of processes
MPI_Comm_rank (MPI_COMM_WORLD, &my_rank); // get ID of current process

int *out_send_counts = NULL,*rows = NULL,
    *send_counts = NULL, *send_disp = NULL,
    *gather_disp = NULL;

divide_work(&out_send_counts,&rows, &send_counts, &send_disp, &gather_disp,
            K1, K2, M, N, procs,my_rank);

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
