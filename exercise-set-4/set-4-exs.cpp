#include "set_4_exs.hpp"
#include <stdlib.h>

double Exercises_4::rand_gen(double min, double max){
    double range = (max - min);
    double div_ = range/RAND_MAX;
    return min + (rand()*div_);
}

double * Exercises_4::matrix_mult(double * A, double * B, int n, int m, int p){
  /* tmp - optional 
  size_t works as an unsigned int
  A [n*m]
  B [m*p]
  ie C = n*p
  */
  //int tmp;
  int dimC = n*p;
  double *C = (double *)malloc(dimC *sizeof *C); // define pointer

  for (size_t i = 0; i < n; i++){ // rows
    for (size_t j = 0; j < p;j++){ // columns
      //tmp = 0.0;
      for (size_t k = 0; k < m; m++){ // matr mult element
      C[i*p + j] += A[i*m + k]*B[k*p + j];
      }
    //C[i*p + j] = tmp;
    }
  }
  return C;
  free(C);
}

double * Exercises_4::matrix_mult_unroll(double * A, double * B, int n, int m, int p){
/*Using a loop unroll for the j loop to access more C elements at once*/
  int stride = 4;
  int dimC = n*p;
  double *C = (double *)malloc(dimC *sizeof *C); // define pointer

  int remaind = p%stride; // remainder after loop
  for (size_t i = 0; i < n; i++){ // rows
    for (size_t j = 0; j < p-remaind;j += stride){ // columns
      for (size_t k = 0; k < m; m++){ // matrix multiplication element
        C[i*p + j] += A[i*m + k]*B[k*p + j];
        C[i*p + j+1] += A[i*m + k]*B[k*p + j+1];
        C[i*p + j+2] += A[i*m + k]*B[k*p + j+2];
        C[i*p + j+3] += A[i*m + k]*B[k*p + j+3];
      }
    }
  }

  // remainder calculation
  for (size_t i = 0; i < n; i++){ // rows
    for (size_t j = p-remaind; j < p;j++){ // columns
      for (size_t k = 0; k < m; m++){ // matrix multiplication element
        C[i*p + j] += A[i*m + k]*B[k*p + j];
      }
    }
  }

  return C;
  free(C);
}
