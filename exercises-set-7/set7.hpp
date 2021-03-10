#ifndef SET_7_HPP
#define SET_7_HPP

class SET7_EXS{
  protected:
  public:
    void dense_mat_vec(int m, int n, double *x, double *A, double *y);
    void dense_mat_vec_parallel(int m, int n, double *x, double *A, double *y);
};

#endif
