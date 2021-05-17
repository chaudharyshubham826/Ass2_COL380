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




void str_1(double **A, double **L, double **U, int n, int nthreads)
{
    // int i, j, k;
    // double sum = 0;

    omp_set_num_threads(nthreads);
    #pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        U[i][i] = 1;
        // All diagonal entries 1 in U
    }


    for (int j = 0; j < n; j++)
    {
        // omp_set_num_threads(nthreads);
        #pragma omp parallel for
        for (int i = j; i < n; i++)
        {
            double sum = 0;

            
            for (int k = 0; k < j; k++)
            {
                // #pragma omp critical
                {sum = sum + L[i][k] * U[k][j];}
            }
            // #pragma omp barrier
            L[i][j] = A[i][j] - sum;
        }

        // omp_set_num_threads(nthreads);
        #pragma omp parallel for
        for (int i = j; i < n; i++)
        {
            double sum = 0;

            
            for (int k = 0; k < j; k++)
            {
                // #pragma omp critical
                {sum = sum + L[j][k] * U[k][i];}
            }
            // #pragma omp barrier
            if (L[j][j] == 0)
            {
                exit(0);
            }
            U[j][i] = (A[j][i] - sum) / L[j][j];
        }
    }
}

int* intervals(int m , int n){
    int x = n - m;
    int interval = x/16;
    int* arr = malloc (sizeof(int)*17);
    
    int var1 = 0;
    int var2 = m;

    arr[0] = m;
    for(int i = 0; i < 15; i++){
        var1 = var2 + interval;
        arr[i+1] = var1;
        var2 = var1;
    }
    arr[16] = n;

    return arr;
}



