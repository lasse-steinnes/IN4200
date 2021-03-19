// main for the home exam problems
#include "shared_nn.hpp"
#include <iostream> // cin
#include <omp.h> // openmp
#include <time.h> // time


#define CLOCKS_TO_MS(t) (t)/((double )CLOCKS_PER_SEC)*1000

using namespace std;

int main(int argc, char const *argv[]){

    // read off text file
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

        //create 2D SNN graph
        printf("\n");
        printf("Creating 2D SNN graph:\n");
        int **SNN_table;
        Solver.create_SNN_graph1(*N,table2D,&SNN_table);
      }


    if (stype == 2){
      cout << "Storing CNN as CRS \n \n";
      int *row_ptr , *col_idx;
      Solver.read_graph_from_file2(filename, N, &row_ptr, &col_idx);


      printf("\n");
      printf("CRS storage sample \n");
      printf("------------------------------------------\n");
      printf("col_idx: ");
      for (int i = 0; i < 10; i++){
          printf("%d ", col_idx[i]);
        }
        printf("[...] \n");
        printf("row_ptr: ");
        for (int i = 0; i < 10; i++){
            printf("%d ", row_ptr[i]);
          }
        printf("[...] \n");
        printf("------------------------------------------\n");
    }


  if (stype == 0 || stype > 2){ // || is the same as or
   cout << "Wrong usage, choose correct task option (1/2)!\n";
   return 1;
    }

  return 0;
}
