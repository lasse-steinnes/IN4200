#ifndef SET_4_EXS_HPP
#define SET_4_EXS_HPP

/*
Note: You can also make a header with just small functions
not set in a class, if a class structure with inheritance is
not needed
*/

class Exercises_4{
protected:
public:
  double rand_gen(double min, double max);
  double * matrix_mult(double * mtA, double * mtB, int n, int m, int p);
  double * matrix_mult_unroll(double * A, double * B, int n, int m, int p);
};

#endif
