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
void push_back(vector *vec, char *instuction2, int read12,int read22,int outputreg2,int stage)
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
    vec->count++;
}
void dealloc_vec(vector *vec) { free(vec->vectorl); }
typedef struct{
    vector v;
    int front;
    int rear;
    int itemCount;
}queue;
bool isEmpty(queue *q){return q->itemCount==0;}
int size(queue *q){return q->itemCount;}
void construct_queue(queue *q){
    construct_vector(&q->v);
    q->front=0;
    q->rear=-1;
    q->itemCount=0;
}
void insert(queue *q,node* n){
    push_back(&q->v,n->instuction,n->readreg[0],n->readreg[1],n->outputreg,n->stage);
    q->rear++;
    q->itemCount++;
}
node * removeData(queue *q){
    q->v.count=q->front;
    q->itemCount--;
    if(q->front==q->itemCount+1){
        q->front=0;
    }
    return &q->v.vectorl[q->front++];
}
