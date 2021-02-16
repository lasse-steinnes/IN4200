#include "ex_set_5.hpp"
#include <stdlib.h> // rand
#include <math.h> // sine and cosine

double Exercises_5::rand_gen(double min, double max){
    double range = (max - min);
    double div_ = range/RAND_MAX;
    return min + (rand()*div_);
}


void Exercises_5::foo(int N, double **mat, double **s, int *v){
    int i,j;
    double val;
    for (j=0; j<N; j++){
      for (i=0; i<N; i++) {
        val = 1.0*(v[j]%256);
        mat[i][j] = s[i][j]*(sin(val)*sin(val)-cos(val)*cos(val));
      }
    }
}

void Exercises_5::foo_opt_unroll(int N, double **mat, double **s, int *v){
    double val, val2,val3,val4;
    double cc_val,cc_val2,cc_val3,cc_val4;
    double ss_val,ss_val2,ss_val3,ss_val4;
    int stride = 4;
    int remaind = N%4;

    for (size_t i=0; i<N-remaind; i+= stride){
      val = 1.0*(v[i]%256);   val2 = 1.0*(v[i]%256);
      val3 = 1.0*(v[i]%256);   val4 = 1.0*(v[i]%256);
      cc_val = cos(val)*cos(val); ss_val = sin(val)*sin(val);
      cc_val2 = cos(val2)*cos(val2); ss_val2 = sin(val2)*sin(val2);
      cc_val3 = cos(val3)*cos(val3); ss_val3 = sin(val3)*sin(val3);
      cc_val4 = cos(val4)*cos(val4); ss_val4 = sin(val4)*sin(val4);

      for (size_t j=0; j<N; j++) { // unroll
        mat[i][j] = s[i][j]*(ss_val-cc_val);
        mat[i+1][j] = s[i+1][j]*(ss_val2-cc_val2);
        mat[i+2][j] = s[i+2][j]*(ss_val3-cc_val3);
        mat[i+3][j] = s[i+3][j]*(ss_val4-cc_val4);
      }
    }
    // do remainder of loop w/o stride
    for (size_t i=N-remaind; i<N; i++){
      val = 1.0*(v[i]%256);
       cc_val = cos(val)*cos(val);
       ss_val = sin(val)*sin(val);
      for (size_t j=0; j<N; j++) {
        mat[i][j] = s[i][j]*(ss_val-cc_val);
      }
    }

}

void Exercises_5::foo_opt1(int N, double **mat, double **s, int *v){
    double val;
    double c_val;

    for (size_t i=0; i<N; i++){
      val = 1.0*(v[i]%256);
      c_val = -cos(2.0*val);

      for (size_t j=0; j<N; j++) {
        mat[i][j] = s[i][j]*c_val;
      }
    }

}
