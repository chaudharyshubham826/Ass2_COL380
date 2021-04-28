#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <string.h>

void crout(double **A, double **L, double **U, int n)
{
    int i, j, k;
    double sum = 0;
    for (i = 0; i < n; i++)
    {
        U[i][i] = 1;
        // All diagonal entries 1 in U
    }
    for (j = 0; j < n; j++)
    {
        for (i = j; i < n; i++)
        {
            sum = 0;
            for (k = 0; k < j; k++)
            {
                sum = sum + L[i][k] * U[k][j];
            }
            L[i][j] = A[i][j] - sum;
        }
        for (i = j; i < n; i++)
        {
            sum = 0;
            for (k = 0; k < j; k++)
            {
                sum = sum + L[j][k] * U[k][i];
            }
            if (L[j][j] == 0)
            {
                exit(0);
            }
            U[j][i] = (A[j][i] - sum) / L[j][j];
        }
    }
}

void str_1(double const **A, double **L, double **U, int n)
{
    int i, j, k;
    double sum = 0;

    #pragma omp parallel for
    for (i = 0; i < n; i++)
    {
        U[i][i] = 1;
        // All diagonal entries 1 in U
    }
    for (j = 0; j < n; j++)
    {
        for (i = j; i < n; i++)
        {
            sum = 0;
            for (k = 0; k < j; k++)
            {
                sum = sum + L[i][k] * U[k][j];
            }
            L[i][j] = A[i][j] - sum;
        }
        for (i = j; i < n; i++)
        {
            sum = 0;
            for (k = 0; k < j; k++)
            {
                sum = sum + L[j][k] * U[k][i];
            }
            if (L[j][j] == 0)
            {
                exit(0);
            }
            U[j][i] = (A[j][i] - sum) / L[j][j];
        }
    }
}

void write_output(char fname[], double **arr, int n)
{
    printf("writing output\n");
    FILE *f = fopen(fname, "w");

    printf(".......................................................................\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            // fscanf(f, "%d", &num);
            printf("%f\n", arr[i][j]);
            // inp[i][j] = num;
        }
    }
    printf(".......................................................................\n");

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
    char input[10];
    strcpy(input, argv[2]);
    
    int thr_proc = atoi(argv[3]);
    int strategy = atoi(argv[4]);
    printf("%d\n", size);
    printf("%s\n", input);
    printf("%d\n", thr_proc);
    printf("%d\n", strategy);

    FILE *f = fopen(input, "r");
    if (f == NULL)
    {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }

    printf("going well\n");

    double **inp;
    inp = (double **)malloc(sizeof(double*) * size);

    for (int i = 0; i < size; i++){
        inp[i] = (double *)malloc(size * sizeof(double));
    }

    // double inp[size][size];
    printf("going well 2\n");

    double num;
    // while (fscanf(file, "%d", &num) > 0)
    // {
    //     integers[i] = num;
    //     i++;
    // }

    for(int i = 0; i < size; i++){
        printf("going well loop\n");
        for(int j = 0; j < size; j++){
            fscanf(f, "%d", &num);
            printf("Scanned int: %d\n", num);
            inp[i][j] = num;
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

    // double const **A = (double const **)inp;

    if(strategy == 0){
        // printf("str0\n");
        crout(inp, L, U, size);
        printf(".......................................................................\n");
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                // fscanf(f, "%d", &num);
                printf("%d\n", inp[i][j]);
                // inp[i][j] = num;
            }
        }
        printf(".......................................................................\n");
        char out_0_A[] = "out_0_A.txt";
        char out_0_L[] = "out_0_L.txt";
        char out_0_U[] = "out_0_U.txt";

        

        write_output(out_0_A, inp, size);
        write_output(out_0_L, L, size);
        write_output(out_0_U, U, size);
    }

    return 0;
}