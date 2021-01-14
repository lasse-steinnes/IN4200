#ifndef SET1EX1_HPP
#define SET1EX1_HPP

// setting up class structure for exercise 1
class Exercise_1{
protected:
  int *m_ptr;

public:
  void find_lim(int N, double tol); // finding the limit of a sum
  void allocate(int n); // allocates random numbers to 1D array
  void max_min(int n); // find the max and min of the allocated
  void matrix_assign(int m, int n); //  two ways to assign mxn matrix
  void allocate_flat_3D(int xdims,int ydims, int zdims);  // allocating flat 3D matrix
};

#endif
