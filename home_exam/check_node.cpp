#include "shared_nn.hpp"

using namespace std;
void Shared_NN::check_node(int node_id, int tau, int N, int *row_ptr, int *col_idx, int *SNN_val){
  // Clustering algo - Breadth-first

   // allocate memory
  int *cluster = (int *) malloc(N * sizeof(*cluster)); // Storing the nodes in cluster dyn. allocate connected to node_id
                                                      // where 1 means part of cluster
  int *queue = (int *) malloc(N * sizeof(*queue)); // Storing the nodes in cluster dyn. allocate

  // tak in node_id and find out how many shared nearest neighbours it has
  // with nodes directly connected to it
  // initialize cluster
  // know that no nodes hold negative values

  // initialize to zeroes and -1
  for (int i = 0; i < N; i++){
      cluster[i] = 0;
      queue[i] = -1;
    }

  queue[0] = node_id; // put given node as search key
  cluster[node_id] = 1; // store node_id to cluster
  bool in_queue = true;
  int discovered_node;
  int q;
  while (in_queue == true){
      int i = 0;
      int k = 0;
    // enter the adjacent nodes and check if they uphold tau
      while(queue[i] != -1){
        //cout << "q " << queue[i];
        // access the node in queue
        q = queue[i];
        queue[i] = -1; // "delete" the node in queue[i]

        cout << "check nodes edged to " << q << "\n";
        for (int j = row_ptr[q]; j < row_ptr[q+1]; j++){
          discovered_node = col_idx[j];
          if (SNN_val[j] >= tau and cluster[discovered_node] == 0){
            cluster[discovered_node] = 1; // storing nodes belonging to cluster
            queue[k] = discovered_node; // filling up next qeue with nodes, starting from index 0;
            cout << "add to queue " << discovered_node  << "\n";
            k++;
          }
        }
        cout << "q " << queue[0] << "\n";
        cout << "\n";
        i++;
    }
    // checking if there are any nodes in queue;
    if (queue[0] == -1){
      in_queue = false; // i.e. end while
    }
  }

  // print out nodes in cluster
  // initialize to zeroes and -1
  for (int i = 0; i < N; i++){
      if (cluster[i] == 1){
      printf("Node %d in cluster \n",i);
    }
  }

};
