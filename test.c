#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>

void write_output(char fname[], double** arr, int n ){
    FILE *f = fopen(fname, "w");
    printf("writing\n");
    for( int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            fprintf(f, "%0.12f ", arr[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

int main(int argc, char* argv[]){
MPI_Init(&argc, &argv);
int t,pid;
MPI_Comm_rank(MPI_COMM_WORLD, &pid);
MPI_Comm_size(MPI_COMM_WORLD, &t);
int n = atoi(argv[1]);
  char *input_file = argv[2];
  int strat = 4;
  
  double** A = malloc(n * sizeof(double*));
  for(int i=0; i<n; i++){
    A[i] = malloc(n * sizeof(double*));
  }

  FILE *infile;
  infile = fopen(input_file, "r");
  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
      if(!fscanf(infile, "%lf", &A[i][j]))
        break;
    }
  }
 
  fclose(infile);
   

   int i, j, k;
   double** L = malloc(n * sizeof(double*));
    for(int i=0; i<n; i++){
      L[i] = malloc(n * sizeof(double*));
    }
    double** U = malloc(n * sizeof(double*));
    for(int i=0; i<n; i++){
      U[i] = malloc(n * sizeof(double*));
    }
    for (int i=0;i<n;i++){
    for (int j=0;j<n;j++){
    L[i][j] = 0;
    U[i][j] = 0;
    }
    }
   double sum = 0;
    for (i = 0; i < n; i++) {
        U[i][i] = 1;
    }
    for(j=0;j<n;j++){
        for(i=j;i<n;i++){
            if(i%t == pid){
                sum = 0;
                for (k = 0; k < j; k++) {
                    sum = sum + L[i][k] * U[k][j];	
                }
                L[i][j] = A[i][j] - sum;
                //printf("%0.3lf \n %d %d ",sum, i,j);
            }
            MPI_Bcast(
            &L[i][j],
            1,
            MPI_DOUBLE,
            i%t,
            MPI_COMM_WORLD);
        }
        for(i=j;i<n;i++){
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
            MPI_Bcast(
            &U[j][i],
            1,
            MPI_DOUBLE,
            i%t,
            MPI_COMM_WORLD);
        }
    }
 
 char fname1[100];
sprintf(fname1, "output_U_%d_%d.txt", strat,t);
write_output(fname1, U, n );
char fname2[100];
sprintf(fname2, "output_L_%d_%d.txt", strat,t);
write_output(fname2, L, n ); 
MPI_Finalize(); 
return 0;
}