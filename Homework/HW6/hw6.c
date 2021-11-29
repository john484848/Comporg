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

void parse_file(FILE* f,vector * q){
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    ssize_t line_size;
    char sep[] = {','};
    line_size = getline(&line_buf, &line_buf_size, f);
    while(line_size>=0){
        line_buf[line_size-1]='\0';
        char * cpy=malloc(sizeof(char)*line_size+1);
        strcpy(cpy,line_buf);
        char * cpy2=malloc(sizeof(char)*line_size+1);
        strcpy(cpy2,line_buf);
        char * up=strchr(cpy,' ')+1;
        remove_spaces(up);
        //creats necissary copies 
        char *token;
        int i=0;
        int reg[3]={-1,-1,-1};
        token = strtok(up, sep);
        if(token[0]=='$'){
            reg[i]=(int)token[1]*token[2]-'0';
        }
        char *s=strchr(token,'$');
        if(s!=NULL){
            reg[i]=(int)*(s+1)*token[2]-'0';

        }
        i++;
        //hashes the first register
        while (token != NULL){
            token = strtok(NULL, sep);
            if(token!=NULL && token[0]=='$'){
                reg[i]=(int)token[1]*token[2]-'0';
            }
            else if(token!=NULL){
            char *s=strchr(token,'$');
            if(s!=NULL){
                reg[i]=(int)*(s+1)*(*(s+2))-'0';
            }
            }
            i++;
        }
        //tokenizes and hashes all the other regiserts
        node n;
        n.instuction=cpy2;
        n.readreg[0]=reg[1];
        n.readreg[1]=reg[2];
        n.outputreg=reg[0];
        n.stage=0;
        n.fill=0;
        n.repstage=0;
        insert(q,&n); 
        //creats and add the node to the vector 
        line_size = getline(&line_buf, &line_buf_size, f);
        //gets the line 
        free(cpy);
        //frees extra memory
    }

}
void print_line(node * v,int cycle,int foundif){
    if(foundif!=1){
        printf("%s",v->instuction);
        //prints the instruction
            for(int i=0;i<cycle;i++){
                printf("\t.");
            }
            //prints all the dots befroe the stage 
            for(int i=0;i<=v->stage;i++){
                if(i==v->repstage){
                    for(int e=0;e<v->fill;e++){
                switch (v->repstage){
                case 0:
                    printf("\tIF");
                    break;
                case 1:
                    printf("\tID");
                    break;
                case 2:
                    printf("\tEX");
                    break;
                case 3:
                    printf("\tMEM");
                    break;
                case 4:
                    printf("\tWB");
                    break;
                }
            }
            //prints the repeated stages from an nop

                }
                switch (i){
                case 0:
                    printf("\tIF");
                    break;
                case 1:
                    printf("\tID");
                    break;
                case 2:
                    printf("\tEX");
                    break;
                case 3:
                    printf("\tMEM");
                    break;
                case 4:
                    printf("\tWB");
                    break;
                }
                
            }
            //prints all the usual stages 
            int q;
            if(v->stage>=5){
                q=4;
            }
            else{
                q=v->stage;
            }
            for(int i=9-q-cycle-(v->fill);i>1-foundif;i--){
                printf("\t.");
            }
            //prints all the dots at the end of the line. 
            printf("\n");
    }
    else{
        printf("%s",v->instuction);
        for(int i=0;i<9;i++){
            printf("\t.");
        }
        printf("\n");
    }
    //prints the remaining instructions if an nop has been executed 

}
void printnop(node * n,int cycle){
    printf("%s\t",n->instuction);
    for(int i=0;i<cycle;i++){
                printf("\t.");
    }
    //prints all the begining dots 
    for(int i=0;i<=n->outputreg;i++){
        if(i==n->repstage){
            for(int e=0;e<n->fill;e++){
                switch (n->repstage){
                case 0:
                    printf("\tIF");
                    break;
                case 1:
                    printf("\tID");
                    break;
                case 2:
                    printf("\tEX");
                    break;
                case 3:
                    printf("\tMEM");
                    break;
                case 4:
                    printf("\tWB");
                    break;
                }
            }
        }
        //prints all the repeated stages 
        switch (i){
            case 0:
                printf("\tIF");
                break;
            case 1:
                printf("\tID");
                break;
            case 2:
                printf("\tEX");
                break;
            case 3:
                printf("\tMEM");
                break;
            case 4:
                printf("\tWB");
                break;
        }
        //prints all the regular stages 
    }
    int q=0;
            if(n->stage>=3){
                q=3;
            }
            else{
                q=n->stage;
            }
    for(int i=0;i<q;i++){
        printf("\t*");
    }
    //prints the stars 
    for(int i=9-q-cycle-n->outputreg-(n->fill);i>1;i--){
                printf("\t.");
            }
            printf("\n");
    //prints the dots at the end 
}
int main(int argc, char * argv[]){
    FILE * f=fopen(argv[1],"r");
    vector q;
    construct_vector(&q);
    parse_file(f,&q);
    int finish=0;
    printf("START OF SIMULATION\n\n");
    //prints the header and parses the file 
    while (finish<size(&q)){
        int foundif=0;
        int cycle=0;
        int bf=0;
        //the finish found an if and bouble found values being initalized
        printf("CPU Cycles ===>\t1\t2\t3\t4\t5\t6\t7\t8\t9\n");
        //printing the header
        for(int i=0; i<size(&q);i++){
            for(int g=0;g<i;g++){
                    if(((q.vectorl[i].readreg[0]!=-1 && q.vectorl[i].readreg[0]==q.vectorl[g].outputreg) || (q.vectorl[i].readreg[1]!=-1 && q.vectorl[i].readreg[1]==q.vectorl[g].outputreg)) && (q.vectorl[i].stage==1 && bf==0 && q.vectorl[g].stage<=5) ){
                    //checks for an nop
                    node d;
                    int point=i+1;
                    d.instuction=malloc(sizeof(char)*4);
                    d.instuction[0]='n';
                    d.instuction[1]='o';
                    d.instuction[2]='p';
                    d.instuction[3]='\0';
                    d.outputreg=1;
                    d.stage=1;
                    d.readreg[0]=-1;
                    d.readreg[1]=-1;
                    d.fill=q.vectorl[i].fill;
                    d.repstage=1;
                    //creats an nop
                    for(int t=i;t<size(&q);t++){
                        q.vectorl[t].repstage=q.vectorl[t].stage;
                    }
                    //sets all the repeated stages 
                    int vr=i;
                    while (vr>0){
                        if(q.vectorl[vr].readreg[0]!=q.vectorl[i].readreg[0]){
                            break;
                        }
                        point=vr;
                        vr--;
                        
                    }
                    //determines the point to insert it 
                    insert2(&q,&d,point);
                    //inserts it 
                    bf=1;
                    i++;
                    //incriments i and sets buble found to halt execution 
                    break;
                    }
                    else if(q.vectorl[i].readreg[1]<0 && strcmp(q.vectorl[i].instuction,"nop")==0){
                        for(int e=i+1;e<size(&q);e++){
                                q.vectorl[e].fill++;
                        }
                        //increases the fill value to help move execusion forward 
                        break;
                    }
            }
            
            if(strcmp(q.vectorl[i].instuction,"nop")==0){
                printnop(&q.vectorl[i],cycle);
                if(q.vectorl[i].stage<3){
                    q.vectorl[i].stage++;
                    q.vectorl[i].readreg[1]++;
                }
                if(q.vectorl[i].stage==3){
                    q.vectorl[i].stage++;
                    finish++;
                    q.vectorl[i].readreg[1]++;;
                }
                //updates nop instructions 
            }
            else{
                print_line(&q.vectorl[i],cycle,foundif);
                if(q.vectorl[i].stage<5 && foundif==0 && bf==0){
                    if(q.vectorl[i].stage==0){
                        foundif=1;
                    }
                q.vectorl[i].stage++;
                }
                if(q.vectorl[i].stage==5){
                    q.vectorl[i].stage++;
                    finish++;
                }
                cycle++;
                }
                //updates reegular instructions
        }
        printf("\n");
    }
     printf("END OF SIMULATION\n");
}