
/**
 * Matrix product with MPI/OpenMP
 * 
 * <p>
 *  This execution is a hybrid parallel execution
 * </p>
 * 
 * @date 19/09/2020
 * @author Jerome Dh
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>

#include "matrice-mpi.h"

/**
 * Fill matrix with the random values
 *
 * @param V - Matrix
 * @param M - x lenght
 * @param N - y lenght
 */
void matrix_fill(float V[][T2], int M, int N) {
    for(int i=0; i<M; i++) {
        for(int j=0; j<N; j++) {
            V[i][j] = (rand() % 2) + 1;
        }
    }
}

/**
 * Print matrix
 */ 
void matrix_print(float V[][T2], int M, int N) {

    for(int i=0; i<M; i++) {
        for(int j=0; j<N; j++) {
            printf("%.2lf, ", V[i][j]);
        }
    }
    printf("\n");
}


int main(int argc, char** argv) {

    srand(time(NULL));

    int rank, size, TAG = 200;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    float A[T1][T2];
    float B[T1][T2];
    float C[T1][T2];

    FILE *file_result = fopen(RESULT_FILENAME, "a");
    if(file_result == NULL){
        exit(EXIT_FAILURE);
    }

    // Processor clock time
    clock_t t1 = clock();
    printf("Processus MPI %d/%d..\n", rank+1, size);

    // Init all variables and then send to all others processes
    if(rank == 0)
    {
        // Fill the matrixes
        matrix_fill(A, T1, T2);
        matrix_fill(B, T1, T2);

        // Send to all others process
        for(int j = 1; j < size; j++)
        {
            MPI_Send(A, T1 * T2, MPI_FLOAT, j, TAG, MPI_COMM_WORLD);
            MPI_Send(B, T1 * T2, MPI_FLOAT, j, TAG + 1, MPI_COMM_WORLD);
        }
    }
    else
    {
        // Receive from master
        MPI_Recv(A, T1 * T2, MPI_FLOAT, 0, TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(B, T1 * T2, MPI_FLOAT, 0, TAG + 1, MPI_COMM_WORLD, &status);
    }

    // Product of A * B
    int i, j;
    #pragma omp parallel for
    for(i = rank * T1/size; i < (rank+1) * T1/size; i++) 
    {
        for(j = 0; j<T2; j++) {
            C[i][j] = 0;
            for(int k=0; k<T2; k++) {
                C[i][j] = C[i][j] + (A[i][k] * B[k][j]);
            }
            // printf("C[%d][%d]=%.2f, ", i, j, C[i][j]);
            fprintf(file_result, "C[%d,%d]=%.2f\n", i, j, C[i][j]);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // End of computing
    if(rank == 0)
    {
        clock_t t2 = clock();
        double diff_clock = ((double)t2 - (double)t1) / CLOCKS_PER_SEC;
        printf("Fin de calcul !\nTemps Processeur ecoule: %lf sec\n", diff_clock);

        // Write in file
        FILE *file_out = fopen(OUT_FILENAME, "w");
        if(file_result == NULL){
            exit(EXIT_FAILURE);
        }

        fprintf(file_out, "%.5lf", diff_clock);
        fclose(file_out);

    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}