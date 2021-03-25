#include "shared_nn.hpp"
using namespace std;

void swap(int *a, int *b){
       int t =*a; *a=*b; *b=t;
}

void quicksort(int *arr,int beg, int end){ // pivots around beginning index
  if (end > beg + 1) {
    int piv = (arr)[beg], l = beg + 1, r = end;
    while (l < r) {
      if ((arr)[l] <= piv)
        l++;
      else
        swap(&(arr)[l], &(arr)[--r]);
    }
    swap(&(arr)[--l], (&(arr)[beg]));
    quicksort(arr, beg, l);
    quicksort(arr, r, end);
  }
}

void Shared_NN::read_graph_from_file2 (char *filename, int *N, int **row_ptr,int **col_idx){
  /* Function which reads of a txt file and stores information in row_ptr and col_idx.
  txt-file has ascii format
  "
  # Undirected graph: facebook_combined.txt
  # A connectivity graph representing social circles from Facebook
  # Nodes: 5 Edges: 8
  # FromNodeId    ToNodeId
  0 1
  0 2
  [...]"

  Inputs are all pointers
  - filename of txt file.
  - N: number of nodes
  - arrays for connectivity graphs
      - row_ptr: number of accumulative connections (length nodes + 1)
      - col_idx: in range row_ptr[i],row_ptr[i+1]-1 lists nodes connected
        to node i (length 2*connections) (connections = edges)
  */

  FILE *asciifile; // pointer to a file stream
  string dir = "./data/";
  dir.append(filename);
  dir.append(".txt");
  cout << "Reading from directory:" + dir + "\n";

  const char * dirchar = dir.c_str();   // string to const char * for input
  asciifile = fopen(dirchar,"r"); // open the right folder and name, read rb

  if (asciifile == NULL){
    perror("Error opening file"); // print error if NULL
    }
  else {
    // read line by line for all edges,
    //must check if node i and j (!= i) has an edge/is connected
    int nodes, edges;
    char line [100];
    fgets(line,sizeof(line),asciifile); fgets(line,sizeof(line),asciifile);

    // get nodes and edges
    fgets(line,sizeof(line),asciifile);
    sscanf(line,"%*s %*s %d %*s %d", &nodes,&edges); // get wanted data attributes
    fgets(line,sizeof(line),asciifile); // jump over this line
    printf("Attributes of SNN data || Nodes:%d | Edges:%d \n",nodes, edges);

    *N = nodes;
    // Allocate dynamic memory by dereferencing
    int *sum = (int *) calloc(nodes, sizeof(*sum)); // dyn. allocate
    int *indx = (int *) calloc(nodes, sizeof(*indx)); // dyn. allocate
    int *from = (int *) calloc(edges+1, sizeof(*from)); // dyn. allocate
    int *to = (int *) calloc(edges+1, sizeof(*to)); // dyn. allocate
    *row_ptr = (int *) calloc((*N+1), sizeof(*row_ptr)); // dyn. allocate
    *col_idx = (int *) calloc(2*edges, sizeof(*col_idx)); // dyn. allocate

  // loop over lines
    int from_node,to_node;
    int assigned;
    int edge = 1;
    size_t idx = 0;

    while(idx <= edges){
      // read off nodes to from
      fgets(line,sizeof(line),asciifile);
      assigned = sscanf(line,"%d %d", &from_node, &to_node);
      if (from_node != to_node and assigned == 2){// exception handling
        //use two 1D from_node and to_node arrays
        from[idx] = from_node;
        to[idx] = to_node;
        idx += 1;
        }
      }

      // loop over edges
      for (int i = 0; i < edges;i++){
        sum[from[i]+1] += 1;
        sum[to[i]+1] += 1;
      }

      // fill in row_ptr
      int cum_sum = 0;
      for (int i = 0; i < nodes;i++){
          cum_sum += sum[i+1];
          sum[i+1] += sum[i];
          (*row_ptr)[i+1] = cum_sum;
      }
      free(sum);

      // fill in for col_idx
      for (int i = 0; i < edges;i++){
        (*col_idx)[(*row_ptr)[from[i]]+indx[from[i]]] = to[i];
        (*col_idx)[(*row_ptr)[to[i]] + indx[to[i]]] = from[i];
        indx[from[i]] += 1;
        indx[to[i]] += 1;
      }
      free(to);
      free(from);

      // then sort for each from node using quicksort
      int beg, end;
      for (int i = 0; i < nodes;i++){
          // sort
          beg = (*row_ptr)[i];
          end = (*row_ptr)[i+1];
          quicksort(*col_idx,beg,end);
        }
      }

  fclose(asciifile);
};
