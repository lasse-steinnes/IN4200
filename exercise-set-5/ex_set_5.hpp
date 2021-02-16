#ifndef EX_SET_5_HPP
#define EX_SET_5_HPP

class Exercises_5{
protected:

public:
    double rand_gen(double min, double max);
    void foo(int N, double **mat, double **s, int *v);
    void foo_opt1(int N, double **mat, double **s, int*v);
    void foo_opt_unroll(int N, double **mat, double **s, int*v);

};

#endif
