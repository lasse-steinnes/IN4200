// main for the home exam problems
#include "shared_nn.hpp"
#include <iostream> // cin
#include <omp.h> // openmp
#include <time.h> // time


#define CLOCKS_TO_MS(t) (t)/((double )CLOCKS_PER_SEC)*1000

using namespace std;

int main(int argc, char const *argv[]){
  int exercise;
  cout << "Press 1 to read off text file \n";
  cout << "Press 2 to output shared nearest neighbours\n";
  cout << "Press 3 to find clusters \n";
  cout << "Enter number:" << " ";
  cin >> exercise;

  if (exercise == 1){
    // test reading off txt-file
    int stype;
    cout << "Choose storage type 2D/CRS (1/2):" << " ";
    cin >> stype;

    int *N; int nodes = 4039; N = &nodes;
    char *filename = "facebook_combined";
    Shared_NN Solver;

    if (stype == 1){
      cout << "Storing CNN as 2D matrix \n \n";
      char **table2D;
      Solver.read_graph_from_file1(filename,N,&table2D);

      printf("\n");
      printf("Connectivity graph of 10 first nodes \n");
      printf("--------------------\n");
      for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
          printf("%c ", table2D[i][j]);
        }
        cout << "\n";
      }
        printf("[............] \n");
        printf("-------------------- \n");
      }


    if (stype == 2){
      cout << "Storing CNN as CRS \n \n";
      int *row_ptr , *col_idx;
      Solver.read_graph_from_file2(filename, N, &row_ptr, &col_idx);


      printf("\n");
      printf("CRS storage of 5 nodes \n");
      printf("--------------------\n");
      for (int i = 0; i < 370; i++){
          printf("%d ", col_idx[i]);
        }
        printf("[............] \n");
        printf("-------------------- \n");

        printf("\n");
        for (int i = 0; i < 10; i++){
            printf("%d ", row_ptr[i]);
          }
        printf("\n");
    }

  }

  if (exercise == 0 || exercise > 3){ // || is the same as or
    cout << "Wrong usage, choose correct task number \n";
    return 1;
  }
  return 0;
}
