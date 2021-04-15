#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>



void crout(double const **A, double **L, double **U, int n)
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
    char input[] = argv[2];
    int thr_proc = atoi(argv[3]);
    int strategy = atoi(argv[4]);

    if(strategy == 0){
        
    }

    return 0;
}