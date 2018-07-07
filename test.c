#include<mpi.h>
#include<stdio.h>

int main(int argc, char* argv[])
{
    int my_rank;
    int size;
    
    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    printf("size =  %d", size);    
    MPI_Finalize();
}
