#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
/*
1. Given unsigned int variable n, write exactly one malloc() call to dynamically allocate the exact amount of memory needed to create an array containing exactly n double values. 
Call this array a and be sure to properly declare this variable. Set all values in a to 1.0 and then free up the allocated memory.
*/
/*
int main(){
    unsigned int n=12;
    double * a=malloc(sizeof(double)*n);
    for(unsigned int i=0;i<n;i++){
        a[i]=1.0;
    }
    free(a);
}
*/
/*
2. The code below outputs several memory addresses with printf(). For each output, identify if the printed address would be a location in the heap, stack, or text/code segments of process memory.
stack
heap
heap
text
stack
*/
typedef struct {
    char * name;
    char something;
    int something2;
    double something3;
    int something4;
} my_data;

my_data* read_data(char* filename)
{
    FILE* f=fopen(filename,"r");
    int count=0;
    int size=1;
    my_data* data=malloc(1*sizeof(data));
  // allocate a struct array
  while(fscanf(f, "%s %c %d %f %d",data[count].name,data[count].something,data[count].something2,data[count].something3,data[count].something4)!=EOF){
      count++;
      if(count>=size){
          size++;
          data=realloc(data,size);
      }
  }
  fclose(f);
  return data;
  // read in data line by line
  // fill in the array
  // return it
}
/*
4. Assume we have a function filesize(FILE* fp) that tells us the size of a file in bytes. What might the expected values of x and y from the code snippet below be if we assume one byte per character in an ASCII text files?

char* a = "Woo!";
FILE* fp1 = fopen("file1", "w");
FILE* fp2 = fopen("file2", "w");
fprintf(fp1, "%s", a);
fwrite(a, 1, 4, fp2);
int x = filesize(fp1);
int y = filesize(fp2);

x and y are 4
*/

