#include "shared_nn.hpp"

using namespace std;
void Shared_NN::read_graph_from_file1(char *filename, int *N, char ***table2D){
  /* Function which reads of a txt file and stores information in table2D.
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
  - 2D table for connectivity graph of nearest neighbours NN (address &tabel2D)
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
    // Allocate dynamic memory by dereferencing (or use nodes from reading off)
    *table2D = (char**) malloc((*N) * sizeof *table2D); // dyn. allocate rows
    for (size_t i = 0; i < (*N); i++) { // dyn. allocate cols
        (*table2D)[i] = (char*) malloc((*N) * sizeof (*table2D)[0]);
    }

    // filling in zeroes with loop unrolling over columns (avoid strided access)
    int stride = 4;
    int remaind = (*N)%stride; // remainder after loop
    for (int i = 0; i < (*N); i++){
      for (int j = 0; j < (*N)-remaind; j+= stride){
        (*table2D)[i][j] = '0';
        (*table2D)[i][j+1] = '0';
        (*table2D)[i][j+2] = '0';
        (*table2D)[i][j+3] = '0';
      }
    }

  // fill in for the remainder
  // remainder calculation
  if (remaind != 0){
  for (size_t i = 0; i < *N; i++){ // rows
    for (size_t j = (*N)-remaind; j < (*N);j++){ // columns
        (*table2D)[i][j] = '0';
      }
    }
  }

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
  };
