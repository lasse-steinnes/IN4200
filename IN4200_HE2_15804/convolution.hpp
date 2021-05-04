// header file for convolution
#ifndef convolution_H
#define convolution_H

#include <stdio.h> // printf
#include <stdlib.h> // atoi
//#include <mpi.h> // MPI

using namespace std;

class ConvFuncsions{

public:

void MPI_double_layer_convolution(int M, int N, float **input, int K1, float **kernel1,
int K2, float **kernel2, float **output);
/*
void allocate_and_initiate(int M, int N,float ***input, int K1, float ***kernel1,
                        int K2, float ***kernel2,float***output);
void double_layer_convolution(int M, int N, float **input, int K1,
  float **kernel1, int K2, float **kernel2, float **output);
void alloc2dfloat(float ***mat, int m, int n);
void free2dfloat(float ***mat);
void mean_squared_error(float ** mat1, float** mat2, int rows, int cols);
void print_matrix(float ** mat, int rows, int cols);
*/

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

void mean_squared_error(float ** mat1, float** mat2, int rows, int cols){
  double error = 0;
  double fac = 1/((double) rows*cols);
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++){
      error += (mat1[i][j] - mat2[i][j])*(mat1[i][j] - mat2[i][j]);
    }
  }
  printf("\n");
  printf("Comparing MPI and serial results... \n");
  printf("Mean squared error: %e \n",fac*error);
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



void print_matrix(float **mat,int rows,int cols){
  printf("\n");
  printf("Resulting matrix \n");
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < cols; j++){
      printf("%f ", mat[i][j]);
      }
    printf("\n");
    }
}


#endif
