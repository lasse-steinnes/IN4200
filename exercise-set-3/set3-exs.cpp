#include "set3-exs.hpp"

double Set3_Exercises::numerical_integration(double x_min, double x_max, int slices){
  double delta_x = (x_max-x_min)/slices; double x, sum = 0.0;
  for (int i=0; i < slices; i++) {
    x = x_min + (i+0.5)*delta_x;
    sum = sum + 4.0/(1.0+x*x);
  }
  return sum*delta_x;
}
