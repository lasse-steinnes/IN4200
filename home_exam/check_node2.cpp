/*#include "shared_nn.hpp"

using namespace std;
void Shared_NN::check_node(int node_id, int tau, int N, int *row_ptr, int *col_idx, int *SNN_val){
*/
  // Clustering algo

  // tak in node_id and find out how many shared nearest neighbours it has
  // with nodes directly connected to it
/*
  int edges = row_ptr[N];
  int *cluster = (int *) malloc(N * sizeof(*cluster)); // Storing the nodes in cluster dyn. allocate connected to node_id
  int *been_here = (int *) malloc(N * sizeof(*been_here)); // Storing the nodes in cluster dyn. allocate
  int *data = (int *)malloc(edges* sizeof(*data)); // Storing the edges with SNN > tau
  int *from_n = (int *)malloc(edges* sizeof(*from_n)); // Storing from_nodes already explored
  int *to_n = (int *)malloc(edges* sizeof(*from_n)); // storing to nodes


  // initialize to zeroes
  for (int i = 0; i < N; i++){
      cluster[i] = 0;
      been_here[i] = 0;
    }

  for (int i = 0; i < edges; i++){
        data[i] = 0;
        to_n[i] = 0;
}


for (int i = 0; i < N; i++){
for (int j = row_ptr[i]; j < row_ptr[i+1]; j++){
  from_n[j]  = i;
  }
}

  // first access nodes
  cout << "node id: " << node_id << " tau:" << tau <<"\n";
  for (int j = row_ptr[node_id]; j < row_ptr[node_id+1]; j++){
      cout << " col_idx: "<< col_idx[j] << " " ; // access node
      cout << "SNN_val: " <<  SNN_val[j] << " "; // access how many shared neighbours
      cout << "\n";
}

  // store indices
  // first task: access nodes and check what nodes fullfills criteria, directly linked to
  // node_id

// go through all connected first -- avoid too many nested loops and ifs
for (int i = 0; i < edges; i++){
  if (tau <= SNN_val[i]){
      data[i] = 1; // store where criteria is fullfilled
      // (note, there are double mentions here)
  }
}

//begin from node_id, take heed from data
//loop through all after that and check if connected to nodes connected to node_id
//

// need code to figure out HOW they are connected to node id upholding tau = something


int edged_node, egded_node2, sum, new_, from;

for (int i = row_ptr[node_id]; i < row_ptr[node_id+1]; i++){
    been_here[node_id] = 1;// storing "from" nodes already explored
    if (data[i] == 1){   // access the relevant nodes
      edged_node = col_idx[i];
      cluster[edged_node] = 1; // stores nodes connected with 1 and upholding the criteria
      to_n[i] = 1; // inidices I've been on for col_idx, SNN_val and data, is one
      // need a while loop for each of these
    }
}

// run over all data with a while loop which
for (int i = 0; i < N; i++){
for (int j = row_ptr[i]; j < row_ptr[i+1]; j++){
  // acccess data j
  if (data[j] == 1 and edged_node !== col_idx[j]*to_n[j])
  }
}

/*
// loop through all nodes, check if connected to node_id in some way
  for (int i = row_ptr[node_id]; i < row_ptr[node_id+1]; i++){
      if (data[i] == 1){   // access the relevant nodes
        edged_node = col_idx[i];
        cluster[edged_node] = 1; // access node
        to_n[i] = 1; // inidices I've been on for col_idx, SNN_val and data, is one
        been_here[node_id] = 1;// storing "from" nodes already explored
        // need a while loop for each of these
        while (cluster[edged_node] = 1){ // so we know it is connected to node_id
          for (int j = row_ptr[edged_node]; j < row_ptr[edged_node+1]; j++){
            edged_node2 = col_idx[j];
            if (data[j] == 1 and been_here[edged_node2] == 0){
                cluster[edged_node2] = 1;
                been_here[egded_node2] = 1;
              }
            }
          edged_node = edged_node2
      }
}
}};
*/
/*
        while (end_here !== 1){
            for (int j = row_ptr[edged_node]; j < row_ptr[edged_node+1]; j++){
              edged_node2 = col_idx[j];
              if (data[j] == 1 and been_here[edged_node2] == 0){
                  cluster[edged_node2] = 1;
                  been_here[egded_node2] = 1;

                for (int k = row_ptr[edged_node2]; k < row_ptr[edged_node2+1]; k++){
                    if (data[k] == 1 and been_here[edged_node2] == 0){
                      cluster[edged_node2] = 1;
                      been_here[egded_node2] = 1;
                    }
              }
              }
        }
      }

  */
/* // sweep through the entire shit
      int idx,z,z2,to_n,shared_node;
      for (int k = 0; k < N ;k++){
        z = row_ptr[k];
        z2 = row_ptr[k+1];
        for (int m = z; m < z2;m++){// for each to_n
          to_n = col_idx[m];
          for (int j = z; j < z2; j++){
            shared_node = col_idx[j]; // looping over to nodes belonging to from_n
            for (int i = row_ptr[to_n]; i < row_ptr[to_n+1];i++){
              if (col_idx[i] == shared_node and SNN_val[j] >= tau and been_here[to_n] == 0){ // checking if tonodes is shared
                cluster[to_n] = 1; // add to compact array  i
                been_here[to_n] = 1;
            }
          }
        }
      }
    }
*/
/*
for (int i = 0; i < edges; i++){
          cout << data[i] << " ";
      }
cout << "\n";

for (int i = 0; i < N; i++){
          cout << "node"<< i << " "  << cluster[i] << ";";
      }
cout << "\n";
// then check if nodes fullfills tau criteria.

//then check if those nodes have other connections also fullilling the criteria n
*/
//};
