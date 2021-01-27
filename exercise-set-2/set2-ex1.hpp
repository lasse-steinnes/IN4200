#ifndef SET2EX1_HPP
#define SET2EX1_HPP

#include <string>
#include <iostream>

using namespace std;

// setting up class structure for exercise 1
class Exercise_1{
protected: // variables only to use within class


public: // functions
void read_binary(string filename, int n);
void write_binary(double *data, string filename, int n);
void read_ascii(string filename, int n);
void write_ascii(double *data, string filename, int n);
};

#endif
