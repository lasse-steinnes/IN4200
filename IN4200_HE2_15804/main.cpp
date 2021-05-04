#include <stdio.h> // printf
#include <stdlib.h> // atoi
#include <mpi.h> // MPI
#include "convolution.hpp" // MPI convolution function

void allocate_and_initiate(int M, int N,float ***input, int K1, float ***kernel1,
                        int K2, float ***kernel2,float***output);

void double_layer_convolution(int M, int N, float **input, int K1,
  float **kernel1, int K2, float **kernel2, float **output);

void alloc2dfloat(float ***mat, int m, int n);

void free2dfloat(float ***mat);

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
  if (my_rank>0) {
    // allocate memory for  the convolutional kernels in non-root processes
    alloc2dfloat(&kernel1, dims[2], dims[2]);  alloc2dfloat(&kernel2, dims[3], dims[3]);
  }

  // broadcast content of kernels
  MPI_Bcast(&(kernel1[0][0]), dims[2]*dims[2], MPI_FLOAT, root, MPI_COMM_WORLD);

  // broadcast content of kernels
  MPI_Bcast(&(kernel2[0][0]), dims[3]*dims[3], MPI_FLOAT, root, MPI_COMM_WORLD);

  // parallel computation of double layer convolution
  MPI_double_layer_convolution(dims[0], dims[1], input, dims[2], kernel1,
  dims[3], kernel2, output);

  // print the output matrix
  /*if (my_rank == root){
    printf("In main \n");
    for (int i = 0; i < M-K1-K2+2; i++){
      for (int j = 0; j < N-K1-K2+2; j++){
        printf("%f ", output[i][j]);
      }
      printf("\n");
    }

  }*/
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();

  // print the output matrix
  //printf("In main \n");
  //for (int i = 0; i < M-K1-K2+2; i++){
  //  for (int j = 0; j < N-K1-K2+2; j++){
  //    printf("%f ", output[i][j]);
  //  }
  //  printf("\n");
  //}



  free(input);
  free(output);
  return 0;
}

void double_layer_convolution(int M, int N, float **input, int K1,
  float **kernel1, int K2, float **kernel2, float **output){
  int i,j,ii,jj;
  float temp;

  // allocate a temporal_arr
  float **intermediate= (float**) malloc((M-K1+1) * sizeof *intermediate); // dyn. allocate rows
  for (size_t i = 0; i < M-K1+1; i++) { // dyn. allocate cols
      intermediate[i] = (float*) malloc((N-K1+1) * sizeof intermediate[0]);
  }

  // first convolution
  for (i=0; i<=M-K1; i++) {
  for (j=0; j<=N-K1; j++) {
    temp = 0.0f;

    for (ii=0; ii < K1; ii++){
    for (jj=0; jj < K1; jj++){
      temp += input[i+ii][j+jj]*kernel1[ii][jj];
      }
    }
    intermediate[i][j] = temp;
    }
  }

  // second convolution
  for (i=0; i<=M-K1-K2+1; i++) {
  for (j=0; j<=N-K1-K2+1; j++) {
    temp = 0.0f;

    for (ii=0; ii < K2; ii++){
    for (jj=0; jj < K2; jj++){
      temp += intermediate[i+ii][j+jj]*kernel2[ii][jj];
      }
    }
    output[i][j] = temp;
    }
  }

  free(intermediate);
}

void allocate_and_initiate(int M, int N,float ***input, int K1, float ***kernel1,
                        int K2, float ***kernel2,float***output){ // witht deallocation

  // allocate 2D matrices with contiguous memory allocation
  // inputrix to be convoluted
  alloc2dfloat(&(*input), M,N);

  // allocate the output inputrix M-K+1 x N-K+1 or M-K1-K2+2 x N-K1-K2 + 2 (double)
  alloc2dfloat(&(*output),M-K1-K2+2,N-K1-K2+2);

  // kernel1
  alloc2dfloat(&(*kernel1), K1,K1);

  // kernel2
  alloc2dfloat(&(*kernel2), K2,K2);

  // assign variables to the inputrix
  for (int i = 0; i < M; i++){
    for (int j = 0; j < N; j++){
      (*input)[i][j] = 0.0f; //((float)i) +
    }
  }

  // 4 first cols filled with tens for all rows.
  for (int i = 0; i < M; i++){
    for (int j = 0; j < 4; j++){
      (*input)[i][j] = 10.0f;
    }
  }

  // initiate output to zero
  for (int i = 0; i < M - K1 -K2 + 2; i++){
    for (int j = 0; j < N - K1 - K2 + 2; j++){
      (*output)[i][j] = 0.0f;
    }
  }

  // assign variables to the kernels
  for (int i = 0; i < K1; i++){
    for (int j = 0; j < K1; j++){
      if (j == 0){
        (*kernel1)[i][j] = 1.0f;
      }
      else if (j == K1-1){
        (*kernel1)[i][j] = -1.0f;
      } else {
        (*kernel1)[i][j] = 0.0f;
      }
    }
  }

  for (int i = 0; i < K2; i++){
    for (int j = 0; j < K2; j++){
      if (j == 0){
        (*kernel2)[i][j] = 1.0f;
      }
      else if (j == K2-1){
        (*kernel2)[i][j] = -1.0f;
      } else{
        (*kernel2)[i][j] = 0.0f;
      }
    }
  }
}


void alloc2dfloat(float ***mat, int m, int n) {
    /* allocate n*m contiguous slots */
    float *p = (float *)malloc((m*n) * sizeof(float));

    /* allocate the row pointers into the memory */
    (*mat) = (float **)malloc(m * sizeof(float*));

    /* set up the pointers into the contiguous memory */
    for (int i=0; i<m; i++){
       (*mat)[i] = &(p[i*n]);
     }
}

void free2dfloat(float ***mat){
    /* free the memory - the first element  */
    free(&((*mat)[0][0]));

    /* free the pointers in mem */
    free(*mat);
}
