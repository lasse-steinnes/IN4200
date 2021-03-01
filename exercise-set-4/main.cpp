// main for exercise set 4
#include <iostream>
#include <omp.h>
#include <stdlib.h> // rand (PS: Know that rand doesn't have high precision)
#include <time.h> // time (to set seed)
#include "set_4_exs.hpp"

#define CLOCKS_TO_SEC(t) (t)/((double )CLOCKS_PER_SEC)

using namespace std;

int main(int argc, char const *argv[]){
  int exercise;
  cout << "Press 1 to run ex.1 and 2 \n";
  cout << "Press 2 to run ex.3 \n";
  cout << "Press 3 to verify openmp (4 cores) \n";
  cout << "Enter number:" << " ";
  cin >> exercise;

  if (exercise == 1){
    /* note: stream benchmark is memory bound */
    printf("\n");
    printf("Task 1: Measuring realistically achievable memory bandwidth \n");

    // defining variables
    double s = 2.5;
    int N = 1e6;
    double max = -2.0;
    double min = 2.0;

    // allocating memory for vectors
    double *A = new double[N];
    double *B = new double[N];

    // clock variables
    clock_t start, start_stride, end,end_stride;

    // set seed
    srand(time(0));

    Exercises_4 Solver;
    // assigning A and B with random numbers
    for (int i = 1; i < N; i++){
      A[i] = Solver.rand_gen(min,max);
      B[i] = Solver.rand_gen(min,max);
    }

    // can use STREAM benchmark to find mem bandwidth (words/second)
    //kernel loop (SCALE)
    start = clock();
    for (int i = 0; i < N;i++){
      A[i] = s*B[i];
    }
    end = clock();

    double time_ = CLOCKS_TO_SEC(end-start); // time in seconds
    int n_bytes = 2*N*sizeof * A; // 1 load (Bi) + 1 store (Ai) = 2 words
    double FP1 =  ((double) N)/time_;

                              // N iterations, double preci: 8 bytes = 64 bits
    printf("Runtime: %2.f ms \n",time_*1000);
    printf("Realistically achievable bandwith: %.2e [byte/sec]\n", ((double) n_bytes)/time_);
    printf("In words/sec: %.2e \n",2*N/((double) time_));
    printf("FLOPS/sec %2.e \n",FP1);

    delete [] A;
    delete [] B;

    printf("Task 2: Strided access \n");
    /*
     Want to find out what strided access does to performance in flops/sec
     */

     // assigning A and B with random numbers
     for (int i = 1; i < N; i++){
       A[i] = Solver.rand_gen(min,max);
       B[i] = Solver.rand_gen(min,max);
     }

     int stride = 10; // jumps more in memory?
     start_stride = clock();
     for (int i = 0; i < N;i+=stride){
       A[i] = s*B[i];
     }
     end_stride = clock();
     double time_st = CLOCKS_TO_SEC(end_stride-start_stride);
     double FP2 = (int)(N/(double) stride + 0.5)/(double) time_st;

     // free dynamic memory
     delete [] A;
     delete [] B;

     // note that the + 0.5 is so that we round up
     // the standard for c is to round down
     printf("\n");
     printf("Runtime: %2.f ms \n",time_st*1000);
     printf("Realistically achievable bandwith: %.2e [byte/sec]\n", (int)(n_bytes/(double) stride+0.5)/(double) time_st);
     printf("In words/sec: %.2e \n",2* (int) (N/(double) stride + 0.5)/(double) time_st);
     printf("FLOPS/sec %2.e \n",FP2);
     printf("FLOPS/sec is a lot (!) lower with strided access, ratio %.2f \n",FP2/FP1);
  }

  if (exercise == 2){
    printf("Task 3: Matrix-matrix multiplication \n");

    //defining constants (examples)
    int n = 3;
    int m = 3;
    int p = 4;
    double min = -2.0;
    double max = 2.0;

    //clock
    clock_t start, end, start_unroll, end_unroll;

    // Allocating memory - 2D, one dimensional contiguous storage
    // access elements columnwise by (row_i*n_columns + col_j)
    // inner loop over colum - since row major storage!! :)
    int dimA = n*m;
    int dimB = m*p;

    double *A = new double[n*m];
    double *B = new double[m*p];
    double *res = new double[n*p];
    double *res2 = new double[n*p];

    srand(time(0)); // seed
    Exercises_4 Solver;
    // assigning random values between -2 and 2 to A and B
    // note also have a function which allocates, send in address &A
    // --> dereferencing (see intstructor solutions)
    for (int i = 1; i < dimA; i++){
      A[i] = Solver.rand_gen(min,max);
    }
    for (int i = 1; i < dimB; i++){
      B[i] = Solver.rand_gen(min,max);
    }
    // normal loop
    start = clock();
    res = Solver.matrix_mult(A,B, n,m,p);
    end = clock();

    // loop unrolling
    start_unroll = clock();
    res2 = Solver.matrix_mult_unroll(A,B,n,m,p);
    end_unroll = clock();

    double time_ = CLOCKS_TO_SEC(end - start);
    double time_un = CLOCKS_TO_SEC(end_unroll - start_unroll);
    printf("Speedup with unroll: %.2f \n", time_un/time_);
    //printf("Note: Using a temporal storage for C can be more efficient than unrolling \n");
    // not sure if this comment was false or not
    delete [] res;
    delete [] res2;
    delete [] A;
    delete [] B;
  }

  if (exercise == 3){
    #pragma omp parallel
      cout << "Current thread number: " << omp_get_thread_num() << "\n";
      cout << "Hello, World! \n" << endl;
  }
  return 0;
}
