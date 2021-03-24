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

    int *N; int nodes = 5; N = &nodes; //4039
    char *filename = "face_x"; //  "facebook_combined";
    //int *N; int nodes = 4039; N = &nodes; //4039
    //char *filename = "facebook_combined"; //  "facebook_combined";
    Shared_NN Solver;

    if (stype == 1){
      cout << "Storing CNN as 2D matrix \n \n";
      char **table2D;
      Solver.read_graph_from_file1(filename,N,&table2D);

      printf("\n");
      printf("Connectivity graph of 10 first nodes \n");
      printf("--------------------\n");
      for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
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

        printf("\n");
        printf("SNN graph of 10 first nodes \n");
        printf("--------------------\n");
        for (int i = 0; i < 5; i++){
          for (int j = 0; j < 5; j++){
            printf("%d ", SNN_table[i][j]);
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
      printf("CRS storage sample \n");
      printf("------------------------------------------\n");
      printf("col_idx: ");
      for (int i = 0; i < 16; i++){
          printf("%d ", col_idx[i]);
        }
        printf("[...] \n");
        printf("row_ptr: ");
        for (int i = 0; i < 6; i++){
            printf("%d ", row_ptr[i]);
          }
        printf("[...] \n");
        printf("------------------------------------------\n");

        // create SNN graph type 2
        printf("\n");
        printf("Creating CRS SNN graph:\n");
        int *SNN_val;
        Solver.create_SNN_graph2(*N,row_ptr,col_idx, &SNN_val);

        // printf("\n");
        printf("SNN graph for 10 first nodes \n");
        printf("------------------------------------------\n");
          for (int i = 0;  i < 16;i++){
            printf("%d ", SNN_val[i]);

          }
        printf("[...] \n");
        printf("------------------------------------------\n");

        // checking if node is within cluster, and printing other nodes
        printf("Investigate existence of cluster\n");
        int node_id = 0;
        int tau = 2;
        Solver.check_node(node_id,tau, *N,row_ptr, col_idx,SNN_val);
    }

  if (stype == 0 || stype > 2){ // || is the same as or
   cout << "Wrong usage, choose correct task option (1/2)!\n";
   return 1;
    }

  return 0;
}
