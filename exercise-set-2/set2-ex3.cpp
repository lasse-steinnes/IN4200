#include "set2-ex3.hpp"
#include <math.h>

#define PI 3.14159265

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
