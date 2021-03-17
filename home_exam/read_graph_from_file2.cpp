#include "shared_nn.hpp"

void Shared_NN::read_graph_from_file2 (char *filename, int *N, int **row_ptr,int **col_idx){
  /*
  inputs are all pointers
  - filename of txt file.
  - N: number of nodes
  - arrays for connectivity graphs
      - row_ptr: number of accumulative connections (length nodes + 1)
      - col_idx: in range row_ptr[i],row_ptr[i+1]-1 lists nodes connected
        to node i (length 2*connections)
  */

  FILE *asciifile; // pointer to a file stream

  // Allocate dynamic memory by dereferencing (or use nodes from reading off)

  *row_ptr = (int*) malloc((*N) * sizeof *row_ptr); // dyn. allocate
  *col_idx = (int*) malloc((*N) * sizeof *col_idx); // dyn. allocate
/*
  // filling in zeroes
  for (int i = 0; i < (*N); i++){
    for (int j = 0; j < (*N); j++){
      (*table2D)[i][j] = '0';
    }
  }


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

  // loop over lines
    int from_node,to_node;
    int assigned;
    int edge = 1;

    while(edge <= edges){
      // read off nodes to from
      fgets(line,sizeof(line),asciifile);
      assigned = sscanf(line,"%d %d", &from_node, &to_node);
      if (from_node != to_node and assigned == 2){// exception handling
        // Fill in connectivity graph for dereferended 2D matrix
        (*table2D)[from_node][to_node] = '1';
        (*table2D)[to_node][from_node] = '1';
      }
      edge += 1;
    }
  }
  fclose(asciifile);
  */

};
