// function for MPI version of convolution
#include "convolution.hpp"
#include <mpi.h>

void MPI_double_layer_convolution(int M, int N, float **input, int K1, float **kernel1,
int K2, float **kernel2, float **output){

// Calculate displacements and number of rows for each process.
/*int *n_rows = malloc(numprocs*sizeof *n_rows);


MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
MPI_Comm_size(MPI_COMM_WORLD, &procs); // used to divide workload

// Used when scattering A.
int *send_counts = malloc(numprocs*sizeof *sendcounts);
int *S_displs = malloc(numprocs*sizeof *Sdispls);

// Used when gathering y.
int *Gather_displs = malloc(numprocs*sizeof *Gdispls);


int rows = N/numprocs;
int remainder = N%numprocs;
Sdispls[0] = 0;
Gdispls[0] = 0;

// Last remainder processes gets an extra row.
for (int rank = 0; rank < numprocs-1; rank++) {
    n_rows[rank] = rows + ((rank >= (numprocs - remainder)) ? 1:0);
    sendcounts[rank] = n_rows[rank]*N;
    Sdispls[rank+1] = Sdispls[rank] + sendcounts[rank];
    Gdispls[rank+1] = Gdispls[rank] + n_rows[rank];
}
n_rows[numprocs-1] = rows + ((numprocs-1) >= (numprocs - remainder) ? 1:0);

sendcounts[numprocs-1] = n_rows[numprocs-1]*N;

// Allocate local buffers.
double *A;
double *x = malloc(N * sizeof *x);
if (myrank == 0) {
    A = malloc(N*N * sizeof *A);

    // Initialize to some values:
    for (size_t i = 0; i < N; i++) {
        x[i] = i%4;
        for (size_t j = 0; j < N; j++) {
            A[idx(i,j)] = 0.01*i + 0.01*j;
        }
    }
} else {
    A = malloc(N*n_rows[myrank] * sizeof *A);
}*/

/*
  MPI_Scatterv(A,                 // Sendbuff, matters only for root process.
               sendcounts,
               Sdispls,
               MPI_DOUBLE,
               A,                 // Recieve buff is the same as sendbuf here.
               N*n_rows[myrank],
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
