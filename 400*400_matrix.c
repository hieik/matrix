#include<mpi.h>
#include<stdio.h>

#define NRA 400
#define NCA 400
#define NCB 400
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
               
                 if(extra_number == 0)
                     row = i * base_number;
                 else
                     row = extra_number * (base_number + 1) + (i - extra_number);
             }
             else
             {
                 length = NRA * (base_number + 1);
                 row = i * (base_number + 1);
             }
             MPI_Send(matrix_a + row, length , MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
             MPI_Send(matrix_b, NCA*NCB, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
        }
        for (i = 0; i < real_number; i++) 
        {
            for (j = 0; j < NCA; j++)
            {
                matrix_c[i][j] = 0;
                for (k = 0; k < NCA; k++)
                {
                    matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j]; 
                }
            }
        }
        for (i = 1; i < size; i++)
        {
            if (i > extra_number - 1)
            {
                length = NRA * base_number;
                if (extra_number == 0)
                    row = i * base_number;
                else
                    row = extra_number * (base_number + 1) + (i - extra_number);
            }
            else
            {
                length = NRA * (base_number + 1);
                row = i * (base_number + 1);
            }
            MPI_Recv(matrix_c + row, length, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status);
        } 
        endtime = MPI_Wtime();
        for (i = 0; i < NRA; i++) 
        {
            for (j = 0; j < NCB; j++) 
            {
                printf("matrix[%d][%d]: %f\n ", i, j, matrix_c[i][j]);
            }
        }
        printf("time = %f\n", endtime - starttime);
    }        
    if (my_rank != 0) 
    {
        int length = 0;
        int row;
        int row_number;
        int i, j, k;
        double matrix_a[NCA][NCA], matrix_b[NCA][NCB], matrix_c[NCB][NCB];
        if (my_rank > extra_number - 1) 
        {
            length = NRA * base_number;
            row_number = base_number;
            if (extra_number == 0) 
                row = my_rank * base_number;
            else
                row = extra_number * (base_number + 1) + (my_rank - extra_number);
        }
        else
        {
            length = NRA * (base_number + 1);
            row_number = base_number + 1;
            row = my_rank * (base_number + 1);
        }
        MPI_Recv(matrix_a + row, length, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(matrix_b, NCA*NCB, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        for (i = row; i < row + row_number; i++)
        {
            for (j = 0; j < NCB; j++)
            {
                matrix_c[i][j] = 0;
                for (k = 0; k < NCB; k++)
                {
                    matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
                }
            }
        }
        MPI_Send(matrix_c + row, length, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
        
    }
    MPI_Finalize();
}
  
