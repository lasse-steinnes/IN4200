#include "set2-ex3.hpp"
#include <math.h>

#define PI 3.14159265

#define idx(i, j, k) i*ny*nz + j*nz + k

double * Exercise_3::allocate(int nx, int ny, int nz){
  int size = nx*ny*nz;
  double *matrix = (double *)malloc(size *sizeof(double)); //nx (rows) x ny (cols) x nz (layer) dimensions
  double fraction = PI/((nx-1)*(ny-1)*(nz-1));
  for (int i = 0; i < nx; i++){
    for (int j = 0; j < ny; j++){
      for (int k = 0; k < nz; k++){
        matrix[i*ny*nz + j*nz + k] =  2.0 + sin(i*j*k*fraction);
      }
    }
  }
  return matrix;
  free(matrix);
}

void Exercise_3::solve(int nx, int ny, int nz, int num_iter, double *u, double *v){
  // make the computation and copy
  double *temp;
  double fraction = 1/6.0;

  int counter = 0;
  while (counter++ < num_iter){
  for (int i = 1; i < nx-1; i++){
    for (int j = 1; j < ny-1;j++){
      for (int  k = 1; k < nz-1; k++){
        //calculate u
        u[idx(i,j,k)] = v[idx(i,j,k)] +
        fraction*(v[idx(i-1,j,k)] + v[idx(i,j-1,k)]
        + v[idx(i,j,k-1)] - 6*v[idx(i,j,k)]  +
          v[idx(i+1,j,k)] + v[idx(i,j+1,k)]
          + v[idx(i,j,k+1)]);
        }
      }
    }
    // copying content of u over to v;
    temp = v;
    v = u;
    u = temp;
    //new iteration
  }
}
