#include "set1ex1.hpp"
#include "set1ex2.hpp"
#include "set1ex3.hpp"

#include <iostream>


using namespace std;

int main(int argc, char const *argv[]){
  // menu for exercise set 1
  int exercise;
  cout << "Press 1 to run ex.1 \n";
  cout << "Press 2 to run ex.2 \n";
  cout << "Press 3 to run ex.3 \n";
  cout << "Enter number:" << " ";
  cin >> exercise;

  if (exercise == 1){ // tasks for exercise 1
    Exercise_1 Solver;

    int part;
    cout << "Press 1 to run a) limit \n";
    cout << "Press 2 to run b) max/min \n";
    cout << "Press 3 to run c) matrix\n";
    cout << "Press 4 to run d) 3D pointer\n";
    cout << "Enter number:" << " ";
    cin >> part;

    if (part == 1){
    cout << "a) Finding the limit: \n";
    int Nmax = 1000;
    double tol = 1e-12;
    Solver.find_lim(Nmax,tol);
    cout << "\n";
    }

    if (part == 2){
    cout << "b) Allocating and find max/min \n";
    int n = 10;
    Solver.allocate(n);
    Solver.max_min(n); // finding the max/min of the allocated array
    cout << "\n";
    }

    if (part == 3){
    cout << "c) Assign a matrix two ways \n";
    int rows = 3, cols = 3;
    Solver.matrix_assign(rows, cols);
    cout << "\n";
    }

    if (part == 4){
    cout << "d) Making  a flattened 3D pointer \n";
    int nx = 2, ny = 2, nz = 2;
    Solver.allocate_flat_3D(nx,ny,nz);
    cout << "\n";
    }
  }

  if (exercise == 2){ // tasks for exercise 2
    Exercise_2 Solver;

    int part;
    cout << "Press 1 to run a) read temperature \n";
    cout << "Press 2 to run b) smooth a 2D array \n";
    cout << "Enter number:" << " ";
    cin >> part;

    if (part == 1){
    cout << "Reading temperature from file \n";
    string filename = "temps" ;
    int data_size = 6;
    Solver.read_file(filename,data_size);
    cout << "\n";
    }

    if (part == 2){
    cout << "Smoothing out an (int) array \n";
    int rows = 5, cols = 5;
    double c = 0.25; // smoothing factor
    int **arr, **smooth_arr;

    arr = Solver.create_2D(rows, cols);
    smooth_arr = Solver.smooth_2D(c,arr);
    }
  }

  if (exercise == 3){ // tasks for exercise 3
      Exercise_3 Solver;
      int arr[] = {12, 5, 1, 2, 9, 6};
      int n = sizeof(arr) / sizeof(arr[0]);

      int part;
      cout << "Press 1 to run a) quicksort \n";
      cout << "Press 2 to run b) revised quicksort \n";
      cout << "Enter number:" << " ";
      cin >> part;

      if (part == 1){
      int *sort_arr;
      sort_arr = Solver.quicksort(arr, 0, n); // so n is the size not endpoint
      cout << "sorted array \n";
      for (int i = 0; i < n; i++){
        cout << sort_arr[i] << " ";
      }
      cout << "\n";
      }


      if (part == 2){
      cout << "Quicksort with permutation vector \n";
      int *perm;
      int * indices= new int[n]; // declaring the indices vector
      for (int i; i < n; i++){
       indices[i] = i;
      }

      perm = Solver.quicksort_revised(indices,arr,0,n);

      cout << "permutated indices\n";
      for (int i = 0 ; i < n; i++){
        cout << perm[i] << " ";
      }
      cout << "\n";

      cout << "permuted array \n";
      for (int i = 0; i < n; i++){
        cout << arr[perm[i]] << " ";
      }
      cout << "\n";
    }
  }
  return 0;
}
