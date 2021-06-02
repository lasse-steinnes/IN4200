#ifndef convolution_H // header file for convolution
#define convolution_H

// function for double convolution
void MPI_double_layer_convolution(int M, int N, float **input, int K1, float **kernel1,
int K2, float **kernel2, float **output);
#endif
