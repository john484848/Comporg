#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct
{
    char * instuction;
    int readreg[2];
    int outputreg;
    int stage;
    int reapet;
    int fill;
    int repstage;
    int nopcycle;
} node;
// creats a struct for nodes of a vector
typedef struct
{
    node *vectorl;
    int size;
    int count;
} vector;
//creats a vector struct 
void construct_vector(vector *vec)
{
    vec->size = 2;
    vec->vectorl = malloc(vec->size * sizeof(node));
    vec->count = 0;
}
void push_back(vector *vec, char *instuction2, int read12,int read22,int outputreg2,int stage,int fill,int repstage)
{
    if (vec->count >= vec->size)
    {
        vec->size *= 2;
        vec->vectorl = realloc(vec->vectorl, vec->size * sizeof(node));
    }
    vec->vectorl[vec->count].instuction = instuction2;
    vec->vectorl[vec->count].outputreg = outputreg2;
    vec->vectorl[vec->count].readreg[0] = read12;
    vec->vectorl[vec->count].readreg[1] = read22;
    vec->vectorl[vec->count].stage = stage;
    vec->vectorl[vec->count].fill = fill;
    vec->vectorl[vec->count].repstage = repstage;
    vec->count++;
    //pushes the value to the back
}
void dealloc_vec(vector *vec) { free(vec->vectorl); }
void insert(vector *q,node* n){
    push_back(q,n->instuction,n->readreg[0],n->readreg[1],n->outputreg,n->stage,n->fill,n->repstage);
    //just a frame work to use push back
}
int size(vector *v){return v->count;}
//returns the size
void insert2(vector *q,node* n,int index){
    q->count++;
     if (q->count >= q->size)
    {
        q->size *= 2;
        q->vectorl = realloc(q->vectorl, q->size * sizeof(node));
    }
    //checks for resize
    for(int i=q->count;i>=index;i--){
        memcpy(&(q->vectorl[i]),&(q->vectorl[i-1]),sizeof(node));
    }
    //shifts the values
    
    memcpy(&(q->vectorl[index]),n,sizeof(node));
    //adds the next one at the position
}