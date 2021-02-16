#include <iostream> // cin cout
#include <time.h> // clock
#include "ex_set_5.hpp"

#define CLOCKS_TO_MS(t) (t)/((double )CLOCKS_PER_SEC)*1000


using namespace std;
int main(int argc, char const *argv[]){
  int exercise;
  cout << "Press 1 to run exercise 2 \n";
  cout << "Press 2 to run ex.3 \n";
  cout << "Enter number:" << " ";
  cin >> exercise;

  if (exercise == 1){
    int N = 7;
    Exercises_5 Solver;
    // allocating matrices
    double *y;
   double **A = new double*[N];
   double *B = new double[N];
   y = (double *) calloc(N, sizeof(double));

   for (size_t i = 0; i < N; i++)
       A[i] = new double[N];

  // set seed
  srand(time(0));
  // assign values to matrice
  for (size_t i = 0; i < N; i++)
    for (size_t j = 0; j < N; j++)
        A[i][j] = Solver.rand_gen(-2,2);

    // assigning values to vector
    for (size_t i = 0; i < N; i++){
      B[i] = Solver.rand_gen(-2,2);
    }

    clock_t begin,begin2, end, end_unroll;

    // original
    begin = clock();
    for (size_t j=0; j<N; j++){
      for (size_t i=0; i<=j; i++){
        y[j] += A[j][i]*B[i];
      }
    }
    end = clock() - begin;


    // want to unroll and jam
    int stride = 4;
    int remaind = N%stride;  // remainder
    begin2 = clock(); // be careful with using the SAME object, lol.

    for (size_t j=0; j<N-remaind; j+= stride){
      for (size_t i=0; i<=j; i++){
        y[j] += A[j][i]*B[i];
        y[j+1] += A[j+1][i]*B[i];
        y[j+2] += A[j+2][i]*B[i];
        y[j+3] += A[j+3][i]*B[i];
      }
    }

    // if N is not divisible by 4, do normal loop for rest
    for (size_t j=N-remaind; j<N; j++){
      for (size_t i=0; i<=j; i++){
        y[j] += A[j][i]*B[i];
      }
    }
    end_unroll = clock() - begin2;
    // expects end_unroll to be smaller after all --> so divide by this
    double ratio = CLOCKS_TO_MS(end)/((double) CLOCKS_TO_MS(end_unroll));
    printf("speedup of unroll and jam: %.2f \n", ratio);

    delete [] y;
    delete [] B;
    delete [] A[0];
    delete [] A;
    return 0;
    }

    if (exercise == 2){
      Exercises_5 Solver;
      // assign dynamic memory
      int N = 1e3;
      double **A = new double*[N];
      double **B = new double*[N];
      int *c = new int [N];

      // initialize
      for (size_t i= 0; i < N; i++){
        A[i] = new double[N];
        B[i] = new double[N];
      }

      srand(time(0));
      for (size_t i = 0; i < N; i++){
        c[i] = rand()%100; // int [0,99]
      }

      // fill in matrice elements
      for (size_t i = 0; i < N; i++){
        for (size_t j = 0; j < N; j++){
          A[i][j] = Solver.rand_gen(-2,2);
          B[i][j] = Solver.rand_gen(-2,2); //float [-2,2]
        }
      }

      // take time
      clock_t start, end, start_opt,end_opt,start_unroll,end_unroll;

      // optimize a function foo
      // original version
      start = clock();
      Solver.foo(N,A,B,c);
      end = clock() - start;

      // optimized version
      start_opt = clock();
      Solver.foo_opt1(N,A,B,c);
      end_opt = clock() - start_opt;

      // optimized version
      start_unroll = clock();
      Solver.foo_opt_unroll(N,A,B,c);
      end_unroll = clock() - start_unroll;

      /*
       Note - an even faster speedup would be to prescribe value
        to val outside the loop through implicit indexing via a table
        - see intstuctor solutions - foofaster
        */

      double ratio = CLOCKS_TO_MS(end)/((double) CLOCKS_TO_MS(end_opt));
      double ratio2 = CLOCKS_TO_MS(end)/((double) CLOCKS_TO_MS(end_unroll));

      printf("Speedup of simplifying trig.: %.2f \n", ratio);
      printf("Speedup of unroll: %.2f \n", ratio2);
      printf("Speedup higher with less usage of sin/cosine! \n");
      return 0;
    }

    else{
      cout << "Error - Bad usage: No such option! \n";
      return 1;
      }
}
