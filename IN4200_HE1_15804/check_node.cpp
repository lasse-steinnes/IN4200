#include "shared_nn.hpp"

using namespace std;
void Shared_NN::check_node(int node_id, int tau, int N, int *row_ptr, int *col_idx, int *SNN_val){
  /* Clustering algo with breadth-first as inspiration

  Input:
  - node_id: Search key.  Starting node of cluster search.
  - tau: Threshold of SNN (shared nearest neighbours similarity) for a
    node-pair to be in a cluster
  - row_ptr: Holds indices relating to a specific node
  - col_idx: Holds information about nodes connected to node i.
            Index range j [row_ptr[i],row_ptr[i]).
  - SNN_val: Holds information about SNN similarity
            Index range j [row_ptr[i],row_ptr[i]).
  */

  // -----------------------------------------
   // allocate memory and objects
   //-----------------------------------------
   // Storing the nodes in the cluster for search key (node_id)
   // where 1 means part of cluster, else 0s
  int *cluster = (int *) malloc(N * sizeof(*cluster));
  // Storing the nodes in cluster dyn. allocate
  // -1 means empty "slot",  assuming no nodes hold negative values
  int *queue = (int *) malloc(N * sizeof(*queue));

  // initialize to zeroes and -1, with loop unrolling
  for (int i = 0; i < N; i++){
      cluster[i] = 0;
      queue[i] = -1;
    }

  queue[0] = node_id; // put given node as search key
  cluster[node_id] = 1; // store node_id to cluster
  bool in_queue = true; // because first queue holds node_id
  int discovered_node; // placeholder to avoid accessing array unnecessary
  int q;               // placeholder for node in queue being worked on

  // -----------------------------------------
   // Begin algo
   //-----------------------------------------
  while (in_queue == true){
      int i = 0;
      int k = 0;
      //cout << "here" << "\n";
      // enter the adjacent nodes and check if they uphold tau
      while(queue[i] != -1){
        //cout << "q " << queue[i];
        // access the node in queue
        q = queue[i];
        queue[i] = -1; // "delete" the node in queue[i]
        //cout << " " << i << "\n";
        //cout << "check nodes edged to " << q << "\n";
        // check if nodes edged to node (q) in queue
        // is not stored in cluster and upholds the SNN criteria
        for (int j = row_ptr[q]; j < row_ptr[q+1]; j++){
          discovered_node = col_idx[j];
          if (SNN_val[j] >= tau and cluster[discovered_node] == 0){
            cluster[discovered_node] = 1; // storing nodes belonging to cluster
            queue[k] = discovered_node; // filling up next qeue with nodes, starting from index 0;
            //cout << "add to queue " << discovered_node  << "\n";
            k++;
          }
        }
        //cout << "q " << queue[0] << "\n";
        //cout << "\n";
        i++;
    }
    // checking if there are any nodes in queue
    // after going through queue[0] in the new cycle/depth;
    if (queue[0] == -1){
      in_queue = false; // i.e. end while
    }
  }
  //end algo
  // -----------------------------------------------------
  // print out nodes in cluster
  cout << "node id/search key: " << node_id << "  tau:" << tau <<  "\n";
  printf("------------------------------------------\n");
  cout << "In cluster with: \n";
  int stride = 4;
  int remaind = N%stride; // remainder after loop
  for (int i = 0; i < N; i+=stride){ // with unroll
      if (cluster[i] == 1 and i != node_id){
      printf("Node %d \n",i);
      }
      if (cluster[i+1] == 1 and i+1 != node_id){
      printf("Node %d \n",i+1);
      }
      if (cluster[i+2] == 1 and i+2 != node_id){
      printf("Node %d \n",i+2);
      }
      if (cluster[i+3] == 1 and i+3 != node_id){
      printf("Node %d \n",i+3);
      }
    }

    // print remainder
    if (remaind != 0){
      for (int i = N-remaind; i < N; i+=stride){
      if (cluster[i] == 1 and i != node_id){
      printf("Node %d \n",i);
        }
      }
    }
};
