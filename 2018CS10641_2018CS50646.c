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
    int i, j, k;
    double sum = 0;

    omp_set_num_threads(nthreads);
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
            #pragma omp parallel for
            for (k = 0; k < j; k++)
            {
                #pragma omp critical
                {sum = sum + L[i][k] * U[k][j];}
            }
            #pragma omp barrier
            L[i][j] = A[i][j] - sum;
        }
        for (i = j; i < n; i++)
        {
            sum = 0;
            #pragma omp parallel for
            for (k = 0; k < j; k++)
            {
                #pragma omp critical
                {sum = sum + L[j][k] * U[k][i];}
            }
            #pragma omp barrier
            if (L[j][j] == 0)
            {
                exit(0);
            }
            U[j][i] = (A[j][i] - sum) / L[j][j];
        }
    }
}



void str_2(double **A, double **L, double **U, int n)
{
    int i, j, k;
    double sum = 0;
    #pragma omp parallel sections 
    {
        #pragma omp section
        {
            for (i = 0; i < n/4; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (i = n/4; i < 2*n/4; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (i = 2*n/4; i < 3*n/4; i++) {
                U[i][i] = 1;
            }
        }
        #pragma omp section
        {
            for (i = 3*n/4; i < n; i++) {
                U[i][i] = 1;
            }
        }
    }



    for (int j = 0; j < n; j++) {

        for (int i = j; i < n; i++) {
            sum = 0;

            #pragma omp parallel sections
            {
                #pragma omp section
                {
                    double l_sum = 0;
                    for (int k = 0; k < j/4 ; k++) {
                        l_sum = l_sum + L[i][k] * U[k][j];
                    }
                    #pragma omp critical
                    {sum= sum + l_sum;}
                }

                #pragma omp section
                {
                  double s = 0;
                  for (int k = j/4; k <2*j/4; k++) {
                    s=s+ L[i][k] * U[k][j];
                  }
                  #pragma omp critical
                    {sum = sum + s;}
                }

                #pragma omp section
                {
                  double s = 0;
                  for (int k = 2*j/4; k < 3*j/4; k++) {
                      s=s+ L[i][k] * U[k][j];
                  }
                  #pragma omp critical
                  {sum = sum + s;}
                }

                #pragma omp section
                {
                  double s = 0;
                  for (int k = 3*j/4; k < 4*j/4; k++) {
                      s= s + L[i][k] * U[k][j];
                  }
                  #pragma omp critical
                  {sum = sum + s;}
                }

            }

            L[i][j] = A[i][j] - sum;
        }

        for (int i = j; i < n; i++) {
            sum = 0;

            #pragma omp sections
            {
              #pragma omp section
              {
                double s = 0;
                for (int k = 0; k < j/4 ; k++) {
                    s = s + L[j][k] * U[k][i];
                }
                #pragma omp critical
                {sum= sum + s;}
              }

              #pragma omp section
              {
                double s = 0;
                for (int k = j/4; k < 2*j/4; k++) {
                    s=s+ L[j][k] * U[k][i];
                }
                #pragma omp critical
                {sum = sum + s;}
              }

              #pragma omp section
              {
                double s = 0;
                for (int k = 2*j/4; k < 3*j/4; k++) {
                    s=s+ L[j][k] * U[k][i];
                }
                #pragma omp critical
                {sum = sum + s;}
              }

              #pragma omp  section
              {
                double s = 0;
                for (int k = 3*j/4; k < 4*j/4; k++) {
                    s= s + L[j][k] * U[k][i];
                }
                #pragma omp critical
                {sum = sum + s;}
              }
            }
            if (L[j][j] == 0) {
                exit(0);
            }
            U[j][i] = (A[j][i] - sum) / L[j][j];
          }
      }
}



void str_3(double **A, double **L, double **U, int n, int nthreads)
{
    int i, j, k;
    double sum = 0;

    omp_set_num_threads(nthreads);
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

            omp_set_num_threads(nthreads);
            #pragma omp parallel for
            for (k = 0; k < j; k++)
            {
                #pragma omp critical
                {sum = sum + L[i][k] * U[k][j];}
            }
            #pragma omp barrier
            L[i][j] = A[i][j] - sum;
        }
        for (i = j; i < n; i++)
        {
            sum = 0;
            #pragma omp  sections
            {
              #pragma omp  section
              {
                double s = 0;
                for (int k = 0; k < j/4 ; k++) {
                    s = s + L[j][k] * U[k][i];
                }
                #pragma omp critical
                {sum= sum + s;}
              }

              #pragma omp  section
              {
                double s = 0;
                for (int k = j/4; k < 2*j/4; k++) {
                    s=s+ L[j][k] * U[k][i];
                }
                #pragma omp critical
                {sum = sum + s;}
              }

              #pragma omp  section
              {
                double s = 0;
                for (int k = 2*j/4; k < 3*j/4; k++) {
                    s=s+ L[j][k] * U[k][i];
                }
                #pragma omp critical
                {sum = sum + s;}
              }

              #pragma omp  section
              {
                double s = 0;
                for (int k = 3*j/4; k < 4*j/4; k++) {
                    s= s + L[j][k] * U[k][i];
                }
                #pragma omp critical
                {sum = sum + s;}
              }
            }
            if (L[j][j] == 0) {
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
            fscanf(f, "%lf", &num);
            printf("Scanned int: %f\n", num);
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
                printf("%f\n", inp[i][j]);
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

    if(strategy == 1){
        // printf("str0\n");
        str_1(inp, L, U, size, thr_proc);
        printf(".......................................................................\n");
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                // fscanf(f, "%d", &num);
                printf("%f\n", inp[i][j]);
                // inp[i][j] = num;
            }
        }
        printf(".......................................................................\n");
        char out_1_A[] = "out_1_A.txt";
        char out_1_L[] = "out_1_L.txt";
        char out_1_U[] = "out_1_U.txt";

        

        write_output(out_1_A, inp, size);
        write_output(out_1_L, L, size);
        write_output(out_1_U, U, size);
    }

    if(strategy == 2){
        str_2(inp, L, U, size);
        printf(".......................................................................\n");
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                // fscanf(f, "%d", &num);
                printf("%f\n", inp[i][j]);
                // inp[i][j] = num;
            }
        }
        printf(".......................................................................\n");
        char out_2_A[] = "out_2_A.txt";
        char out_2_L[] = "out_2_L.txt";
        char out_2_U[] = "out_2_U.txt";

        

        write_output(out_2_A, inp, size);
        write_output(out_2_L, L, size);
        write_output(out_2_U, U, size);
    }

    if(strategy == 3){
        str_3(inp, L, U, size, thr_proc);
        printf(".......................................................................\n");
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                // fscanf(f, "%d", &num);
                printf("%f\n", inp[i][j]);
                // inp[i][j] = num;
            }
        }
        printf(".......................................................................\n");
        char out_3_A[] = "out_3_A.txt";
        char out_3_L[] = "out_3_L.txt";
        char out_3_U[] = "out_3_U.txt";

        

        write_output(out_3_A, inp, size);
        write_output(out_3_L, L, size);
        write_output(out_3_U, U, size);
    }

    return 0;
}