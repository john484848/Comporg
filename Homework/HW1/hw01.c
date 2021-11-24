/* 
Assignment 1: NxM Matrix Multiplication 
CSCI-2500 Fall 2021
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>


/******************************************************************************/
/* Function Calls */
/******************************************************************************/

typedef struct{
  double** data;
  int rows;
  int cols;
} matrix; 
int mm_alloc(matrix* mat);
int mm_free (matrix* mat);
int mm_print(matrix* mat);
int mm_read (char* filename, matrix* mat);

int mm_alloc(matrix* mat)
{

 if (mat->rows < 1 || mat->cols < 1){
    printf("mm_alloc: allocation failed.");
    exit(1);
  }
  //deals with invalid dimensions
  else{
    mat->data = (double **) calloc(mat->rows, sizeof(double *));
    if (mat->data == NULL){
      printf("mm_alloc: allocation failed.");
      exit(1);
    }
    //allocates the first part of the array and does error checking 
    for (int i = 0; i < mat->rows; i++){
      mat->data[i] = (double *) calloc(mat->cols, sizeof(double));
      if(mat->data[i]==NULL){
        printf("mm_alloc: allocation failed.");
        exit(1);
      }

    }
    //alloctes the 2nd dimension of the array and makes sure it gets allocated 
  }
/*
Notes:
  Allocate an array of pointers to a double
  Then, allocate an row/array of doubles and assign each double pointer that row
  Make sure you're allocating using mat->data in the matrix struct
  You can use calloc for allocation and zeroing out all the bytes
  - otherwise, you'd want to manually set all arrays to 0 (good practice)
  If your malloc/calloc returns null, the allocation failed
*/
  
  return 0;
}

int mm_free(matrix* mat)
{
  for(int i=0;i<mat->rows;i++){
    free(mat->data[i]);
  }
  //frees each sub array 
  free(mat->data);
  //frees big array 
/*
Notes:
  Reverse the allocation process, but you'll be calling free() instead
*/
  
  return 0;
}

int mm_print(matrix* mat)
{
  printf("\n/******** START of 2-D Matrix **********************************/\n");
  //prints top
  for(int i=0;i<mat->rows;i++){
    for(int b=0;b<mat->cols;b++){
      printf("%10.2lf\t",mat->data[i][b]);
      //prints the value 
    }
    printf("\n");
    //prints \n
  }
  printf("/******** END of 2-D Matrix ************************************/\n");
  //prints bottom
/*
Notes:
  Print all the data values in your matrix
  Go row by row
  At the end of each row, print a newline
  Between each data value, insert a tab
  Use 2 digits of precision after the decimal (use "%10.2lf\t" as format string)
  See output.txt for expected output formatting
*/
  
  return 0;
}

int mm_read(char* filename, matrix* mat) 
{
  FILE* file=fopen(filename,"r");
  //opens file 
  
  if (file == NULL){
    printf("mm_read: failed to open file.");
    exit(1);
  }
  //makes sure the file opened 
  else {
    char first[256];
    if (fgets(first, 256, file)==NULL) {
    printf("mm_read: failed to read from file.\n");
    exit(-1);
  }
  //uses fgets for the first line to check if its empty 
  
  if (2 != sscanf(first, "%d %d", &mat->rows, &mat->cols)) {
    printf("mm_read: failed to read matrix dimensions.\n");
    exit(-1);
  }
  //makes sure there is enough numbers for the dimensions 
  int count=0;
  mm_alloc(mat);
  for(int i = 0; i < mat->rows; i++){
    for(int j = 0; j < mat->cols; j++){
      if(fscanf(file, "%lf", & (mat->data[i][j]))!=1){
        printf("mm_read: failed to read matrix values.");
        exit(1);
      }
      //makes sure that you red values in and assigned them to the array
      count++;
    }
  }
    if(count<mat->cols*mat->rows){
      printf("mm_read: failed to read matrix values.");
      exit(1);
    }
    //makes sure you assigned enough values 
  }
  
  
/*
Notes:
  Use fopen to open the matrix file, then use fgets or getline to read lines
  The first line of the matrix contains integers for "#rows #columns"
  - Use these to allocate your arrays for matrix data
  Each subsequent line contains a single data value
  - You can use sscanf to parse each matrix entry
  - First value is data[0][0], next is data[0][1] ... data[#rows-1][#columns-1]
*/
  fclose(file);
  return 0;
}

matrix* mm_matrix_mult(matrix* mat1, matrix* mat2)
{
   matrix* result_matrix = (matrix*)malloc(sizeof(matrix));
   if(mat1->cols==mat2->rows){
     result_matrix->rows=mat1->rows;
     result_matrix->cols=mat2->cols;
     //sets rows and columns for result matrix 
     mm_alloc(result_matrix);
     //allocates result matrix
     for(int i=0;i<mat1->rows;i++){
       for(int b=0;b<mat2->cols;b++){
         double result=0;
         //goes through each row and column 
         for(int c=0;c<mat1->cols;c++){
           result+=mat1->data[i][c] * mat2->data[c][b];
         }
         //multiplies each row by every column 
         result_matrix->data[i][b]=result;
         //adds it to the result matrix

       }
     }

   }
   else{
     printf("mm_matrix_mult: dimension mismatch between matrices.");
     mm_free(mat1);
     mm_free(mat2);
     free(mat1);
     free(mat2);
     free(result_matrix);
     exit(1);

   }
   //in the event of incompadible matrix dimensions frees all the memory



/*
Notes:
  First check that it is actually possible to multiply the two inputs
  If so, construct a result matrix
  Then perform the multiplication
  Then return a pointer to your result matrix
*/
  return result_matrix;
}
int main()
{
  /*
   * you must keep this function AS IS. 
   * you cannot modify it!
   */
  char filename_A[256];
  char filename_B[256];
  matrix* A = (matrix*)malloc(sizeof(matrix));
  matrix* B = (matrix*)malloc(sizeof(matrix));
  matrix* C = NULL;

  printf("Please enter the matrix A data file name: ");
  if (1 != scanf("%s", filename_A)) {
    printf("main: scanf failed to read filename\n");
    exit(-1);
  }
  mm_read(filename_A, A);
  mm_print(A);
  
  printf("Please enter the matrix B data file name: ");
  if (1 != scanf("%s", filename_B)) {
    printf("main: scanf failed to read filename\n");
    exit(-1);
  }
  mm_read(filename_B, B);
  mm_print(B);
  
  C = mm_matrix_mult(A, B);
  mm_print(C);
  mm_free(A);
  mm_free(B);
  mm_free(C);
  free(A);
  free(B);
  free(C);
  return 0;
}
