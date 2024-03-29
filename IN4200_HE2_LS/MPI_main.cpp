#include <stdio.h> // printf
#include <stdlib.h> // atoi
#include <mpi.h> // MPI
#include "convolution.hpp" // MPI convolution function
#include "helper_functions.hpp" // helper functions

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

/*                         MAIN PROGRAM                                     */

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/


using namespace std;
int main(int nargs, char **args){

  // obects allocated for all processes
  int M = 0, N = 0, K1 = 0, K2 = 0, my_rank, procs;
  int nvals = 4; int dims[nvals];
  int root = 0; // set process 0 to root by default
  float **input=NULL, **output=NULL, **kernel1=NULL, **kernel2 = NULL;

  MPI_Init (&nargs, &args);
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs); // used to divide workload

  if (my_rank == 0){
  if (nargs < 2){
    M = 8; N = 8;
    K1 = 3; K2 = 3;
  } else{
    M = atoi(args[1]); N = atoi(args[2]);
    K1 = atoi(args[3]); K2 = atoi(args[4]);
    if (K1 > N || K2 > N-K1+1 ){ // || is the same as or
     printf("\n");
     printf("Error: Dimensions not suitable for double convolution! \n");
     printf("Exiting programme \n");
     return 1;
      }
  }

  printf("\n");
  printf("--------------------------------------------- \n");
  printf("Number of rows available for work division: %d \n", (M-K1-K2+2));
  printf("Number of processes available for work: %d \n", procs);
  printf("--------------------------------------------- \n");

  if (procs >= (M-K1-K2+2)/((double) 2)){
    printf("\n");
    printf("OBS! Danger of inefficient work division, due to \n");
    printf("low dimension:processes ratio: %.1f \n\n", (M-K1-K2+2)/((double) procs));
  }

  if (procs > (M-K1-K2+2)){
    printf("\n");
    printf("Input Error! Rows (output matrix) : # processes ratio < 1.\n");
    printf("Choose less processes\n\n");
    return 1;
    }

  // vals M,N,K1,K2;
  dims[0] = M; dims[1] = N;
  dims[2] = K1; dims[3] = K2;

  allocate_and_initiate(M, N, &input, K1, &kernel1, // all initiated and allocated in root
                          K2, &kernel2,&output);
  }

  // broadcast objects to be shared
  MPI_Bcast(&dims, nvals, MPI_INT, root, MPI_COMM_WORLD); // send to each process buffer
                                                          // collective communication

  // handle kernels in non-root processes, and allocate memory for workloads?
  if (my_rank> root) {
    // allocate memory for  the convolutional kernels in non-root processes
    alloc2dfloat(&kernel1, dims[2], dims[2]);  alloc2dfloat(&kernel2, dims[3], dims[3]);
  }

  // broadcast content of kernels
  MPI_Bcast(&(kernel1[0][0]), dims[2]*dims[2], MPI_FLOAT, root, MPI_COMM_WORLD);

  // broadcast content of kernels
  MPI_Bcast(&(kernel2[0][0]), dims[3]*dims[3], MPI_FLOAT, root, MPI_COMM_WORLD);


  double start, end;
  if (my_rank == root){
    printf("--------------------------------------------- \n");
    printf("Performing double layer convolution...\n");
  }
  start = MPI_Wtime();

  // parallel computation of double layer convolution
  MPI_double_layer_convolution(dims[0], dims[1], input, dims[2], kernel1,
  dims[3], kernel2, output);

  end = MPI_Wtime();
  if (my_rank == root){
    printf("Double layer convolution finished! \n");
    }

  // print the output matrix and compare with serial
  if (my_rank == root){
    //print matrix
    printf("--------------------------------------------- \n");
    if (N < 10){
    print_matrix(output,M-K1-K2+2,N-K1-K2+2);
    }

    // compare with serial
    float **output_serial = NULL;
    alloc2dfloat(&output_serial,M-K1-K2+2,N-K1-K2+2);
    double_layer_convolution(M, N, input, K1,
      kernel1, K2, kernel2, output_serial);
    mean_squared_error(output, output_serial,M-K1-K2+2,N-K1-K2+2);
    free2dfloat(&output_serial);
    printf("\n");
    printf("--------------------------------------------- \n");
  }

  // check that input and output matrix is NULL in non-root processes
  // after function MPI double layer convolution is called
  if (my_rank > root){
    if (input != NULL || output != NULL){ // || is the same as or
      printf("Error: Memory leakage in non-root processes! \n");
      return 1;
      }
    }

  // free memory
  free2dfloat(&kernel1);
  free2dfloat(&kernel2);

  // free input and output memory from root
  if (input != NULL & output != NULL){
  free2dfloat(&input);
  free2dfloat(&output);
  };

  MPI_Barrier(MPI_COMM_WORLD); // to get printf together
  printf("Double layer convolution took %.5e ms on process %d \n", (end-start)/((double)1000.0), my_rank);

  MPI_Finalize(); // end paralell region
  return 0;
}
