#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
// #include <time.h>
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


    FILE *f = fopen(input, "r");
    if (f == NULL)
    {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }

    // printf("going well\n");

    double **A;
    A = (double **)malloc(sizeof(double*) * size);

    for (int i = 0; i < size; i++){
        A[i] = (double *)malloc(size * sizeof(double));
    }

    // double inp[size][size];
    // printf("going well 2\n");

    double num;
    // while (fscanf(file, "%d", &num) > 0)
    // {
    //     integers[i] = num;
    //     i++;
    // }

    for(int i = 0; i < size; i++){
        // printf("going well loop\n");
        for(int j = 0; j < size; j++){
            fscanf(f, "%lf", &num);
            // printf("Scanned int: %f\n", num);
            A[i][j] = num;
        }
    }
    // printf("going well 3\n");

    fclose(f);

    // printf("going well 4\n");


    double **L;
    L = (double **)malloc(sizeof(double *) * size);

    for (int i = 0; i < size; i++){
        L[i] = (double *)malloc(size * sizeof(double));
    }
    // double L[size][size];
    // double U[size][size];
    double **U;
    U = (double **)malloc(sizeof(double *) * size);

    for (int i = 0; i < size; i++){
        U[i] = (double *)malloc(size * sizeof(double));
    }

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            L[i][j] = 0;
            U[i][j] = 0;
        }
    }

    //////////////////////////////////////////////////////////
    // printf("str0\n");
    ///////////////////////////////////////////////////////////

    int i, j, k;
    double sum = 0;
    for (i = 0; i < size; i++)
    {
        U[i][i] = 1;
        // All diagonal entries 1 in U
    }

    MPI_Init(&argc, &argv);
    int t, pid;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &t);
    for(j = 0; j < size; j++){
        for(i = j; i < size; i++){
            if(i%t == pid){
                sum = 0;
                for (k = 0; k < j; k++) {
                    sum = sum + L[i][k] * U[k][j];	
                }
                L[i][j] = A[i][j] - sum;
                //printf("%0.3lf \n %d %d ",sum, i,j);
            }
            MPI_Bcast(&L[i][j], 1, MPI_DOUBLE, i%t, MPI_COMM_WORLD);
        }


        for(i = j; i < size; i++){
            if(i%t == pid){
                sum = 0;
                for(k = 0; k < j; k++) {
                    sum = sum + L[j][k] * U[k][i];
                }
                if (L[j][j] == 0) {				
                    exit(0);
                }
                U[j][i] = (A[j][i] - sum) / L[j][j];
            }
            MPI_Bcast(&U[j][i], 1, MPI_DOUBLE, i%t, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize(); 
    /////////////////////////////////////////////////////
    // printf(".......................................................................\n");
    // for (int i = 0; i < size; i++)
    // {
    //     for (int j = 0; j < size; j++)
    //     {
    //         // fscanf(f, "%d", &num);
    //         printf("%f\n", A[i][j]);
    //         // inp[i][j] = num;
    //     }
    // }
    // printf(".......................................................................\n");
    // char out_4_A[] = "out_4_A.txt";
    char out_4_L[100];
    sprintf(out_4_L, "output_L_4_%d.txt", t);
    char out_4_U[100];
    sprintf(out_4_U, "output_U_4_%d.txt", t);

    

    // write_output(out_4_A, A, size);
    write_output(out_4_L, L, size);
    write_output(out_4_U, U, size);
    //////////////////////////////////////////////////////////

    
    return 0;
}