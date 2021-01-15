#ifndef SET1EX2_HPP
#define SET1EX2_HPP
#include <string>
using namespace std;

// setting up class structure for exercise 1
class Exercise_2{
protected:
  int i_Tmax, i_Tmin;
  double T_max, T_min;
  int m_rows, m_cols;


public:
  void read_file(string filename, int size); // read temperature
  void max_min(double *ptr, int N);
  void mean_std(double *ptr, int N); //find the mean and standard deviation
  int ** create_2D(int rows, int cols);
  int ** smooth_2D(double c, int **ptr); // smooths a given 2D vector
};

#endif
