// main for exercises set 7
#include <iostream> // cin
#include <stdlib.h> // rand
#include "set7.hpp"
#include <omp.h> // openmp
#include <time.h> // time
#include <math.h> // sqrt


#define CLOCKS_TO_MS(t) (t)/((double )CLOCKS_PER_SEC)*1000

using namespace std;

double foo(int j);

int main(int argc, char const *argv[]){
  int exercise;
  cout << "Press 1 to run exercise 1 \n";
  cout << "Press 2 to run exercise 2 \n";
  cout << "Press 3 to run exercise 3 \n";
  cout << "Enter number:" << " ";
  cin >> exercise;

  // arrays and matrices for tasks 1 and 2
  int n = 1e5;
  double *a = new double[n];
  double *b = new double[n];

  srand(time(0)); // set seed

  for (size_t i = 0; i < n; i++){
    a[i] = rand()%100+1; //[1,100]
    b[i] = rand()%30+50; // [50,80]
  }


  if (exercise == 1){
    cout << "Make loops parallel if possible\n";


    char sub;
    cout << "Choose subtasks a-h \n";
    cout << "Enter letter: " << " ";
    cin >> sub;


    if (sub == 'a'){
      cout << "embarassingly parallel \n";
      double x = 10;
      #pragma omp parallel for
        for (size_t i=0; i < (int) sqrt(x); i++){
        //printf("%d \n", omp_get_num_threads());
        a[i] = 2.3 * x;
        if (i < 10){
        b[i] = a[i];
          }
        }
    }

    if (sub == 'b'){
      cout << "Not parallelizable bc. of flag \n";
      // flag condition here &!flag means  ... & not true
      // stops parallel region
      // if parallel then maybe it would terminate differently/thread
      int flag = 0;
      for (size_t i = 0; (i<n) & (!flag); i++) {
        a[i] = 2.3 * i;
        if (a[i] < b[i]) flag = 1;
      }
    }

    if (sub == 'c'){
      cout << "embarassingly paralell \n";
      #pragma omp parallel for
      for (int i = 0; i < n; i++)
        a[i] = foo(i); // functions are automatically thread private

    }

    if (sub == 'd'){
      cout << "embarassingly paralell \n";
      #pragma omp parallel for
      for (int i = 0; i < n; i++) {
        a[i] = foo(i); // again function w/o break etc --> thread private
        if (a[i] < b[i]) // and parallel
        a[i] = b[i]; // no special dependencies --> parallelizable
      }
    }

    if (sub == 'e'){
      cout << "break --> not parallel \n";
      for (int i = 0; i < n; i++){
        a[i] = foo(i);
        if (a[i] < b[i]) // content of a depends on order of iteration
        break;           // not parallel. If parallel --> race condition
      }              // sequential dependence
    }

    if (sub == 'f'){
      cout << "parallel: one line reduction \n";
      double dotp = 0.0;
      #pragma omp parallel for reduction(+:dotp) // good :)
      for (int i = 0; i < n; i++){
        dotp += a[i] * b[i]; // reading to same variable, indep. operations
      }
    }

    if (sub == 'g'){
      int k = 5;
      int i;
      cout << "embarassingly parallel \n";
      #pragma omp parallel for private(i)
      for (i = k; i < 2*k; i++)
        a[i] = a[i] + a[i-k]; // note a[i] must have n >= 2k elements
                              // parallel because i-k access previous elements in a
    }                          // i-k is in range [0,k] (outside altered region)

    if (sub = 'h'){ //
      double c = 1.5;
      int k = 5;
      cout << "not parallel \n";
      for (int i = k; i < n; i++)
        a[i] = c * a[i-k]; // i-k in range [0,n-k] - can be inside altered region
                            // sequential dependency!! --> not parallel unless n <= 2k!
    }
  }

  if (exercise == 2){
    int chunk;
    printf("Solving task 2: Dot product\n");
    printf("Choose chunksize (int): ");
    cin >> chunk; // to be distributed to each thread at a time


    // MEASURE TIME
    clock_t begin_s, end_s, begin_d, end_d, begin_g,end_g;

    /* Note that we can use
    double start = omp_get_wtime();
    // Do some parallel work here
    double end = omp_get_wtime();
    double total = end - start;
    */

    // CHOOSING DIFFERENT SCHEDULERS AND CHUNKSIZES

    /*
    Alternative to hardcoding; do in terminal
    export (or set) OMP_SCHEDULE=sheduling-type
    (set scheduling-type equal to runtime in program)
    */
    // static
    begin_s = clock();
    double dotp = 0.;
    int i;
    #pragma omp parallel for reduction(+:dotp) schedule(static,chunk) private(i)
    for (i = 0; i < n; i++)
    dotp += a[i]*b[i];
    end_s = clock() - begin_s;

    // dynamic
    begin_d = clock();
    dotp = 0.;
    int j;
    #pragma omp parallel for reduction(+:dotp) schedule(dynamic,chunk) private(j)
    for (j = 0; j < n; j++){
      dotp += a[j]*b[j];
    }
    end_d = clock() - begin_d;

    //guided
    begin_g = clock();
    dotp = 0.;
    int k;
    #pragma omp parallel for reduction(+:dotp) schedule(guided,chunk) private(k)
    for (k = 0; k < n; k++){
      dotp += a[k]*b[k];
    }
    end_g = clock() - begin_g;


    printf("Time with chunksize %d, guided: %.2e, static: %2.e, dynamic: %2.e [ms] \n",
     chunk, CLOCKS_TO_MS(end_g), CLOCKS_TO_MS(end_s), CLOCKS_TO_MS(end_d));
     // guided seem to be faster
     // followed by dynamic then static
     // when chunk size determined
     // without chunk size for low to high time: guided, static, dynamic
     // auto chooses one of the mentioned schedules,
     // runtime --> given in terminal or automatically chosen
     // time goes up for guided schedule with chunk size (goes down for
     //dynamic and same almost for static, chunk 5 -50 - 100)
     // --> should remember to present speedup as fraction compared
     // to unparallel serial execution of loop
  }

  if (exercise == 3){
    printf("Solving task 3: Dense matrix vector multiplication \n");
    // use of class
    int m, n;
    printf("Choose dimension of A mxn \n");
    printf("Enter m (int): ");
    cin >> m;
    printf("Enter n (int):");
    cin >> n;

    // initialize  mat A and vector x, y
    double *x = new double[n];
    double *y = new double[n];
    double *A = new double[m*n]; // calling it with [i*n_cols + j]


    // fill in values for y
    srand(time(0)); // set seed
    #pragma omp parallel for
    for (size_t i = 0; i < n; i++){
        y[i] = rand()%100+1; //[1,100]
    }

    // fill in values for A
    #pragma omp parallel for
    for (size_t i = 0; i < m; i++)
      for (size_t j= 0; j < n; j++)
        A[i*n + j] = rand()%100+1; //[1,100]

    char task;
    printf("Press a to run serial \n");
    printf("Press b to run parallel program\n");
    printf("Press c to run parallel blocks in main \n");
    printf("Enter letter a/b/c: ");
    cin >> task;
    SET7_EXS Solver;

    if (task == 'a'){
      Solver.dense_mat_vec(m,n,x,A,y);
    }

    if (task == 'b'){
      Solver.dense_mat_vec_parallel(m,n,x,A,y);
    }

    if (task == 'c'){
      /* For task c
      use  dense_mat_vec and work with "blocks"*/ //(see intructor sols)
      #pragma omp parallel // note not parallel for here, but general omp region
      {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        // Using integer division to divide up the rows means that the last
        // remainder = m%num_threads, gets an extra row.
        int start_m = (thread_id*m)/num_threads;
        int stop_m = ((thread_id+1)*m)/num_threads; // parallel over rows

        // calls the serial dense_mat_vec function
        Solver.dense_mat_vec(stop_m-start_m, n, &x[start_m], &A[start_m*n], y);
      }
    }



    delete [] A;
    delete [] x;
    delete [] y;

  }

  if (exercise == 0 || exercise > 3){ // || is the same as or
    cout << "Wrong usage, choose correct task number \n";
    return 1;
  }

  delete [] a;
  delete [] b;

  return 0;
}

double foo(int j){
  return j*j;
}
