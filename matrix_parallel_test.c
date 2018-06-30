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
    double starttime, endtime;
    int base_number;
    int extra_number;
    int real_number;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    base_number = NCA / size;
    extra_number = NCA - base_number * size;
  
    if (my_rank == 0) 
    {
        int i, j, k;
        int row = 0;
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
        starttime = MPI_Wtime();
        if (extra_number > 0)
            real_number = base_number + 1;
        else
            real_number = base_number;
        int length = 0;
          
        for (i = 1; i < size; i++)  
        {
             if (i > extra_number - 1)
             {
                 length = NRA * base_number;
                 row += base_number;
             }
             else
             {
                 length = NRA * (base_number + 1);
                 row += base_number + 1;
             }
             MPI_Send(matrix_a + row, length , MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
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
        for (i = 1; i < NCA; i++)
        {
            MPI_Recv(matrix_c + i, NCB, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status);
        } 
        endtime = MPI_Wtime();
        for (i = 0; i < NRA; i++) 
        {
            for (j = 0; j < NCB; j++) 
            {
                printf("matrix[%d][%d]: %f\nb ", i, j, matrix_c[i][j]);
            }
    
        }
        printf("time = %f\n", endtime - starttime);
    }        
    if (my_rank != 0) 
    {
        int length = 0;
        int row;
        int row_number;
        int i, j;
        double vector_a[NCA], matrix_b[NCA][NCB], vector_c[NCB];
        if (my_rank > extra_number - 1) 
        {
            length = NRA * base_number;
            row_number = base_number;
            row = extra_number * base_number + (my_rank - extra_number);
        }
        else
        {
            length = NRA * (base_number + 1);
            row_number = base_number + 1;
            row = my_rank * (base_number + 1);
        }
        printf("processor%d %d %d\n", my_rank, row_number, row);
        MPI_Recv(vector_a, length, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(matrix_b, NCA*NCB, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        for (i = 0; i < NCA; i++)
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
  
