#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int chr_until_comma(const char * str);
int readDataPointsFromFile(const char *filename, double ***data, int *numRows, int *numCols);
void enter_datapoint(double *arr, char* line);
void print_result( double** A, int rows, int cols);


int readDataPointsFromFile(const char *filename, double ***data, int *numRows, int *numCols) {

    int i, rowIndex;
    char line[1024];
    int rowCount = 0;
    int columnCount = 0;

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        rowCount++;

        if (rowCount == 1) {
            char *token = strtok(line, ",");
            while (token) {
                columnCount++;
                token = strtok(NULL, ",");
            }
        }
    }

    *numRows = rowCount;
    *numCols = columnCount;


    *data = (double **)malloc(rowCount * sizeof(double *));
    for (i = 0; i < rowCount; i++) {
        (*data)[i] = (double *)malloc(columnCount * sizeof(double));
    }

    fseek(file, 0, SEEK_SET);

    rowIndex = 0;
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        int colIndex = 0;
        while (token) {
            (*data)[rowIndex][colIndex] = atof(token);
            colIndex++;
            token = strtok(NULL, ",");
        }
        rowIndex++;
    }

    fclose(file);
    return 0;
}

void enter_datapoint(double *arr, char* line){
    int i = 0;
    int pos = 0;
    while (sscanf(line + pos, "%lf", &arr[i]) == 1) {
        pos += chr_until_comma(line + pos) + 1;
        if ((line[pos-1]) == '\0'){
            break;
        }
        i++;
    }
}

int chr_until_comma(const char * str){
    int i = 0;
    if (str == NULL){
        return 0;
    }
    while (str[i] != '\0' && str[i] != ','){
        i++;
    }
    return i;
}

void print_result( double** A, int rows, int cols){
 int i;
 int j;

 for (i=0; i < rows; i++){
        for(j=0; j < cols; j++){
            printf("%.4f", A[i][j]);
            if (j < cols - 1){
                printf(",");}
            
        }
        printf("\n");
    }

}

double squared_distance(double* x, double* y, int d) {
    int i;
    double sum = 0;
    for (i = 0; i < d; i++) {
        double diff = x[i] - y[i];
        sum += diff * diff;
    }
    return sum;
}

double sq_frobenius_norm(double** A, double** B, int n, int k) {
    int i,j;
    double sum = 0.0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < k; j++) {
            double diff = A[i][j] - B[i][j];
            sum += diff * diff;
        }
    }
    return sum;
}

double average_matrix(double** W, int n) {
    int i,j;
    double sum = 0.0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            sum += W[i][j];
        }
    }
    return sum / (n * n);
}

double compute_WH(double** W, double** H_prev, int i, int j, int n) {
    int l;
    double sum = 0.0;
    for ( l = 0; l < n; l++) {
        sum += W[i][l] * H_prev[l][j];
    }
    return sum;
}

double compute_H_Htr_H(double** H_prev, int i, int j, int n, int k) {
    double sum = 0.0;
    int l,m;
    for (l = 0; l < n; l++) {
        double temp = 0.0;
        for (m = 0; m < k; m++) {
            temp += H_prev[i][m] * H_prev[l][m];
        }
        sum += temp * H_prev[l][j];
    }

    return sum;
}



double** sym(double** X, int d, int n) {
 
    int i,j;
    double** A = malloc(n * sizeof(double*)); 

    for (i = 0; i < n; i++) {

        A[i] = (double*) malloc(n*sizeof(double));
        
        for (j = 0; j < n; j++) {
            if (i == j) {
                A[i][j] = 0.0;
            } else {
                double dist = squared_distance(X[i], X[j], d);
                A[i][j] = exp(-dist / 2);
            }
        }
    }
    return A;
}


double** ddg(double** X, int d, int n) {

    int i,j;
    double** A = sym(X,d,n);
    double** D = calloc(n , sizeof(double*)); 

    for (i = 0; i < n; i++) {
        double sum = 0.0;
        D[i] = (double*) calloc(n,sizeof(double));
        for (j = 0; j < n; j++) {
            sum += A[i][j];
        }
        D[i][i] = sum;
    }

    for (i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);

    return D;
}



