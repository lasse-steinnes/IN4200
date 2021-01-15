#include "set1ex2.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
//uses namespace std from header file

void Exercise_2::read_file(string filename, int size){
  // read from txt file cpp style
  // input: file name (string), size (of dateset)(int)
  string line, T, time_; // string to store
  double *temps = new double[size];
  string *times = new string[size];

  ifstream mystream ("./data/" + filename + ".txt"); // open the file
  if (mystream.is_open()){
    int i = 0;
    while (getline(mystream,line)){

      istringstream ss(line); // separate from whitespaces
      ss >> time_ >> T;
      temps[i] = stod(T); // transform to double
      times[i] = time_;
      i++;
    }
      // finding max and minimum temperature
      max_min(temps, size);
      // cout something here
      cout << "max deg. Celsius: " << T_max << " " << "at time: " << times[i_Tmax];
      cout << "\n";
      cout << "min deg. Celsius: " << T_min << " " << "at time: " << times[i_Tmin];
      cout << "\n";

      // calculating the average and standard deviation
      mean_std(temps,size);

      mystream.close();
      delete [] temps;
      delete [] times;
    } else {
    cout << "Unable to open given filename (txt-file)";
  }
}

void Exercise_2::max_min(double *ptr, int N){ // taking pointer as input
  int i = 0;
  i_Tmin = i; i_Tmax = i;
  T_max = ptr[i]; T_min = ptr[i];

  while (i <= N){
    i++;
    if (ptr[i] < T_min){
      T_min = ptr[i];
      i_Tmin = i;
    }
    if (ptr[i] > T_max){
      T_max = ptr[i];
      i_Tmax = i;
    }
  }
}

void Exercise_2::mean_std(double *ptr, int N){
  // finding mean
  double sum = 0.0;
  for (int i = 0; i < N; i++){
    sum += ptr[i];
  }
  double expval = sum/N;
  // finding std
  sum = 0;
  for (int i = 0; i < N; i++){
    sum += (ptr[i]-expval)*(ptr[i]-expval);
  }
  double std = sqrt(sum/(N-1));// bessel correction to population std N-1

  cout << "E(T): " << expval << " " << "deg. Celsius, std: " << std;
  cout << " deg. Celsius" <<"\n";
}

int ** Exercise_2::create_2D(int rows, int cols){
  m_rows = rows, m_cols = cols;
  int **matrix = new int*[rows];
  for (int i = 0; i < rows; i++){
    matrix[i] = new int[cols]; // make columns row by row
  }

  srand(time(NULL)); // seed
  // filling in values by rows
  for (int i = 0; i < rows; i ++){
    for (int j =0; j < cols; j ++){
      matrix[i][j] = rand() % 100;
    }
  }
  return matrix;
}

int ** Exercise_2::smooth_2D(double c, int **ptr){ //
  /*
  smoothing out the inner elements of given 2D array
  input:
  ptr: 2D array to be smoothened
  c: smoothing coefficient
   */

  cout << "unsmoothed matrix \n";
  for (int i = 0; i < m_rows; i ++){
    for (int j =0; j < m_cols; j ++){
      cout << ptr[i][j] << " ";
    }
    cout << "\n";
  }
  cout << "\n";

  // smooth matrix
  for (int i = 1; i < m_rows-1; i ++){
    for (int j = 1; j < m_cols-1; j ++){
      ptr[i][j] = ptr[i][j] + c*(ptr[i-1][j] + ptr[i][j-1] \
                  - 4*ptr[i][j] + ptr[i][j] +  ptr[i+1][j]);
    }
  }

  cout << "smoothened matrix  (inner elements)\n";
  for (int i = 0; i < m_rows; i ++){
    for (int j =0; j < m_cols; j ++){
      cout << ptr[i][j] << " ";
    }
    cout << "\n";
  }
  cout << "\n";

  return ptr;
}
