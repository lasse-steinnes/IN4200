// main for exercise set 4
#include <iostream>
#include <omp.h>

using namespace std;

int main(int argc, char const *argv[]){
  int exercise;
  cout << "Press 1 to run ex.1 \n";
  cout << "Press 2 to run ex.2 \n";
  cout << "Press 3 to run ex.3 \n";
  cout << "Press 4 to test openmp \n";
  cout << "Enter number:" << " ";
  cin >> exercise;

  if (exercise == 1){
    printf("Task 1: Measuring realistically achievable memory bandwidth \n");
    double s = 1.0;
    int N = 1e5;

    double *A = new double[N];
    double *B = new double[N];

    // can use STREAM benchmark to find mem bandwidth
    //kernel loop
    for (int i = 0; i < N;i++){
      A[i] = s*B[i];
    }
  }

  if (exercise == 2){
    printf("Task 2: Strided access \n");
  }

  if (exercise == 3){
    printf("Task 3: Matrix-matrix multiplication \n");

    //defining constants (examples)
    int n = 10;
    int m = 20;
    int p = 15;

    // Allocating memory - 2D, one dimensional contiguous storage
    // access elements columnwise by (row_i*n_columns + col_j)
    // inner loop over colum - since row major storage!! :)
    double *A = new double[n*m];
    double *B = new double[m*p];
  }
  if (exercise == 4){
    #pragma omp parallel
      std::cout << "Current thread number: " << omp_get_thread_num() << std::endl;
      std::cout << "Hello, World!" << std::endl;
  }
  return 0;
}
