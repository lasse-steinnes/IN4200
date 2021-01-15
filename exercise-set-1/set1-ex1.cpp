// class for set 1 tasks: Make functional code first - then think of efficiecy;
#include "set1ex1.hpp"
#include <iostream>
#include <stdlib.h> // rand
#include <time.h>

using namespace std;

void Exercise_1::find_lim(int max_N, double tol){ // finding the limit of the sum
  int i = 0;
  double sum = 1;
  double denom = -4.0;

  while (i <= max_N){
    sum += 1.0/denom;
    denom = (-4)*denom;
    i += 1;
  }

  cout << "Number of sums:" << " " << i-1 << '\n';
  cout << "Limit from sum:" << " " << sum << '\n';
  cout << "Actual limit:" << " " << 4.0/5.0 << '\n';
  if (abs(sum - 4.0/5.0) <= tol){
    cout << "Convergence of sum verified within tol" << " " << tol << '\n';
  }else{
    cout << "Convergence not within tol:"  << " " << tol << '\n';
  }
};

void Exercise_1::allocate(int n){ // allocates random numbers to 1D array
  // using pointers to repeat some knowlegde
  // allocating memory for arrays
  m_ptr = new int[n]; // member variable, to refer to address m_ptr = &varname

  srand(time(NULL)); // seed initiating rand
  // filling in random numbers between 0 and 99
  for (int i = 0; i < n; i++){
    *(m_ptr + i) = rand() % 100; // accessing the variable values with *
    //cout << *(m_ptr + i) << '\n'; // optional: access as array indexing m_ptr[i]
  }
  //cout << "This is the 2n. array element:" << " " << *(ptr + 1) << "\n";
}

void Exercise_1::max_min(int n){ // find the max and min of the allocated
  int mx = 0, mn = 99;
  for (int i = 0; i < n; i++){ // option: use the built in max min
    if (mx < m_ptr[i]){
      mx = m_ptr[i]; // different notation, but same meaning
    }
    if (mn > m_ptr[i]){
      mn = m_ptr[i];
    }
  }
  cout << "max value of array:" << " " << mx << '\n';
  cout << "min value of array:" << " " << mn << '\n';

  // delete pointer when finished
  delete [] m_ptr;
}


void Exercise_1::matrix_assign(int rows, int cols){ // assigns an m times n array
  // declaring a pointer for a m (row) x n (cols) matrix of integer values

  // dynamic allocation, note the possibility of a flat matrix with size rows*cols
  int **matrix = new int*[rows];
  for (int i = 0; i < rows; i++){
    matrix[i] = new int[cols]; // make columns row by row
  }

  // using rand for values to fill in
  int size = rows*cols;
  int *ptr = new int[size];
  srand(time(NULL));
  for (int i = 0; i < size; i++){
    ptr[i] = rand() % 100;
  }
  // filling in values by rows
  for (int i = 0; i < rows; i ++){
    for (int j =0; j < cols; j ++){
      matrix[i][j] = ptr[i*cols + j];
      cout << matrix [i][j] << " ";
    }
    cout << "\n";
  }

  cout << "\n";

  // filling in by columns
  for (int j = 0; j < cols; j ++){
    for (int i = 0; i < rows; i ++){
      matrix[i][j] = ptr[i*cols + j];
      }
    }

  // printing out the values from the above operation (cols)
  for (int i = 0; i < rows; i ++){
    for (int j =0; j < cols; j ++){
      cout << matrix [i][j] << " ";
    }
    cout << "\n";
  }

    for (int i = 0; i < rows; i++){
     delete []  matrix[i]; // rows
    }

    delete [] matrix;
    delete [] ptr;

    /*
    note: this creates and n x m matrix
    int **matrix2 = new int*[cols];
    for (int j = 0; j < cols; j++){
      matrix[j] = new int[rows]
    }
    */
}

void Exercise_1::allocate_flat_3D(int nx,int ny,int nz){
  int size = nx*ny*nz;
  int *matrix = new int[size]; //nx (rows) x ny (cols) x nz (layer) dimensions

  // assign using rand
  srand(time(NULL)); // seed

  for (int i = 0; i < nx; i++){
    for (int j = 0; j < ny; j++){
      for (int k = 0; k < nz; k++){
        matrix[i*ny*nz + j*nz + k] =  rand() % 100;  // = matrix[i,j,k]
      }
    }
  }

  /*
  printing the layered matrix (can define which index to be the "layer"
   - here defined as k)
  */
  for (int k = 0; k < nz; k++){
    for (int i = 0; i < nx; i++){
      for (int j = 0; j < ny; j++){
        cout << matrix[i*ny*nz + j*nz + k] << " ";
      }
      cout << "\n";
    }
    cout << "\n";
  }

  delete [] matrix;
}
