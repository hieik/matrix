#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define NRA 5
#define NCA 5
#define NCB 5
int main(int argc, char* argv[])
{
    int my_rank;
    int size;
    int source;
    int dest;
    int tag = 0;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    clock_t begin,end;
    
    if (my_rank == 0) 
    {
        int i, j, k;
        double matrix_a[NRA][NCA],matrix_b[NCA][NCB], matrix_c[NRA][NCB];

        for (i = 0; i < NRA; i++)
        {
            for (j = 0; j < NCA; j++)
                matrix_a[i][j] = i + j;
        }

        for (i = 0; i < NCA; i++)
        {
            for (j = 0; j < NCA; j++)
                matrix_b[i][j] = i * j;

        }
        for (i = 0; i < NRA; i++)
            for (j = 0; j < NRA; j++)
                printf("%f\n", matrix_a[i][j]);

        printf("\n\n");
        
            MPI_Send(matrix_a+4, NRA, MPI_DOUBLE, 1, tag, MPI_COMM_WORLD);
    }
    
            
    if (my_rank != 0) 
    {
        double vector_a[NRA], vector_b[NRA], vector_c[NRA];
        MPI_Recv(vector_a, NRA, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        for (int i = 0; i < NRA; i++) 
        {
            printf("%f      \n", vector_a[i]);
        }   
    }

    MPI_Finalize();
}

