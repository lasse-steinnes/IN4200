// trying to compile MPI

#include <mpi.h> //MPI
#include <stdio.h> // printf

int main(void){

  MPI_Init(NULL,NULL);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size); // number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  printf("Hello World and rank %d \n", rank);


  MPI_Finalize();
  return 0;
}
