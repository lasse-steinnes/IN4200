
#include "set2-ex1.hpp"
#include "set2-ex3.hpp"
#include  <stdio.h> // printf
#include <iostream>
#include <time.h>  // clock_t, clock, CLOCKS_PER_SEC
#include <string.h> // memcpy and string

#define CLOCKS_TO_MILLISEC(t) (t*1000)/((double )CLOCKS_PER_SEC) // note how you can define
                                                                // here and use later
#define idx(i, j, k) i*ny*nz + j*nz + k// note that this is more efficient than function
                                      // but might be best just to write it directly
typedef double my_type; // Change double to int, short, long etc.

using namespace std;

int main(int argc, char const *argv[]){
  // menu for exercise set 1
  int exercise;
  cout << "Press 1 to run ex.1 \n";
  cout << "Press 2 to run ex.2 \n";
  cout << "Press 3 to run ex.3-4 \n";
  cout << "Enter number:" << " ";
  cin >> exercise;

  if (exercise == 1){
    // declare some objects we will need

    clock_t start, ascii_read_timer, ascii_write_timer, \
            bin_read_timer, bin_write_timer; //

    // allocate memory for data
    int len = 1e5;
    double *data;
    data = (double *)malloc(len * sizeof(double)); // dynamic mem. allocation
    for (int i = 0; i < len; i ++)
      data[i] = i; // how you would write in c


    Exercise_1 Solver;

    cout << "Write and read binary (bin) file \n";

    string *name;
    string name1 = "data-binary";
    name = &name1;

    start = clock();
    Solver.write_binary(data, *name, len); // note: arrays - need to pass len
    bin_write_timer = clock() - start;     // (vectors does not need to pass len)


    start = clock();
    Solver.read_binary(*name,len);
    bin_read_timer = clock() - start;

    string name2 = "data-ascii";
    name = &name2;
    cout << "Write and read ascii (txt) file  \n";
    start = clock();
    Solver.write_ascii(data,*name, len);
    ascii_write_timer = clock() - start;

    start = clock();
    Solver.read_ascii(*name,len);
    ascii_read_timer = clock() - start;

    free(data);

    printf("Time elapsed writing to ASCII file: %.2lf ms.\n", CLOCKS_TO_MILLISEC(ascii_write_timer));
    printf("Time elapsed writing to binary file: %.2lf ms.\n", CLOCKS_TO_MILLISEC(bin_write_timer));
    printf("Time elapsed reading from ASCII file: %.2lf ms.\n", CLOCKS_TO_MILLISEC(ascii_read_timer));
    printf("Time elapsed reading from binary file: %.2lf ms.\n", CLOCKS_TO_MILLISEC(bin_read_timer));
    cout << "It takes LESS time with binary files, but they are unreadable for humans. \n";
  }

  if (exercise == 2){
    // working with a smoothing/heat equation - undetermined system
    // needed IC and BC (constant at boundary)

    cout << "Comparing speedup, memcpy and hand written copies \n";
    clock_t start, manual_timer, memcpy_timer;
    // Defining arrays as pointers with new instead of malloc().
    int n = 1e5;
    my_type *a = new my_type[n];
    my_type *b = new my_type[n];

    // or with malloc
    //a = (double *)malloc(n * sizeof(double));
    //b =  (double *)malloc(n * sizeof(double));

    // filling in array a before copying to b
    for (int i = 0; i < n; i++) a[i] = i; // c type of loop

    //Doing the exercise directly because small code
    // manually
    start = clock();
    for (int i=0; i<n; i++) b[i]=a[i]; //
    manual_timer = clock() - start;

    // with memcpy
    start = clock();
    memcpy(b,a, n*sizeof(my_type));
    memcpy_timer = clock()- start;

    delete [] a, b;

    printf("Time elapsed with manual copy: %.2lf ms.\n", CLOCKS_TO_MILLISEC(manual_timer));
    printf("Time elapsed with memcpy copy: %.2lf ms.\n", CLOCKS_TO_MILLISEC(memcpy_timer));
    printf("Speedup: %lf \n", CLOCKS_TO_MILLISEC(manual_timer)/((double) CLOCKS_TO_MILLISEC(memcpy_timer)));
    printf("Note: When comparing values, it's nice to have a ratio.\n");
  }

  if (exercise == 3){
    /* Instructor comments
    For exercise 4, we just compute. Updatnig a cell is 9 floating point operations.
    There are (m-2)*(n-2)*(o-2) cells per time step (while step),
    and there are Num iter time steps, for a total of N*9*(m-2)*(n-2)*(o-2)
    floating point operarions.

    Oh yeah! This is an excellent candidate for compiler optimisation!
    */

    printf("Exercise 3-4: Computing with 3D arrays and measure FLOPS rate\n");
    double *v, *u; // note working with flat 3D;
    int nx = 10, ny = 10, nz = 10;
    int size = nx*ny*nz;
    clock_t  start, t_final;

    //initalizing u and v to zeroes with calloc
    u = (double *)calloc(size, sizeof(double));
    v = (double *)calloc(size,sizeof(double)); // or malloc

    Exercise_3 Solver;
    u = Solver.allocate(nx,ny,nz); // intializing v as specified
    printf("Initial values set.\n"); // think of u as v_new
    memcpy(v,u, size*sizeof(double));
    int num_iter = 50;

    start = clock();
    Solver.solve(nx,ny,nz,num_iter,u,v);
    t_final = clock() - start;

    int num_flops = 9*num_iter*(nx-2)*(ny-2)*(nz-2);

    printf("Using a %d x %d x %d grid, and %d time steps.\n", nx, ny, nz, num_iter);
    printf("Performing %lu floating point operations.\n", num_flops);
    printf("time: %.2lf, number of FLOPS/sec: %lf \n",CLOCKS_TO_MILLISEC(t_final), \
              num_flops/CLOCKS_TO_MILLISEC(t_final));

    // deallocate
    delete [] v; // calloc assigned pointer
    delete [] u; // pointer assigned with malloc/could also use new
    //delete [] temp; // note that it wouldn't delete this one for some reason
    // before I set it in solve
  }
  return 0;
}
