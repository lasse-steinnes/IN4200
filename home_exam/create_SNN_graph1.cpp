#include "shared_nn.hpp"

using namespace std;
void Shared_NN::create_SNN_graph1(int N, char **table2D, int ***SNN_table){
  /*Creates an SNN graph
  Input:
  - N: number of nodes
  - table 2D: a connectivity "grap"
  - SNN_table: SNN graph to be allocated

  int x = character - '0';  // converting chars to integers (from ascii table)
  */

  // allocate memory for SNN_table
  *SNN_table = (int**) malloc(N * sizeof *SNN_table); // dyn. allocate rows
  for (size_t i = 0; i < N; i++) { // dyn. allocate cols
      (*SNN_table)[i] = (int*) malloc(N * sizeof (*SNN_table)[0]);
  }
  int cols = 2;
  int *compact = (int *) malloc((N*N*2) * sizeof(*compact)); // dyn. allocate
  int *len = (int *) calloc(N+1, sizeof(*len)); // dyn. allocate
  // Now count number of shared neighbours, between direct neighbours i and j
  // filling in zeroes // avoid looping over
  int sum_ones = 0; // make length of non-zeros in row i
  int ind = 0;
  int idx;
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
        if (table2D[i][j] == '1'){
          compact[ind*cols] = i;
          compact[ind*cols + 1] = j;
          sum_ones+=1;
          ind += 1;
        }
      }
      len[i+1] = sum_ones;
    }

    // test output

    for (int i = 345; i < 380; i++){
            cout << compact[i*cols] << " ";
            cout << compact[i*cols+1] << " ";
            cout << "\n";
        }
    cout << len[1] << "\n";


    // find the shard nearest neighbours


  }
