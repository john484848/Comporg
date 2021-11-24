#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#define TRUE 1
typedef struct{
    char name[260];
    int data;
}node;

typedef struct{
    char title[260];
    char coltitle1[260];
    char coltitle2[260];
    node * vectorl;
    int size;
    int count;
}vector;
void construct_vector(vector * vec, char * titlein,char * c1,char * c2){
    vec->size=2;
    vec->vectorl=malloc(vec->size*sizeof(node));
    strcpy(vec->title,titlein);
    vec->count=0;
    strncpy(vec->coltitle1,c1,strlen(c1)-1);
    strncpy(vec->coltitle2,c2,strlen(c2)-1);
    free(titlein);
    free(c1);
    free(c2);
}
void push_back(vector * vec, char * namein,int datain){
    if(vec->count>=vec->size){
        vec->size*=2;
        vec->vectorl=realloc(vec->vectorl,vec->size*sizeof(node));
    }
    strcpy(vec->vectorl[vec->count].name,namein);
    vec->vectorl[vec->count].data=datain;
    vec->count++;
    free(namein);
}
void display(vector * v){
    printf("\n");
    printf(" %33s",v->title);
    printf("%-20s|  %21s\n",v->coltitle1,v->coltitle2);
    printf("--------------------------------------------\n");
    for(int i=0;i<v->count;i++){
        printf("%-20s|  %21d\n",v->vectorl[i].name,v->vectorl[i].data);
    }
    printf("\n"); 
    for(int i=0;i<v->count;i++){
        printf("%20s ",v->vectorl[i].name);
        for(int c=0; c<v->vectorl[i].data; c++){
            printf("*");
        }
        printf("\n");
    }

}
void dealloc_vec(vector * vec){free(vec->vectorl);}
int main(){
    vector v; 
    char * title=NULL;
    char * c1=NULL;
    char * c2=NULL;
    size_t len=0;
    printf("Enter a title for the data:");
    getline(&title,&len, stdin);
    printf("\nYou entered: %s",title);

    printf("\nEnter the column 1 header:");
    getline(&c1,&len, stdin);
    printf("\nYou entered: %s",c1);
    printf("\nEnter the column 2 header:");
    getline(&c2,&len, stdin);
    printf("\nYou entered: %s\n",c2);
    construct_vector(&v,title,c1,c2);
    while(TRUE){
        char* input=NULL;
        size_t len=0;
        int data=0;
        int er=0;
        printf("Enter a data point (-1 to stop input):");
        printf("\n");
        getline(&input,&len, stdin);
        if(strcmp(input,"-1\n")==0){
            free(input);
            break;
        }
        if(strchr(input,',')==NULL){
            printf("Error: No comma in string.\n\n");
            free(input);
            continue;
        }
        int count=0;
        for(size_t i=0;i<len;i++){
            if(input[i]==','){
                count++;
            }
        }
        if(count>1){
            printf("Error: Too many commas in input.\n\n");
            free(input);
            continue;
        }
        char sep[] = ",";
        char* token;

        token = strtok (input, sep);
        while (token != NULL)
        {
            er=sscanf (token, "%d", &data);

            token = strtok (NULL, sep);
        }
        if(er!=1){
            printf("Error: Comma not followed by an integer.\n\n");
            free(token);
            continue;

        }
        printf("Data string: %s\nData integer: %d",input,data);
        printf("\n");
        printf("\n");
        push_back(&v,input,data);
    }
    display(&v);
    dealloc_vec(&v);
}