double** norm(double** X, int d, int n) {

    int i,j;
    double ** A= sym(X,d,n);
    double** D = ddg(X,d,n); 
    double** intermediate;
    double** W;

    double** D_inv_sqrt = malloc(n * sizeof(double*));

    for (i = 0; i < n; i++) {
        D_inv_sqrt[i] = calloc(n, sizeof(double));
        D_inv_sqrt[i][i] = 1.0 / sqrt(D[i][i]);
    }

    intermediate = malloc(n * sizeof(double*));
    for (i = 0; i < n; i++) {
        intermediate[i] = calloc(n, sizeof(double));
        for (j = 0; j < n; j++) {
            intermediate[i][j] = D_inv_sqrt[i][i] * A[i][j];
        }
    }

    W = calloc(n , sizeof(double*)); 

    for (i = 0; i < n; i++) {
        W[i] = (double*) calloc(n,sizeof(double));
        for (j = 0; j < n; j++) {
            W[i][j] = intermediate[i][j] * D_inv_sqrt[j][j];
        }
    }


    for (i = 0; i < n; i++) {
        free(D_inv_sqrt[i]);
        free(intermediate[i]);
        free(A[i]);
        free(D[i]);
    }
    free(D_inv_sqrt);
    free(intermediate);
    free(A);
    free(D);

    return W;
}

double** symnmf(double** H,double** W, int k, int n) {
   
   int i,j,iter;
   int MAX_ITER =300;
   double epsilon = 0.0001;

    double** H_prev = H;
    double** H_new = malloc(n * sizeof(double*));

    for ( i = 0; i < n; i++) {
        H_new[i] = malloc(k * sizeof(double));
    }
    

    iter = 0;
    
    while (iter < MAX_ITER) {

        for (i = 0; i < n; i++) {
            for (j = 0; j < k; j++) {
                double numerator = 0.5 * (compute_WH(W, H_prev, i, j, n));
                double denominator = compute_H_Htr_H(H_prev, i, j, n, k);
                H_new[i][j] = H_prev[i][j] * (0.5+(numerator / denominator));
            }
        }


        if (sq_frobenius_norm(H_new, H_prev, n, k) < epsilon) {
            break;
        }

        for ( i = 0; i < n; i++) {
            for (j = 0; j < k; j++) {
              H_prev[i][j] = H_new[i][j]; }
        }

        iter++;
    }

    for (i = 0; i < n; i++) {
        free(H_prev[i]);
    }
    free(H_prev);
    
    return H_new;
}




int main(int argc, char *argv[]) {

    int i;
    int num_of_coordinates;
    int num_of_datapoints;
    double** datapoints_arr = NULL;
    char* goal;
    char *file_name;



    if(argc == 3) {
        goal = argv[1];
        file_name = argv[2];
    }
    else { printf("An Error Has Occurred");
          return 1; 
    }

    if (readDataPointsFromFile(file_name, &datapoints_arr, &num_of_datapoints, &num_of_coordinates) != 0) {
        printf("An Error Has Occurred");
        return 1;
    }  
    

    if(strcmp(goal, "sym") == 0) {
        
       double** A= sym(datapoints_arr, num_of_coordinates, num_of_datapoints);
    
       print_result(A, num_of_datapoints, num_of_datapoints);

       for (i = 0; i < num_of_datapoints; i++) {
           free(A[i]);}
        free(A);

        for (i = 0; i < num_of_datapoints; i++) {
           free(datapoints_arr[i]);}
        free(datapoints_arr);
       
        
    } else if(strcmp(goal, "ddg") == 0) {
        
       double** D= ddg(datapoints_arr, num_of_coordinates, num_of_datapoints);
    
       print_result(D, num_of_datapoints, num_of_datapoints);

        for (i = 0; i < num_of_datapoints; i++) {
           free(D[i]);}
        free(D);

        for (i = 0; i < num_of_datapoints; i++) {
           free(datapoints_arr[i]);}
        free(datapoints_arr);

    } else if(strcmp(goal, "norm") == 0) {

       double** W= norm(datapoints_arr, num_of_coordinates, num_of_datapoints);
    
       print_result(W, num_of_datapoints, num_of_datapoints);

        for (i = 0; i < num_of_datapoints; i++) {
           free(W[i]);}
        free(W);

        for (i = 0; i < num_of_datapoints; i++) {
           free(datapoints_arr[i]);}
        free(datapoints_arr);

    }else {
        printf("Invalid goal. Valid options are: sym, ddg, norm\n");
        exit(1);
    }

    return 0;
}
