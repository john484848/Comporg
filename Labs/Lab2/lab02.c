#include <stdio.h>
#include <stdlib.h>


void checkpoint2(char* filename)
{
  FILE* file=fopen(filename,"r");
  // open file  
  fseek(file, 0L, SEEK_END);
  long int res = ftell(file)/sizeof(long);
  // get size of file
  long* arr=calloc(res,sizeof(long));
  // allocate appropriate amount of space for parsing the file as longs
  fseek(file, 0L, SEEK_SET);
  // read in file
  fread(arr,res,sizeof(long),file);
  // output parsed contents
  for(int i=0;i<res;i++){
    printf("Data point # %d %2d\n",i,arr[i]);
  }
  printf("\n");
  fclose(file);
  // close file
}

void checkpoint3(char* filename)
{
  FILE* file=fopen(filename,"r");
  // open file  
  fseek(file, 0L, SEEK_END);
  long int res = ftell(file)/sizeof(unsigned int);
  // get size of file
  unsigned int* arr=calloc(res,sizeof(unsigned int));
  // allocate appropriate amount of space for parsing the file as longs
  fseek(file, 0L, SEEK_SET);
  // read in file
  fread(arr,res,sizeof(unsigned int),file);
  // output parsed contents
  for(int i=0;i<res;i++){
    printf("Data point # %d %u\n",i,arr[i]);
  }
  fclose(file);
  // close file
}


int main()
{
  char filename[256];
  scanf("%s", filename);
  
  checkpoint2(filename);
  checkpoint3(filename);
  
  return 0;
}
//the file contains prime numbers 