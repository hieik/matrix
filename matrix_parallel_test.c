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
//       MPI_Barrier(MPI_COMM_WORLD);
//        int index = my_rank;
//        do{
//            matrix_c[my_rank][index] = 0;
//            for (i = 0; i < NCB; i++)
//            {
//                matrix_c[my_rank][index] += matrix_a[my_rank][i] * matrix_b[my_rank][i];
//            }
//            MPI_Send(vector_b, NCB, MPI_DOUBLE, my_rank + 1, tag, MPI_COMM_WORLD);
//            MPI_Recv(vector_b, NCB, MPI_DOUBLE, size - 1, tag, MPI_COMM_WORLD, &status);
//            index++; 
//            MPI_Barrier(MPI_COMM_WORLD);  
//        }while(index != size);
//
//       printf("breakloop? processor 0 %d", my_rank);
    }
    
            
    if (my_rank != 0) 
    {
        double vector_a[NCA], matrix_b[NCA][NCB], vector_c[NCB];
        MPI_Recv(vector_a, NRA, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(matrix_b, NCA*NCB, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        for (int i = 0; i < NCA; i ++)
        {
            vector_c[i] = 0;
            for (int j = 0; j < NCB; j++)
            {
                vector_c[i] += vector_a[j] * matrix_b[i][j];
            }
        }

        MPI_Send(vector_c, NCB, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
        
     }
//        for (int i = 0; i < NRA; i++) 
//        {
//            printf("processor:%d a  %f      \n", my_rank,vector_a[i]);
//        }
//        for (int i = 0; i < NCA; i++)
//        {
//            printf("processor:%d b  %f      \n", my_rank, vector_b[i]);
//        }
//        int index = my_rank;
//        do {
//          vector_c[index] = 0;
//            for (int i = 0; i < NCB; i++)
//            {
//                vector_c[index] += vector_a[i] * vector_b[i]; 
//            }
//            MPI_Send(vector_b, NCB, MPI_DOUBLE, (my_rank + 1) % size, tag, MPI_COMM_WORLD);
//            MPI_Recv(vector_b, NCB, MPI_DOUBLE, my_rank - 1, tag, MPI_COMM_WORLD, &status);
//            MPI_Barrier(MPI_COMM_WORLD);
//            index = (index + 1) % size;    
//        }while(index != my_rank - 1); 
//        for (int i = 0; i < NCB; i++)
//        {
//            printf("vector_c processor:%d    %f\n", my_rank, vector_c[i]);
//        }        


    MPI_Finalize();
}
  
