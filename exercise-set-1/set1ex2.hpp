#ifndef SET1EX2_HPP
#define SET1EX2_HPP

// setting up class structure for exercise 1
class Exercise_2{
protected:

public:
  void init();
  void get_temperature(); // read temperature
  void max_min_at_time(); // find the max min and which time they occured
  void mean_std(); //find the mean and standard deviation
  void smooth_2D(); // smooths a given 2D vector
  void solver(int method); // solves a given exercise
};

#endif
