// provides a main
#include "set3-exs.hpp"
#include <iostream>
#include <math.h>
#include <x86intrin.h>
#include <time.h>
using namespace std;

#define CLOCKS_TO_MILLISEC(t) (t*1000)/((double )CLOCKS_PER_SEC) // note how you can define

double pow_100(double x);

int main(int argc, char const *argv[]){
  // menu for exercise set 1
  int exercise;
  cout << "Press 1 to run ex.1 \n";
  cout << "Press 2 to run ex.2 \n";
  cout << "Press 3 to run ex.3 \n";
  cout << "Enter number:" << " ";
  cin >> exercise;

  if (exercise == 1){ // tasks for exercise 1
    Set3_Exercises Solver;
    double x_min,x_max, eval;
    x_min = 0.0; x_max = 1.0;
    int *slices = new int[5];

    for (int i = 1; i < 5; i++){
      slices[i] = i*100;
    };

    for (int i = 1; i < 5; i++){
      eval = Solver.numerical_integration(x_min, x_max, slices[i]);
      printf("xmin: %.2f, xmax: %2.f, slices %d \n", x_min, x_max, slices[i]);
      printf("Result of numerically integration %.5f \n", eval);
      printf("Approaching pi with relative error: %.2e \n",eval/((double) M_PI)-1);
      // above equation comes from (eval - m_pi)/m_pi
      }
      /*
      Read Time-Stamp Counter;
      this return numbers of clock cycles
      */
      //lets assume no cache misses and most flops --> n_div
      // so latency assumes outstanding prefetches
      int n_div = 10000;
      unsigned long long start = __rdtsc(); // cycles cannot be negative
      eval = Solver.numerical_integration(0, 1, n_div);
      unsigned long long end = __rdtsc();

      // clock cycles per division
      double avg = (double)(end - start)/n_div;

      printf("\n");
      printf("Cycles per division: %lf\n", avg);
      delete [] slices;
    }

    /* assuming that float point division cannot be pipelined
     -->  want to estimate latency of floating point division
     in CLOCK CYCLES */

  if (exercise == 2){ // tasks for exercise 2
  /*
    // Simple loop for dot product
    double s = 0.;
    for (int i=0; i<N; i++){
    s = s + A[i]*B[i];
  }
  */
  /* want to estimate performance (relating to bandwidth, latency etc)
   * We have 1 GHz [cycles/second] CPU = 1 [ns/cycle]
      - can do one load (or store),
      one multiplication and one addition
      per cycle
  * memory bus: 3.2 Gbytes/sec
  * Latency to load one cache line from main mem: 100 clock cycles
  * 4 double precision numbers fit into one cache line
  * Assume: loop counting and branching comes at no cost
  --> Answers: See notes and instructor suggested solution
  */
  }

  if (exercise == 3){ // tasks for exercise 3
  // special implementation of pow 100
  // Multiplication  operations take more cycles
  // per operation: Eg.
  // multiplication takes 2-4 cycles (32 bit),
  // while addition takes only 1. Thus reduce multiplication
  // and division when possible.


  clock_t start, pow_time, pow100_time;

  double x = 5.0;

  start = clock();
  double fast = pow_100(x);
  pow100_time = clock() - start;

  start = clock();
  double slow = pow(x, 100);
  pow_time = clock() - start;

  printf("Time with pow: %lf , time with special algo: %lf \n", \
        CLOCKS_TO_MILLISEC(pow_time), CLOCKS_TO_MILLISEC(pow100_time));
  printf("Speedup: %.2e \n", \
        CLOCKS_TO_MILLISEC(pow_time)/((double) CLOCKS_TO_MILLISEC(pow100_time)));
  printf("Checking same output: Residual %.2e  \n", fast - slow);
  }
  return 0;
}

double pow_100(double x){
  double x_5 = x*x*x*x*x;
  double x_20 = x_5*x_5*x_5*x_5;
  double x_60 =  x_20*x_20*x_20;
  double x_100 = x_60*x_20*x_20;
  return x_100;
};
