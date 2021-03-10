#include "set7.hpp"
#include <omp.h>
#include <iostream>

// #define idx(i,j) (i*n + j) <--- possibility

using namespace std;

void SET7_EXS::dense_mat_vec(int m, int n, double *x, double *A, double *y) {
  int i, j;
  for (i=0; i<m; i++){
    double tmp = 0.;
     for (j=0; j<n; j++){
      tmp += A[i*n+j]*y[j];
      }
    x[i] = tmp;
  }
}

void SET7_EXS::dense_mat_vec_parallel(int m, int n, double *x, double *A, double *y){
  int i, j;
  #ifdef _OPENMP
  #pragma omp parallel for private(i) // parallelize loop (avoid extra parallel regions)
  #endif
  for (i=0; i<m; i++){
    double tmp = 0.;
     for (j=0; j<n; j++){
      tmp += A[i*n+j]*y[j]; // no sequential dependency
      }
    x[i] = tmp;

    }
}
