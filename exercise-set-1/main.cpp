#include "set1ex1.hpp"
#include "set1ex2.hpp"
#include "set1ex3.hpp"

#include <iostream>
#include <armadillo>
//#include <stdio.h>
//#include <cmath>
//#include <chrono>

using namespace std;
using namespace arma;

int main(int argc, char const *argv[]){
  // menu for exercise set 1
  int exercise;
  cout << "Press 1 to run ex.1 \n";
  cout << "Press 2 to run ex.2 \n";
  cout << "Press 3 to run ex.3 \n";
  cout << "Enter number:" << " ";
  cin >> exercise;

  if (exercise == 1){
    cout << "a) Finding the limit:" << "\n";
    int Nmax = 1000;
    double tol = 1e-12;
    Exercise_1 Solver;
    Solver.find_lim(Nmax,tol);
    cout << "\n";

    cout << "b) Allocating and find max/min" << "\n";
    int n = 10;
    Solver.allocate(n);
    Solver.max_min(n); // finding the max/min of the allocated array
    cout << "\n";

    cout << "c) Assign a matrix two ways" << "\n";
    int rows = 3, cols = 3;
    Solver.matrix_assign(rows, cols);
    cout << "\n";

    cout << "d) Making  a flattened 3D pointer" << "\n";
    int nx = 2, ny = 2, nz = 2;
    Solver.allocate_flat_3D(nx,ny,nz);

  }

  if (exercise == 2){

  }

  if (exercise == 3){

  }

  return 0;
}
