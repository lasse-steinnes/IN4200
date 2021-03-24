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

  // allocate zeroes // with parallel, but fix that to last
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
      (*SNN_table)[i][j] = 0;
    }
  }

  //int cols = 2;
  int *compact = (int *) malloc((N*N) * sizeof(*compact)); // dyn. allocate
  int *len = (int *) calloc(N+1, sizeof(*len)); // dyn. allocate
  // compact holds the node connection (to_node)
  // len holds the information (from_node, in form  an index list)

  // Now count number of shared neighbours, between direct neighbours i and j
  // filling in zeroes // avoid looping over
  int sum_ones = 0; // make length of non-zeros in row i
  int ind = 0;
  int idx;
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
        if (table2D[i][j] == '1'){
          compact[ind] = j;
          //compact[ind*cols+1] = j;
          sum_ones+=1;
          ind += 1;
        }
      }
      len[i+1] = sum_ones;
    }

    /*
    // test output
    for (int i = 345; i < 380; i++){
            cout << compact[i] << " ";
            cout << "\n";
        }
    */
    // begins at len 1
    /*cout << "first node to " << compact[0*cols + 1] << "\n";
    cout << "first node from " << compact[0*cols + 0] << "\n";
    cout << "first index (row) (at first node from)" << len[1] <<  "\n";
    cout << "next index in for compact (corr to next node from)" << len[1] << "\n";
    cout << "last node to " << compact[(len[N]-1)*cols +1] << "\n";
    cout << "last node from" << compact[(len[N]-1)*cols] << "\n";
    cout << "len[N]-1 last index from " <<  len[N]-1 << "\n";
    */
    // find the shard nearest neighbours
    // avoid all the zeros and double ifs

    int to_n, from_n, shared_node, z,z2;
    for (int k = 0; k < N ;k++){
      z = len[k];
      z2 = len[k+1];
      //from_n = compact[z*cols]; // the two we will check if having shared nodes
      for (int m = z; m < z2;m++){// for each to_n
        to_n = compact[m];
        for (int j = z; j < z2; j++){
          shared_node = compact[j]; // looping over to nodes belonging to from_n
          for (int i = len[to_n]; i < len[to_n+1];i++){
            if (compact[i] == shared_node){ // checking if tonodes is shared
              (*SNN_table)[k][to_n] += 1; // sparse arrray
          }
        }
      }
    }
  }
  }
