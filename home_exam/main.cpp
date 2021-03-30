// main for the home exam problems
#include "shared_nn.hpp"
#include <iostream> // cin
#include <omp.h> // openmp
#include <time.h> // time


#define CLOCKS_TO_MS(t) (t)/((double )CLOCKS_PER_SEC)*1000

using namespace std;

int main(int argc, char const *argv[]){

      int N;
      //char *filename = "face_x";
      //int x = 5;

      char *filename = "facebook_combined"; //  "facebook_combined";
      int x = 10;

    // read off text file
    int stype;
    cout << "Choose storage type 2D/CRS (1/2): ";
    cin >> stype;


    Shared_NN Solver; // create instance
    clock_t begin_read,end_read, begin_graph, end_graph;

    if (stype == 1){
      cout << "Storing CNN as 2D matrix \n \n";
      char **table2D;
      begin_read = clock();
      Solver.read_graph_from_file1(filename,&N,&table2D);
      end_read = clock() - begin_read;

      printf("\n");
      printf("Connectivity graph of %d first nodes \n",x);
      printf("--------------------\n");
      for (int i = 0; i < x; i++){
        for (int j = 0; j < x; j++){
          printf("%c ", table2D[i][j]);
        }
        cout << "\n";
      }
        printf("[............] \n");
        printf("-------------------- \n");

        //create 2D SNN graph
        printf("\n");
        printf("Creating 2D SNN graph:\n");
        printf("Max threads available for work: %d \n", omp_get_max_threads());
        int **SNN_table;
        begin_graph = clock();
        Solver.create_SNN_graph1(N,table2D,&SNN_table);
        end_graph = clock() - begin_graph;

        printf("\n");
        printf("SNN graph of %d first nodes \n",x);
        printf("--------------------\n");
        for (int i = 0; i < x; i++){
          for (int j = 0; j < x; j++){
            printf("%d ", SNN_table[i][j]);
          }
          cout << "\n";
        }
          printf("[............] \n");
          printf("-------------------- \n");

          //deallocate
          free(table2D);
          free(SNN_table);

          // time estimates
          printf("\n");
          printf("Time spent...\n");
          printf("Reading from file: %.2f ms\n", CLOCKS_TO_MS(end_read));
          printf("Creating SNN_graph: %.2f ms \n", CLOCKS_TO_MS(end_graph));
          printf("Total time: %.2f ms \n", CLOCKS_TO_MS(end_read+end_graph));

      }


    if (stype == 2){
      cout << "Storing CNN as CRS \n \n";
      int *row_ptr , *col_idx;

      begin_read = clock();
      Solver.read_graph_from_file2(filename, &N, &row_ptr, &col_idx);
      end_read = clock() - begin_read;

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
        printf("Max threads available for work: %d \n", omp_get_max_threads());
        int *SNN_val;
        begin_graph = clock();
        Solver.create_SNN_graph2(N,row_ptr,col_idx, &SNN_val);
        end_graph = clock() - begin_graph;

        printf("\n");
        printf("SNN graph for first nodes \n");
        printf("------------------------------------------\n");
          for (int i = 0;  i < 16;i++){
            printf("%d ", SNN_val[i]);

          }
        printf("[...] \n");
        printf("------------------------------------------\n");
        printf("\n");

        //user input search key and treshold:
        printf("Investigate existence of cluster\n");
        printf("\n");
        int node_id,tau;
        cout << "Choose search key (node_id) [0-"<< N-1 <<"], (int): ";
        cin >> node_id;

        // exception handling
        // Note: Assuming that node_id between 0 to N-1;
        if (node_id >= N){
         cout << "Wrong usage, choose search key node_id < N!\n";
         return 1;
          }

        cout << "Choose threshold for cluster, tau (int): ";
        cin >> tau;

        // checking if node is within cluster, and printing other nodes
        clock_t begin_cluster, end_cluster;
        printf("------------------------------------------\n");
        begin_cluster = clock();
        Solver.check_node(node_id,tau, N,row_ptr, col_idx,SNN_val);
        end_cluster = clock() - begin_cluster;
        printf("------------------------------------------\n");

        //deallocate
        free(col_idx);
        free(row_ptr);
        free(SNN_val);

        // time estimates
        printf("\n");
        printf("Time spent...\n");
        printf("Reading from file: %.2f ms\n", CLOCKS_TO_MS(end_read));
        printf("Creating SNN_graph: %.2f ms \n", CLOCKS_TO_MS(end_graph));
        printf("Finding cluster: %.2f ms \n", CLOCKS_TO_MS(end_cluster));
        printf("Total time: %.2f ms \n", CLOCKS_TO_MS(end_read+end_graph+end_cluster));
    }

  // exception handling
  if (stype == 0 || stype > 2){ // || is the same as or
   cout << "Wrong usage, choose correct task option (1/2)!\n";
   return 1;
    }
  return 0;
}
