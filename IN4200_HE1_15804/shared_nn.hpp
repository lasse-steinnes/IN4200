#ifndef SHARED_NN_H
#define SHARED_NN_H
#include <iostream> // cout
#include <string.h> // string
#include <stdio.h> // sscanf,fgets


// class for the methods
class Shared_NN{
  /*
  A class containing the methods needed to
  1) read a file containing shared neighbouring nodes (SNN)
  --> read... methods, either by reading data as a) table b) 1D arrays
  2)create a SNN "graph"
  --> create... methods, by a) table or b) 1D arrays
  3) check if a node is within a cluster
    + output fellow nodes in same cluster
  */

  public:
    void read_graph_from_file1(char *filename, int *N, char ***table2D);
    void read_graph_from_file2 (char *filename, int *N, int **row_ptr, int **col_idx);
    void create_SNN_graph1(int N, char **table2D, int ***SNN_table);
    void create_SNN_graph2(int N, int *row_ptr, int *col_idx, int **SNN_val);
    void check_node(int node_id, int tau, int N, int *row_ptr, int *col_idx, int *SNN_val);
};

#endif
