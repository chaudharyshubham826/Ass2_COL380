#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <string.h>





void write_output(char fname[], double **arr, int n)
{
    FILE *f = fopen(fname, "w");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fprintf(f, "%0.12f ", arr[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

int main(int argc, char* argv[]){

    int size = atoi(argv[1]);
    char input[100];
    strcpy(input, argv[2]);
    
    // int thr_proc = atoi(argv[3]);
    // int strategy = atoi(argv[4]);


    MPI_Init(&argc, &argv);
    int t, pid;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &t);


    double **A;
    A = (double **)malloc(sizeof(double*) * size);

    for (int i = 0; i < size; i++){
        A[i] = (double *)malloc(size * sizeof(double));
    }

    double **L;
    L = (double **)malloc(sizeof(double *) * size);

    double **U;
    U = (double **)malloc(sizeof(double *) * size);

    for (int i = 0; i < size; i++){
        L[i] = (double *)malloc(size * sizeof(double));
        memset(L[i], 0, size * sizeof(int));
        U[i] = (double *)malloc(size * sizeof(double));
        memset(U[i], 0, size * sizeof(int));
        U[i][i] = 1;
    }



    ////////////////////////

    if (pid == 0) {
        FILE *f = fopen(input, "r");
        if (f == NULL)
        {
            printf("Error! opening file");
            // Program exits if file pointer returns NULL.
            exit(1);
        }
        double num;

        for(int i = 0; i < size; i++){
        // printf("going well loop\n");
            for(int j = 0; j < size; j++){
                fscanf(f, "%lf", &num);
                // printf("Scanned int: %f\n", num);
                A[i][j] = num;
            }
            MPI_Bcast(A[i], size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        }
        
        fclose(f);
    } 
    else {
        for (int i = 0; i < size; i++) {
            MPI_Bcast(A[i], size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        }
    }


    //////////////////
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    
    int *rankof = malloc(size * sizeof(int));
    for (int i=0; i<size; i++) {
        rankof[i] = i%t;
    }
    int i, j, k;
    double sum = 0;
    for (j = 0; j < size; j++) {
        for (i = j; i < size; i++) {

            if (rankof[i] == pid) {
                
                sum = 0;
                for (k = 0; k < j; k++) {
                    sum = sum + L[i][k] * U[j][k];
                }
                L[i][j] = A[i][j] - sum;
                
                for (int dest=0; dest<t; dest++) {
                    if (dest != pid) {
                        MPI_Send(&(L[i][j]), 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
                    }
                }
            } 
        }

        
        for (int i=j; i<size; i++) {
            if (rankof[i] != pid) {
                MPI_Recv(&(L[i][j]), 1, MPI_DOUBLE, rankof[i], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }


        for (i = j; i < size; i++) {
            if (rankof[i] == pid) {
                
                sum = 0;
                for (k = 0; k < j; k++) {
                    sum = sum + L[j][k] * U[i][k];
                }
                if (L[j][j] == 0) {
                    exit(0);
                }
                U[i][j] = (A[j][i] - sum) / L[j][j];
                
                for (int dest=0; dest<t; dest++) {
                    if (dest != pid) {
                        MPI_Send(&(U[i][j]), 1, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
                    }
                }
            }
        }
        
        for (int i=j; i<size; i++) {
            if (rankof[i] != pid) {
                MPI_Recv(&(U[i][j]), 1, MPI_DOUBLE, rankof[i], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                
            }
        }
    }


    /////////////////////





    //////////////////////////////////////////////////////////////////////////

    if (pid == t-1) {
        
        for (int i=0; i<size; i++) {
            for (int j=i+1; j<size; j++) {
                U[i][j] = U[j][i];
                U[j][i] = 0;
            }
        }

        char out_4_L[100];
        sprintf(out_4_L, "output_L_4_%d.txt", t);
        char out_4_U[100];
        sprintf(out_4_U, "output_U_4_%d.txt", t);

        write_output(out_4_L, L, size);
        write_output(out_4_U, U, size);
    

        
    }
    free(A);
    free(L);
    free(U);
    free(rankof);

    
    MPI_Finalize();


    ///////////////////////////////////////////////////////////////////////////
    

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("For strategy: 4, thr/proc: %d, Size: %d, pragram took: %f\n", t, size, cpu_time_used);

    
    return 0;
}