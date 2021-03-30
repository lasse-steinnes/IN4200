#include "shared_nn.hpp"

using namespace std;
void Shared_NN::create_SNN_graph2(int N, int *row_ptr, int *col_idx, int **SNN_val){
  /*Creates a 1D compact SNN graph

  Input:
  - N: number of nodes
  - row_ptr: Holds indices relating to a specific node
  - col_idx: Holds information about nodes connected to node i.
           Index range j [row_ptr[i],row_ptr[i]).
  - SNN_val: Connectivity graph To be allocated. Holds information about SNN similarity
  */

  // -----------------------------------------
   // allocate memory and objects
  // -----------------------------------------
  // allocate SNN_val with dims 1*(2*egdes)
  int edges = row_ptr[N]; // same number of elements as in col_idx in SNN_val
  *SNN_val = (int *) malloc(edges * sizeof(*SNN_val)); // dyn. allocate
  // be careful with calloc and paralellization

  // initialize
  int i;
  #ifdef _OPENMP
  #pragma omp parallel for
  #endif
  for (i = 0; i < N ;i++){ // loop over all nodes
     for (int j = row_ptr[i]; j < row_ptr[i+1];j++){
       (*SNN_val)[j] = 0;
  }
}


  // -----------------------------------------
   // Begin algorithm
   //-----------------------------------------
  //  Count number of shared neighbours, between direct neighbours
  int k,j, idx,z,z2,to_n,shared_node;
  #ifdef _OPENMP
  #pragma omp parallel for private(k)
  #endif
  for (k = 0; k < N ;k++){ // loop over all nodes
    z = row_ptr[k];           // z indices range to access nodes egded
    z2 = row_ptr[k+1];        //  to node k in compact array

    for (int m = z; m < z2;m++){ // loop over indices to access edged node
      to_n = col_idx[m];

      for (j = z; j < z2; j++){   // for each edged node belonging to k
        shared_node = col_idx[j];     // loop over other nodes connected to k

        #ifdef _OPENMP
        #pragma unroll(4)
        #endif
        for (int i = row_ptr[to_n]; i < row_ptr[to_n+1];i++){ // loop over nodes connected to edged node
          if (col_idx[i] == shared_node){ // checking if edged node and k has shared NNs
            (*SNN_val)[j] += 1; // add to compact array SNN_val
        }
      }
    }
  }
}
//end algo
// -----------------------------------------------------
};
