#include "shared_nn.hpp"

using namespace std;
void Shared_NN::create_SNN_graph1(int N, char **table2D, int ***SNN_table){
  /*Creates a 2D SNN graph

  Input:
  - N: number of nodes
  - table 2D: a connectivity "grap"
  - SNN_table: 2D SNN graph to be allocated
  */

  // -----------------------------------------
   // allocate memory and objects
   //-----------------------------------------

  // allocate memory for SNN_table
  *SNN_table = (int**) malloc(N * sizeof *SNN_table); // dyn. allocate rows
  for (size_t i = 0; i < N; i++) { // dyn. allocate cols
      (*SNN_table)[i] = (int*) malloc(N * sizeof (*SNN_table)[0]);
  }

  // allocate zeroes // with parallel, but fix that to last
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
      (*SNN_table)[i][j] = 0;
    }
  }

  // compact holds the node connection (to_node)
  int *compact = (int *) malloc((N*N) * sizeof(*compact)); // dyn. allocate
  // len holds the information (from_node, in form  an index list)
  int *len = (int *) calloc(N+1, sizeof(*len)); // dyn. allocate
  // basically len corresponds to row_ptr and
  // compact refers to col_idx, but we don't know number of edges, so
  // allocate memory for the largest possible dimension N*N (not 2*edges)


  // Store information in compact notation
  // --> avoid nested if statements and 0 hits
  // when counting SNNs
  int sum_ones = 0;
  int ind = 0;
  int idx;
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
        if (table2D[i][j] == '1'){
          compact[ind] = j;
          sum_ones+=1;
          ind += 1;
        }
      }
      len[i+1] = sum_ones;
    }

  // -----------------------------------------
   // Begin algorithm
   //-----------------------------------------
  //  Count number of shared neighbours, between direct neighbours i and j
  int to_n, shared_node, z,z2;
  for (int k = 0; k < N ;k++){ // loop over all nodes
    z = len[k];                // z indices range to access nodes egded
    z2 = len[k+1];            //  to node k in compact array
    for (int m = z; m < z2;m++){ // loop over indices to access edged node
      to_n = compact[m];
      for (int j = z; j < z2; j++){ // for each edged node belonging to k
        shared_node = compact[j]; // loop over other nodes connected to k
        for (int i = len[to_n]; i < len[to_n+1];i++){ // loop over nodes connected to edged node
          if (compact[i] == shared_node){ // checking if edged node and k has shared NNs
            (*SNN_table)[k][to_n] += 1; // Add to 2D table if shared (possibly sparse)
        }
      }
    }
  }
}
//end algo
  // -----------------------------------------------------
  }
