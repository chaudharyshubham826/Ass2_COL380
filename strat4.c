#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <mpi.h>


void write_output(char fname[], double** arr, int n ){
	FILE *f = fopen(fname, "w");
	for( int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			fprintf(f, "%0.12f ", arr[i][j]);
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

int main(int argc, char *argv[]) {
    if(argc != 3){  
      printf("Pass 2 arguments. <n> <input_file>\n");  
    }

    int n = atoi(argv[1]);
    char* input_file_name = argv[2];
    
    // Unique rank for this process
    int rank;

    // Total number of ranks
    int size;

    // Initializes the MPI execution environment
    MPI_Init(&argc, &argv);

    // Get the rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the total number ranks in this communicator
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double **A;
    double **L;
    double **U_trans;
    A = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        A[i] = malloc(n * sizeof(double));
    }
    L = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        L[i] = malloc(n * sizeof(double));
        memset(L[i], 0, n * sizeof(int));
    }
    U_trans = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        U_trans[i] = malloc(n * sizeof(double));
        memset(U_trans[i], 0, n * sizeof(int));
        U_trans[i][i] = 1;
    }
    if (rank == 0) {
        FILE *f = fopen(input_file_name, "r");
        if (!f) {
            return 1;
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fscanf(f, "%lf", &A[i][j]);
            }
            // Sending A to all ranks with one row at a time
            MPI_Bcast(A[i], n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        }
        fclose(f);
    } else {
        // Receiving A from rank 0
        for (int i = 0; i < n; i++) {
            MPI_Bcast(A[i], n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        }
    }

    assert(size > 0);
    // Distributing work statically
    int *rankof = malloc(n * sizeof(int));
    for (int i=0; i<n; i++) {
        rankof[i] = i%size;
    }
    int i, j, k;
    double sum = 0;
    for (j = 0; j < n; j++) {
        for (i = j; i < n; i++) {
            if (rankof[i] == rank) {
                // This work has to be done by current rank
                sum = 0;
                for (k = 0; k < j; k++) {
                    sum = sum + L[i][k] * U_trans[j][k];
                }
                L[i][j] = A[i][j] - sum;
                // Broadcast this value
                for (int dest=0; dest<size; dest++) {
                    if (dest != rank) {
                        MPI_Send(&(L[i][j]), 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
                    }
                }
            } 
        }
        // Make sure you fill col jth col of L for all ranks
        for (int i=j; i<n; i++) {
            if (rankof[i] != rank) {
                MPI_Recv(&(L[i][j]), 1, MPI_DOUBLE, rankof[i], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        for (i = j; i < n; i++) {
            if (rankof[i] == rank) {
                // This work has to be done by current rank
                sum = 0;
                for (k = 0; k < j; k++) {
                    sum = sum + L[j][k] * U_trans[i][k];
                }
                if (L[j][j] == 0) {
                    exit(0);
                }
                U_trans[i][j] = (A[j][i] - sum) / L[j][j];
                // Broadcast this value
                // MPI_Bcast(&(U[j][i]), 1, MPI_DOUBLE, rank, MPI_COMM_WORLD);
                for (int dest=0; dest<size; dest++) {
                    if (dest != rank) {
                        MPI_Send(&(U_trans[i][j]), 1, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
                    }
                }
            }
        }
        // Make sure you fill jth row for U_trans for all ranks
        for (int i=j; i<n; i++) {
            if (rankof[i] != rank) {
                MPI_Recv(&(U_trans[i][j]), 1, MPI_DOUBLE, rankof[i], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                // MPI_Bcast(&(U[j][i]), 1, MPI_DOUBLE, rankof[i], MPI_COMM_WORLD);
            }
        }
    }

    if (rank == size-1) {
        // Since U_trans = transform of original U. Thus I will transform U_trans so as to make it original U
        for (int i=0; i<n; i++) {
            for (int j=i+1; j<n; j++) {
                U_trans[i][j] = U_trans[j][i];
                U_trans[j][i] = 0;
            }
        }
        char file1[50];
	    char file2[50];
        sprintf(file1, "output_L_%d_%d.txt", 4, size);
	    sprintf(file2, "output_U_%d_%d.txt", 4, size);
        write_output(file1, L, n);
        write_output(file2, U_trans, n);
        
    }
    free(A);
    free(L);
    free(U_trans);
    free(rankof);

    
    MPI_Finalize();
    // No MPI calls after this

    return 0;
}