void str_2(double **A, double **L, double **U, int n, int nthreads)
{
    // int i, j, k;
    double sum = 0;

    omp_set_num_threads(nthreads);
    #pragma omp parallel sections 
    {
        #pragma omp section
        {
            for (int i = 0; i < n/16; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (int i = n/16; i < 2*n/16; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (int i = 2*n/16; i < 3*n/16; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (int i = 3*n/16; i < 4*n/16; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (int i = 4*n/16; i < 5*n/16; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (int i = 5*n/16; i < 6*n/16; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (int i = 6*n/16; i < 7*n/16; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (int i = 7*n/16; i < 8*n/16; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (int i = 8*n/16; i < 9*n/16; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (int i = 9*n/16; i < 10*n/16; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (int i = 10*n/16; i < 11*n/16; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (int i = 11*n/16; i < 12*n/16; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (int i = 12*n/16; i < 13*n/16; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (int i = 13*n/16; i < 14*n/16; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (int i = 14*n/16; i < 15*n/16; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (int i = 15*n/16; i < n; i++) {
                U[i][i] = 1;
            }
        }
    }



    for (int j = 0; j < n; j++) {

        int* inter = intervals(j, n);

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                for(int i = inter[0]; i < inter[1]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++){
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[1]; i < inter[2]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }
            #pragma omp section
            {
                for(int i = inter[2]; i < inter[3]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[3]; i < inter[4]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[4]; i < inter[5]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[5]; i < inter[6]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }
            #pragma omp section
            {
                for(int i = inter[6]; i < inter[7]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[7]; i < inter[8]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[8]; i < inter[9]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[9]; i < inter[10]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }
            #pragma omp section
            {
                for(int i = inter[10]; i < inter[11]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[11]; i < inter[12]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[12]; i < inter[13]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[13]; i < inter[14]; i++ ){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }
            #pragma omp section
            {
                for(int i = inter[14]; i < inter[15]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[15]; i < inter[16]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }
        }





        #pragma omp parallel sections
        {
            #pragma omp section
            {
                for(int i = inter[0]; i < inter[1]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++)
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

            #pragma omp section
            {
                for(int i = inter[1]; i < inter[2]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++)
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
            #pragma omp section
            {
                for(int i = inter[2]; i < inter[3]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++)
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

            #pragma omp section
            {
                for(int i = inter[3]; i < inter[4]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++)
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

            #pragma omp section
            {
                for(int i = inter[4]; i < inter[5]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++)
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

            #pragma omp section
            {
                for(int i = inter[5]; i < inter[6]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++)
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
            #pragma omp section
            {
                for(int i = inter[6]; i < inter[7]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++)
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

            #pragma omp section
            {
                for(int i = inter[7]; i < inter[8]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++)
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

            #pragma omp section
            {
                for(int i = inter[8]; i < inter[9]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++)
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

            #pragma omp section
            {
                for(int i = inter[9]; i < inter[10]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++)
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
            #pragma omp section
            {
                for(int i = inter[10]; i < inter[11]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++)
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

            #pragma omp section
            {
                for(int i = inter[11]; i < inter[12]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++)
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

            #pragma omp section
            {
                for(int i = inter[12]; i < inter[13]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++)
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

            #pragma omp section
            {
                for(int i = inter[13]; i < inter[14]; i++ ){
                    double sum = 0;
                    for (int k = 0; k < j; k++)
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
            #pragma omp section
            {
                for(int i = inter[14]; i < inter[15]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++)
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

            #pragma omp section
            {
                for(int i = inter[15]; i < inter[16]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++)
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
    }
}



void str_3(double **A, double **L, double **U, int n, int nthreads)
{
    // int i, j, k;
    double sum = 0;

    omp_set_num_threads(nthreads);
    #pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        U[i][i] = 1;
        // All diagonal entries 1 in U
    }

    
    for (int j = 0; j < n; j++) {

        int* inter = intervals(j, n);

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                for(int i = inter[0]; i < inter[1]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++){
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[1]; i < inter[2]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }
            #pragma omp section
            {
                for(int i = inter[2]; i < inter[3]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[3]; i < inter[4]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[4]; i < inter[5]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[5]; i < inter[6]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }
            #pragma omp section
            {
                for(int i = inter[6]; i < inter[7]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[7]; i < inter[8]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[8]; i < inter[9]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[9]; i < inter[10]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }
            #pragma omp section
            {
                for(int i = inter[10]; i < inter[11]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[11]; i < inter[12]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[12]; i < inter[13]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[13]; i < inter[14]; i++ ){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }
            #pragma omp section
            {
                for(int i = inter[14]; i < inter[15]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }

            #pragma omp section
            {
                for(int i = inter[15]; i < inter[16]; i++){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }
        }

        #pragma omp parallel for
        for (int i = j; i < n; i++)
        {
            double sum = 0;

            
            for (int k = 0; k < j; k++)
            {
                // #pragma omp critical
                {sum = sum + L[j][k] * U[k][i];}
            }
            // #pragma omp barrier
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
    // printf("writing\n");
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
    
    int thr_proc = atoi(argv[3]);
    int strategy = atoi(argv[4]);
    

    FILE *f = fopen(input, "r");
    if (f == NULL)
    {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }

    // printf("going well\n");

    double **inp;
    inp = (double **)malloc(sizeof(double*) * size);

    for (int i = 0; i < size; i++){
        inp[i] = (double *)malloc(size * sizeof(double));
    }

    double num;

    for(int i = 0; i < size; i++){
        // printf("going well loop\n");
        for(int j = 0; j < size; j++){
            fscanf(f, "%lf", &num);
            // printf("Scanned int: %f\n", num);
            inp[i][j] = num;
        }
    }
    // printf("going well 3\n");

    fclose(f);

    // printf("going well 4\n");


    double **L;
    L = (double **)malloc(sizeof(double *) * size);

    double **U;
    U = (double **)malloc(sizeof(double *) * size);

    for (int i = 0; i < size; i++){
        L[i] = (double *)malloc(size * sizeof(double));
        U[i] = (double *)malloc(size * sizeof(double));
    }



    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            L[i][j] = 0;
            U[i][j] = 0;
        }
    }

    // double const **A = (double const **)inp;
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    if(strategy == 0){
        // printf("str0\n");
        crout(inp, L, U, size);
        
        char out_0_L[100];
        sprintf(out_0_L, "output_L_%d_%d.txt", strategy, thr_proc);
        char out_0_U[100];
        sprintf(out_0_U, "output_U_%d_%d.txt", strategy, thr_proc);

        

        // write_output(out_0_A, inp, size);
        write_output(out_0_L, L, size);
        write_output(out_0_U, U, size);
    }

    if(strategy == 1){
        // printf("str0\n");
        str_1(inp, L, U, size, thr_proc);
        
        char out_1_L[100];
        sprintf(out_1_L, "output_L_%d_%d.txt", strategy, thr_proc);
        char out_1_U[100];
        sprintf(out_1_U, "output_U_%d_%d.txt", strategy, thr_proc);

        

        // write_output(out_1_A, inp, size);
        write_output(out_1_L, L, size);
        write_output(out_1_U, U, size);
    }

    if(strategy == 2){
        str_2(inp, L, U, size, thr_proc);
        
        char out_2_L[100];
        sprintf(out_2_L, "output_L_%d_%d.txt", strategy, thr_proc);
        char out_2_U[100];
        sprintf(out_2_U, "output_U_%d_%d.txt", strategy, thr_proc);

        

        // write_output(out_2_A, inp, size);
        write_output(out_2_L, L, size);
        write_output(out_2_U, U, size);
    }

    if(strategy == 3){
        str_3(inp, L, U, size, thr_proc);
        
        char out_3_L[100];
        sprintf(out_3_L, "output_L_%d_%d.txt", strategy, thr_proc);
        char out_3_U[100];
        sprintf(out_3_U, "output_U_%d_%d.txt", strategy, thr_proc);

        

        // write_output(out_3_A, inp, size);
        write_output(out_3_L, L, size);
        write_output(out_3_U, U, size);
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    // printf("For strategy: %d, thr/proc: %d, Size: %d, pragram took: %f\n", strategy, thr_proc, size, cpu_time_used);

    return 0;
}