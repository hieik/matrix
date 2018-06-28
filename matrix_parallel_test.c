#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define NRA 4
#define NCA 4
#define NCB 4
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
        double vector_a[NCA], vector_b[NCB], vector_c[NCB];
        for (i = 0; i < NRA; i++)
        {
            for (j = 0; j < NCA; j++)
                matrix_a[i][j] = i + j;
        }

        for (j = 0; j < NCB; j++)
        {
            for (i = 0; i < NCA; i++)
                matrix_b[j][i] = i + j;

        }
        for (i = 1; i < size; i++)  
        {
             MPI_Send(matrix_a + i, NRA, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
             MPI_Send(matrix_b, NCA*NCB, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
        }
        for (i = 0; i < NCA; i++) 
        {
            matrix_c[0][i] = 0;
            for (j = 0; j < NCB; j++)
            {  
                matrix_c[0][i] += matrix_a[0][j] * matrix_b[i][j];  
            }
        }
        for (i = 1; i < size; i++) 
        {
            MPI_Recv(matrix_c + i, NCB, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status);
        } 
        for (i = 0; i < NRA; i++) 
        {
            for (j = 0; j < NCB; j++) 
            {
                printf("%f ", matrix_c[i][j]);
            }
            printf("\n");
    
        }
    }        
    if (my_rank != 0) 
    {
        int i, j;
        double vector_a[NCA], matrix_b[NCA][NCB], vector_c[NCB];
        MPI_Recv(vector_a, NRA, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(matrix_b, NCA*NCB, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        for (i = 0; i < NCA; i ++)
        {
            vector_c[i] = 0;
            for (j = 0; j < NCB; j++)
            {
                vector_c[i] += vector_a[j] * matrix_b[i][j];
            }
        }

        MPI_Send(vector_c, NCB, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
        
    }
    MPI_Finalize();
}
  
