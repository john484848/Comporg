#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>
#include "ds.h"
void remove_spaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while ((*s++ = *d++));
}
void print_line(node * v,int cycle,int foundif){
    if(foundif!=1){
        printf("%s ",v->instuction);
            for(int i=0;i<cycle;i++){
                printf(".\t");
            }
            for(int i=0;i<=v->stage;i++){
                switch (i){
                case 0:
                    printf("IF\t");
                    break;
                case 1:
                    printf("ID\t");
                    break;
                case 2:
                    printf("EX\t");
                    break;
                case 3:
                    printf("MEM\t");
                    break;
                case 4:
                    printf("WB\t");
                    break;
                }
            }
            for(int i=9-v->stage-cycle;i>1-foundif;i--){
                printf(".\t");
            }
            printf("\n");
    }
    else{
        printf("%s ",v->instuction);
        for(int i=0;i<9;i++){
            printf(".\t");
        }
        printf("\n");
    }

}
int main(int argc, char * argv[]){
    FILE * f=fopen(argv[1],"r");
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    ssize_t line_size;
    char sep[] = {','};
    queue q;
    //queue q2;
    construct_queue(&q);
    line_size = getline(&line_buf, &line_buf_size, f);
    while(line_size>=0){
        line_buf[line_size-1]='\0';
        char * cpy=malloc(sizeof(char)*line_size);
        strcpy(cpy,line_buf);
        char * cpy2=malloc(sizeof(char)*line_size);
        strcpy(cpy2,line_buf);
        char * up=strchr(cpy,' ')+1;
        remove_spaces(up);
        char *token;
        int i=0;
        int reg[3]={-1,-1,-1};
        token = strtok(up, sep);
        if(token[0]=='$'){
            reg[i]=(int)token[1]+token[2]-'0';
        }
        i++;
        while (token != NULL){
            token = strtok(NULL, sep);
            if(token!=NULL && token[0]=='$'){
                reg[i]=(int)token[1]+token[2]-'0';
            }
            i++;
        }
        node n;
        n.instuction=cpy2;
        n.readreg[0]=reg[1];
        n.readreg[1]=reg[2];
        n.outputreg=reg[0];
        n.stage=0;
        insert(&q,&n); 
        line_size = getline(&line_buf, &line_buf_size, f);
        free(cpy);
    }
    printf("START OF SIMULATION\n\n");
    int finish=0;
    while(finish!=size(&q)){
        int cycle=0;
        int foundif=0;
        finish=0;
        printf("CPU Cycles ===> 1\t2\t3\t4\t5\t6\t7\t8\t9\n");
        for(int g=0;g<size(&q);g++){
            node * n= removeData(&q);
            print_line(n,cycle,foundif);
            cycle+=1;
            if(n->stage<5 && foundif==0){
                n->stage++;
            }
            //printf("%d\n",n->stage);
            if(n->stage==5){
                finish++;
            }
            if(n->stage==1){
                foundif=1;   
            }
            insert(&q,n);
        }
        printf("\n");
        
    }
    printf("END OF SIMULATION\n");
}