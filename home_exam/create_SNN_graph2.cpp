#include "shared_nn.hpp"

using namespace std;
void Shared_NN::create_SNN_graph2(int N, int *row_ptr, int *col_idx, int **SNN_val){

  // allocate SNN_val with dims 1*(2*egdes)
  int edges = row_ptr[N]; // same number of elements as in col_idx
  *SNN_val = (int *) calloc(edges, sizeof(*SNN_val)); // dyn. allocate
  // be careful with calloc and paralellization
  // col idx compares to compact
  //  row ptr compares to len

  int idx,z,z2,to_n,shared_node;
  for (int k = 0; k < N ;k++){
    z = row_ptr[k];
    z2 = row_ptr[k+1];
    //idx = 0;
    //from_n = compact[z*cols]; // the two we will check if having shared nodes
    for (int m = z; m < z2;m++){// for each to_n
      to_n = col_idx[m];
      //idx++;
      for (int j = z; j < z2; j++){
        shared_node = col_idx[j]; // looping over to nodes belonging to from_n
        for (int i = row_ptr[to_n]; i < row_ptr[to_n+1];i++){
          if (col_idx[i] == shared_node){ // checking if tonodes is shared
            (*SNN_val)[j] += 1; // add to compact array  indexing: z+idx-1
        }
      }
    }
  }
}


};
