#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// int* diva(int a , int b ){
//     int* ret = malloc(16*sizeof(int));
//     int temp = b-a;
//     int t = 0;
//     for(int i =0;i<16 ; i++){
//         t+=temp/16;
//         ret[i] = a+temp/16;
//         printf("%d",ret[i]);
//     }
//     ret[15] = a+temp-t;
//
//     return ret;
// }

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


int* diva(int m , int n){
    int x = n-m;
    int divi = x/16;
    int* arr = malloc (sizeof(int)*17);
    arr[0] = m;
    int end, init = m;
    for(int i = 0; i < 15; i++){
        end = init + divi;
        arr[i+1] = end;
        init = end;
    }
    arr[16] = n;
    return arr;
}
int main(int argc, char *argv[]) {
  // printf("dfasafs" );
    double start_time = omp_get_wtime();
    int n = atoi(argv[1]);
    int strategy = atoi(argv[4]);
    // printf("%d\n",strategy );
    double** A = (double**) malloc(sizeof(double*)* n);
    double** L = (double**) malloc(sizeof(double*)* n);
    double** U = (double**) malloc(sizeof(double*)* n);
    for(int i = 0; i < n; i++){
        A[i] = (double*) malloc(sizeof(double)*n);
        L[i] = (double*) malloc(sizeof(double)*n);
        U[i] = (double*) malloc(sizeof(double)*n);
    }
    // printf("dfsa" );
    int number_of_threads = atoi(argv[3]);
    omp_set_num_threads(number_of_threads);
    FILE *infile = fopen(argv[2], "r");
    char str[n*64];
    int ii = 0;
    while(fscanf(infile, "%[^\n]%*c", str) != EOF){
        char *delim = " \n", *token;
        int argIndex = 0;
		for (token = strtok (str, delim); token && argIndex + 1 < n+2; token = strtok(NULL, delim)) {
       		A[ii][argIndex++] = strtod(token, NULL);
         }
        // for (int j = 0; j < n; j++)
        //    	printf("%f", A[ii][j]);
        ii++;
    }

// write_output("in.txt",A,n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            L[i][j] = 0;
        }
        // printf("\n");
    }
    // printf("\n");
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            U[i][j] = 0;
        }
        // printf("\n");
    }

    if(strategy == 0){
      // printf("adfsdfa");
        int i, j, k;
        double sum = 0;
        for (i = 0; i < n; i++) {
            U[i][i] = 1;
        }
        for (j = 0; j < n; j++) {
            for (i = j; i < n; i++) {
                sum = 0;
                for (k = 0; k < j; k++) {
                    sum = sum + L[i][k] * U[k][j];
                }
                L[i][j] = A[i][j] - sum;
            }
            for (i = j; i < n; i++) {
                sum = 0;
                for(k = 0; k < j; k++) {
                    sum = sum + L[j][k] * U[k][i];
                }
                if (L[j][j] == 0) {
                  // printf("ko---------------\n" );
                    exit(0);
                }
                U[j][i] = (A[j][i] - sum) / L[j][j];
            }
        }
    }
    if(strategy == 1){
        // int i, j, k;
        // double sum = 0;
        // printf("ggsgdgd\n" );
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            U[i][i] = 1;
        }

        for (int j = 0; j < n; j++) {
          #pragma omp parallel for
            for (int i = j; i < n; i++) {
                double sum = 0;
                for (int k = 0; k < j; k++) {
                    sum = sum + L[i][k] * U[k][j];
                }
                L[i][j] = A[i][j] - sum;
            }
            #pragma omp parallel for
            for (int i = j; i < n; i++) {
                double sum = 0;
                for(int k = 0; k < j; k++) {
                    sum = sum + L[j][k] * U[k][i];
                }
                if (L[j][j] == 0) {
                    exit(0);
                }
                U[j][i] = (A[j][i] - sum) / L[j][j];
            }
        }
    }
    if(strategy == 2){
        // int i, j, k;
        // double sum = 0;
        // printf("ggsgdgd\n" );
        int* li = diva(0,n);
        // for(int i =0 ; i<17 ; i++ ){
        //   printf("%d\n",li[i] );
        // }
        #pragma omp parallel sections
        {
          #pragma omp section
          {
              for(int i =li[15] ; i<li[16] ; i++ ){
                  U[i][i] = 1;
              }
            }

            #pragma omp section
            {
                for(int i =li[0] ; i<li[1] ; i++ ){
                    U[i][i] = 1;
                }
              }
              #pragma omp section
              {
                  for(int i =li[1] ; i<li[2] ; i++ ){
                      U[i][i] = 1;
                  }
                }

                #pragma omp section
                {
                    for(int i =li[2] ; i<li[3] ; i++ ){
                        U[i][i] = 1;
                    }
                  }

                    #pragma omp section
                    {
                        for(int i =li[3] ; i<li[4] ; i++ ){
                            U[i][i] = 1;
                        }
                      }

                      #pragma omp section
                      {
                          for(int i =li[4] ; i<li[5] ; i++ ){
                              U[i][i] = 1;
                          }
                        }
                        #pragma omp section
                        {
                            for(int i =li[5] ; i<li[6] ; i++ ){
                                U[i][i] = 1;
                            }
                          }

                          #pragma omp section
                          {
                              for(int i =li[6] ; i<li[7] ; i++ ){
                                  U[i][i] = 1;
                              }
                            }

                              #pragma omp section
                              {
                                  for(int i =li[7] ; i<li[8] ; i++ ){
                                      U[i][i] = 1;
                                  }
                                }

                                #pragma omp section
                                {
                                    for(int i =li[8] ; i<li[9] ; i++ ){
                                        U[i][i] = 1;
                                    }
                                  }
                                  #pragma omp section
                                  {
                                      for(int i =li[9] ; i<li[10] ; i++ ){
                                          U[i][i] = 1;
                                      }
                                    }

                                    #pragma omp section
                                    {
                                        for(int i =li[10] ; i<li[11] ; i++ ){
                                            U[i][i] = 1;
                                        }
                                      }

                                        #pragma omp section
                                        {
                                            for(int i =li[11] ; i<li[12] ; i++ ){
                                                U[i][i] = 1;
                                            }
                                          }

                                          #pragma omp section
                                          {
                                              for(int i =li[12] ; i<li[13] ; i++ ){
                                                  U[i][i] = 1;
                                              }
                                            }
                                            #pragma omp section
                                            {
                                                for(int i =li[13] ; i<li[14] ; i++ ){
                                                    U[i][i] = 1;
                                                }
                                              }

                                              #pragma omp section
                                              {
                                                  for(int i =li[14] ; i<li[15] ; i++ ){
                                                      U[i][i] = 1;
                                                  }
                                                }
      }

        for (int j = 0; j < n; j++) {
          // #pragma omp parallel for
          //   for (int i = j; i < n; i++) {
                // double sum = 0;
                // for (int k = 0; k < j; k++) {
                //     sum = sum + L[i][k] * U[k][j];
                // }
                // L[i][j] = A[i][j] - sum;
          //   }

            // printf("ggsgdgd\n" );
            int* lii = diva(j,n);
            // for(int i =0 ; i<17 ; i++ ){
            //   printf("%d\n",lii[i] );
            // }
            #pragma omp parallel sections
            {
              #pragma omp section
              {
                  for(int i =lii[15] ; i<lii[16] ; i++ ){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                  }
                }

                #pragma omp section
                {
                    for(int i =lii[0] ; i<lii[1] ; i++ ){
                      double sum = 0;
                      for (int k = 0; k < j; k++) {
                          sum = sum + L[i][k] * U[k][j];
                      }
                      L[i][j] = A[i][j] - sum;
                    }
                  }
                  #pragma omp section
                  {
                      for(int i =lii[1] ; i<lii[2] ; i++ ){
                        double sum = 0;
                        for (int k = 0; k < j; k++) {
                            sum = sum + L[i][k] * U[k][j];
                        }
                        L[i][j] = A[i][j] - sum;
                      }
                    }

                    #pragma omp section
                    {
                        for(int i =lii[2] ; i<lii[3] ; i++ ){
                          double sum = 0;
                          for (int k = 0; k < j; k++) {
                              sum = sum + L[i][k] * U[k][j];
                          }
                          L[i][j] = A[i][j] - sum;
                        }
                      }

                        #pragma omp section
                        {
                            for(int i =lii[3] ; i<lii[4] ; i++ ){
                              double sum = 0;
                              for (int k = 0; k < j; k++) {
                                  sum = sum + L[i][k] * U[k][j];
                              }
                              L[i][j] = A[i][j] - sum;
                            }
                          }

                          #pragma omp section
                          {
                              for(int i =lii[4] ; i<lii[5] ; i++ ){
                                double sum = 0;
                                for (int k = 0; k < j; k++) {
                                    sum = sum + L[i][k] * U[k][j];
                                }
                                L[i][j] = A[i][j] - sum;
                              }
                            }
                            #pragma omp section
                            {
                                for(int i =lii[5] ; i<lii[6] ; i++ ){
                                  double sum = 0;
                                  for (int k = 0; k < j; k++) {
                                      sum = sum + L[i][k] * U[k][j];
                                  }
                                  L[i][j] = A[i][j] - sum;
                                }
                              }

                              #pragma omp section
                              {
                                  for(int i =lii[6] ; i<lii[7] ; i++ ){
                                    double sum = 0;
                                    for (int k = 0; k < j; k++) {
                                        sum = sum + L[i][k] * U[k][j];
                                    }
                                    L[i][j] = A[i][j] - sum;
                                  }
                                }

                                  #pragma omp section
                                  {
                                      for(int i =lii[7] ; i<lii[8] ; i++ ){
                                        double sum = 0;
                                        for (int k = 0; k < j; k++) {
                                            sum = sum + L[i][k] * U[k][j];
                                        }
                                        L[i][j] = A[i][j] - sum;
                                      }
                                    }

                                    #pragma omp section
                                    {
                                        for(int i =lii[8] ; i<lii[9] ; i++ ){
                                          double sum = 0;
                                          for (int k = 0; k < j; k++) {
                                              sum = sum + L[i][k] * U[k][j];
                                          }
                                          L[i][j] = A[i][j] - sum;
                                        }
                                      }
                                      #pragma omp section
                                      {
                                          for(int i =lii[9] ; i<lii[10] ; i++ ){
                                            double sum = 0;
                                            for (int k = 0; k < j; k++) {
                                                sum = sum + L[i][k] * U[k][j];
                                            }
                                            L[i][j] = A[i][j] - sum;
                                          }
                                        }

                                        #pragma omp section
                                        {
                                            for(int i =lii[10] ; i<lii[11] ; i++ ){
                                              double sum = 0;
                                              for (int k = 0; k < j; k++) {
                                                  sum = sum + L[i][k] * U[k][j];
                                              }
                                              L[i][j] = A[i][j] - sum;
                                            }
                                          }

                                            #pragma omp section
                                            {
                                                for(int i =lii[11] ; i<lii[12] ; i++ ){
                                                  double sum = 0;
                                                  for (int k = 0; k < j; k++) {
                                                      sum = sum + L[i][k] * U[k][j];
                                                  }
                                                  L[i][j] = A[i][j] - sum;
                                                }
                                              }

                                              #pragma omp section
                                              {
                                                  for(int i =lii[12] ; i<lii[13] ; i++ ){
                                                    double sum = 0;
                                                    for (int k = 0; k < j; k++) {
                                                        sum = sum + L[i][k] * U[k][j];
                                                    }
                                                    L[i][j] = A[i][j] - sum;
                                                  }
                                                }
                                                #pragma omp section
                                                {
                                                    for(int i =lii[13] ; i<lii[14] ; i++ ){
                                                      double sum = 0;
                                                      for (int k = 0; k < j; k++) {
                                                          sum = sum + L[i][k] * U[k][j];
                                                      }
                                                      L[i][j] = A[i][j] - sum;
                                                    }
                                                  }

                                                  #pragma omp section
                                                  {
                                                      for(int i =lii[14] ; i<lii[15] ; i++ ){
                                                        double sum = 0;
                                                        for (int k = 0; k < j; k++) {
                                                            sum = sum + L[i][k] * U[k][j];
                                                        }
                                                        L[i][j] = A[i][j] - sum;
                                                      }
                                                    }
          }


            // #pragma omp parallel for
            // for (int i = j; i < n; i++) {
            //     double sum = 0;
            //     for(int k = 0; k < j; k++) {
            //         sum = sum + L[j][k] * U[k][i];
            //     }
            //     if (L[j][j] == 0) {
            //         exit(0);
            //     }
            //     U[j][i] = (A[j][i] - sum) / L[j][j];
            // }
            int* liii = diva(j,n);
            // for(int i =0 ; i<17 ; i++ ){
            //   printf("%d\n",li[i] );
            // }
            #pragma omp parallel sections
            {
              #pragma omp section
              {
                  for(int i =liii[15] ; i<liii[16] ; i++ ){
                        double sum = 0;
                        for(int k = 0; k < j; k++) {
                            sum = sum + L[j][k] * U[k][i];
                        }
                        if (L[j][j] == 0) {
                            exit(0);
                        }
                        U[j][i] = (A[j][i] - sum) / L[j][j];
                  }
                }

                #pragma omp section
                {
                    for(int i =liii[0] ; i<liii[1] ; i++ ){
                      double sum = 0;
                      for(int k = 0; k < j; k++) {
                          sum = sum + L[j][k] * U[k][i];
                      }
                      if (L[j][j] == 0) {
                          exit(0);
                      }
                      U[j][i] = (A[j][i] - sum) / L[j][j];
                    }
                  }
                  #pragma omp section
                  {
                      for(int i =liii[1] ; i<liii[2] ; i++ ){
                        double sum = 0;
                        for(int k = 0; k < j; k++) {
                            sum = sum + L[j][k] * U[k][i];
                        }
                        if (L[j][j] == 0) {
                            exit(0);
                        }
                        U[j][i] = (A[j][i] - sum) / L[j][j];
                      }
                    }

                    #pragma omp section
                    {
                        for(int i =liii[2] ; i<liii[3] ; i++ ){
                          double sum = 0;
                          for(int k = 0; k < j; k++) {
                              sum = sum + L[j][k] * U[k][i];
                          }
                          if (L[j][j] == 0) {
                              exit(0);
                          }
                          U[j][i] = (A[j][i] - sum) / L[j][j];
                        }
                      }

                        #pragma omp section
                        {
                            for(int i =liii[3] ; i<liii[4] ; i++ ){
                              double sum = 0;
                              for(int k = 0; k < j; k++) {
                                  sum = sum + L[j][k] * U[k][i];
                              }
                              if (L[j][j] == 0) {
                                  exit(0);
                              }
                              U[j][i] = (A[j][i] - sum) / L[j][j];
                            }
                          }

                          #pragma omp section
                          {
                              for(int i =liii[4] ; i<liii[5] ; i++ ){
                                double sum = 0;
                                for(int k = 0; k < j; k++) {
                                    sum = sum + L[j][k] * U[k][i];
                                }
                                if (L[j][j] == 0) {
                                    exit(0);
                                }
                                U[j][i] = (A[j][i] - sum) / L[j][j];
                              }
                            }
                            #pragma omp section
                            {
                                for(int i =liii[5] ; i<liii[6] ; i++ ){
                                  double sum = 0;
                                  for(int k = 0; k < j; k++) {
                                      sum = sum + L[j][k] * U[k][i];
                                  }
                                  if (L[j][j] == 0) {
                                      exit(0);
                                  }
                                  U[j][i] = (A[j][i] - sum) / L[j][j];
                                }
                              }

                              #pragma omp section
                              {
                                  for(int i =liii[6] ; i<liii[7] ; i++ ){
                                    double sum = 0;
                                    for(int k = 0; k < j; k++) {
                                        sum = sum + L[j][k] * U[k][i];
                                    }
                                    if (L[j][j] == 0) {
                                        exit(0);
                                    }
                                    U[j][i] = (A[j][i] - sum) / L[j][j];
                                  }
                                }

                                  #pragma omp section
                                  {
                                      for(int i =liii[7] ; i<liii[8] ; i++ ){
                                        double sum = 0;
                                        for(int k = 0; k < j; k++) {
                                            sum = sum + L[j][k] * U[k][i];
                                        }
                                        if (L[j][j] == 0) {
                                            exit(0);
                                        }
                                        U[j][i] = (A[j][i] - sum) / L[j][j];
                                      }
                                    }

                                    #pragma omp section
                                    {
                                        for(int i =liii[8] ; i<liii[9] ; i++ ){
                                          double sum = 0;
                                          for(int k = 0; k < j; k++) {
                                              sum = sum + L[j][k] * U[k][i];
                                          }
                                          if (L[j][j] == 0) {
                                              exit(0);
                                          }
                                          U[j][i] = (A[j][i] - sum) / L[j][j];
                                        }
                                      }
                                      #pragma omp section
                                      {
                                          for(int i =liii[9] ; i<liii[10] ; i++ ){
                                            double sum = 0;
                                            for(int k = 0; k < j; k++) {
                                                sum = sum + L[j][k] * U[k][i];
                                            }
                                            if (L[j][j] == 0) {
                                                exit(0);
                                            }
                                            U[j][i] = (A[j][i] - sum) / L[j][j];
                                          }
                                        }

                                        #pragma omp section
                                        {
                                            for(int i =liii[10] ; i<liii[11] ; i++ ){
                                              double sum = 0;
                                              for(int k = 0; k < j; k++) {
                                                  sum = sum + L[j][k] * U[k][i];
                                              }
                                              if (L[j][j] == 0) {
                                                  exit(0);
                                              }
                                              U[j][i] = (A[j][i] - sum) / L[j][j];
                                            }
                                          }

                                            #pragma omp section
                                            {
                                                for(int i =liii[11] ; i<liii[12] ; i++ ){
                                                  double sum = 0;
                                                  for(int k = 0; k < j; k++) {
                                                      sum = sum + L[j][k] * U[k][i];
                                                  }
                                                  if (L[j][j] == 0) {
                                                      exit(0);
                                                  }
                                                  U[j][i] = (A[j][i] - sum) / L[j][j];
                                                }
                                              }

                                              #pragma omp section
                                              {
                                                  for(int i =liii[12] ; i<liii[13] ; i++ ){
                                                    double sum = 0;
                                                    for(int k = 0; k < j; k++) {
                                                        sum = sum + L[j][k] * U[k][i];
                                                    }
                                                    if (L[j][j] == 0) {
                                                        exit(0);
                                                    }
                                                    U[j][i] = (A[j][i] - sum) / L[j][j];
                                                  }
                                                }
                                                #pragma omp section
                                                {
                                                    for(int i =liii[13] ; i<liii[14] ; i++ ){
                                                      double sum = 0;
                                                      for(int k = 0; k < j; k++) {
                                                          sum = sum + L[j][k] * U[k][i];
                                                      }
                                                      if (L[j][j] == 0) {
                                                          exit(0);
                                                      }
                                                      U[j][i] = (A[j][i] - sum) / L[j][j];
                                                    }
                                                  }

                                                  #pragma omp section
                                                  {
                                                      for(int i =liii[14] ; i<liii[15] ; i++ ){
                                                        double sum = 0;
                                                        for(int k = 0; k < j; k++) {
                                                            sum = sum + L[j][k] * U[k][i];
                                                        }
                                                        if (L[j][j] == 0) {
                                                            exit(0);
                                                        }
                                                        U[j][i] = (A[j][i] - sum) / L[j][j];
                                                      }
                                                    }
          }
        }
    }
    if(strategy == 3){
        // int i, j, k;
        // double sum = 0;
        // printf("ggsgdgd\n" );
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            U[i][i] = 1;
        }

        for (int j = 0; j < n; j++) {
          int* lii = diva(j,n);
          #pragma omp parallel sections
          {
            #pragma omp section
            {
                for(int i =lii[15] ; i<lii[16] ; i++ ){
                  double sum = 0;
                  for (int k = 0; k < j; k++) {
                      sum = sum + L[i][k] * U[k][j];
                  }
                  L[i][j] = A[i][j] - sum;
                }
              }

              #pragma omp section
              {
                  for(int i =lii[0] ; i<lii[1] ; i++ ){
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];
                    }
                    L[i][j] = A[i][j] - sum;
                  }
                }
                #pragma omp section
                {
                    for(int i =lii[1] ; i<lii[2] ; i++ ){
                      double sum = 0;
                      for (int k = 0; k < j; k++) {
                          sum = sum + L[i][k] * U[k][j];
                      }
                      L[i][j] = A[i][j] - sum;
                    }
                  }

                  #pragma omp section
                  {
                      for(int i =lii[2] ; i<lii[3] ; i++ ){
                        double sum = 0;
                        for (int k = 0; k < j; k++) {
                            sum = sum + L[i][k] * U[k][j];
                        }
                        L[i][j] = A[i][j] - sum;
                      }
                    }

                      #pragma omp section
                      {
                          for(int i =lii[3] ; i<lii[4] ; i++ ){
                            double sum = 0;
                            for (int k = 0; k < j; k++) {
                                sum = sum + L[i][k] * U[k][j];
                            }
                            L[i][j] = A[i][j] - sum;
                          }
                        }

                        #pragma omp section
                        {
                            for(int i =lii[4] ; i<lii[5] ; i++ ){
                              double sum = 0;
                              for (int k = 0; k < j; k++) {
                                  sum = sum + L[i][k] * U[k][j];
                              }
                              L[i][j] = A[i][j] - sum;
                            }
                          }
                          #pragma omp section
                          {
                              for(int i =lii[5] ; i<lii[6] ; i++ ){
                                double sum = 0;
                                for (int k = 0; k < j; k++) {
                                    sum = sum + L[i][k] * U[k][j];
                                }
                                L[i][j] = A[i][j] - sum;
                              }
                            }

                            #pragma omp section
                            {
                                for(int i =lii[6] ; i<lii[7] ; i++ ){
                                  double sum = 0;
                                  for (int k = 0; k < j; k++) {
                                      sum = sum + L[i][k] * U[k][j];
                                  }
                                  L[i][j] = A[i][j] - sum;
                                }
                              }

                                #pragma omp section
                                {
                                    for(int i =lii[7] ; i<lii[8] ; i++ ){
                                      double sum = 0;
                                      for (int k = 0; k < j; k++) {
                                          sum = sum + L[i][k] * U[k][j];
                                      }
                                      L[i][j] = A[i][j] - sum;
                                    }
                                  }

                                  #pragma omp section
                                  {
                                      for(int i =lii[8] ; i<lii[9] ; i++ ){
                                        double sum = 0;
                                        for (int k = 0; k < j; k++) {
                                            sum = sum + L[i][k] * U[k][j];
                                        }
                                        L[i][j] = A[i][j] - sum;
                                      }
                                    }
                                    #pragma omp section
                                    {
                                        for(int i =lii[9] ; i<lii[10] ; i++ ){
                                          double sum = 0;
                                          for (int k = 0; k < j; k++) {
                                              sum = sum + L[i][k] * U[k][j];
                                          }
                                          L[i][j] = A[i][j] - sum;
                                        }
                                      }

                                      #pragma omp section
                                      {
                                          for(int i =lii[10] ; i<lii[11] ; i++ ){
                                            double sum = 0;
                                            for (int k = 0; k < j; k++) {
                                                sum = sum + L[i][k] * U[k][j];
                                            }
                                            L[i][j] = A[i][j] - sum;
                                          }
                                        }

                                          #pragma omp section
                                          {
                                              for(int i =lii[11] ; i<lii[12] ; i++ ){
                                                double sum = 0;
                                                for (int k = 0; k < j; k++) {
                                                    sum = sum + L[i][k] * U[k][j];
                                                }
                                                L[i][j] = A[i][j] - sum;
                                              }
                                            }

                                            #pragma omp section
                                            {
                                                for(int i =lii[12] ; i<lii[13] ; i++ ){
                                                  double sum = 0;
                                                  for (int k = 0; k < j; k++) {
                                                      sum = sum + L[i][k] * U[k][j];
                                                  }
                                                  L[i][j] = A[i][j] - sum;
                                                }
                                              }
                                              #pragma omp section
                                              {
                                                  for(int i =lii[13] ; i<lii[14] ; i++ ){
                                                    double sum = 0;
                                                    for (int k = 0; k < j; k++) {
                                                        sum = sum + L[i][k] * U[k][j];
                                                    }
                                                    L[i][j] = A[i][j] - sum;
                                                  }
                                                }

                                                #pragma omp section
                                                {
                                                    for(int i =lii[14] ; i<lii[15] ; i++ ){
                                                      double sum = 0;
                                                      for (int k = 0; k < j; k++) {
                                                          sum = sum + L[i][k] * U[k][j];
                                                      }
                                                      L[i][j] = A[i][j] - sum;
                                                    }
                                                  }
        }
            #pragma omp parallel for
            for (int i = j; i < n; i++) {
                double sum = 0;
                for(int k = 0; k < j; k++) {
                    sum = sum + L[j][k] * U[k][i];
                }
                if (L[j][j] == 0) {
                    exit(0);
                }
                U[j][i] = (A[j][i] - sum) / L[j][j];
            }
        }
    }
//

char * s1 = malloc(sizeof(char)*50), *s2 = malloc(sizeof(char)*50);
    sprintf(s1, "output_L_%d_%d.txt",strategy ,number_of_threads );
    sprintf(s2, "output_U_%d_%d.txt", strategy,number_of_threads);

write_output(s1,L,n);
write_output(s2,U,n);
double end_time = omp_get_wtime();
printf("%f\n", end_time-start_time );
    // for(int i = 0; i < n; i++){
    //     for(int j = 0; j < n; j++){
    //         printf("%f ", L[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    // for(int i = 0; i < n; i++){
    //     for(int j = 0; j < n; j++){
    //         printf("%f ", U[i][j]);
    //     }
    //     printf("\n");
    // }
}
