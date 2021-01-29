#ifndef SET2EX3_HPP
#define SET2EX3_HPP

class Exercise_3{
protected:

public:
  double * allocate(int nx,int ny,int nz);
  void solve(int nx, int ny, int nz, int num_iter, double *u, double *v);
};

#endif
