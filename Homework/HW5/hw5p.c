#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#define TRUE 1
typedef struct
{
    char name[2];
    int register0;
} node;

typedef struct
{
    node *vectorl;
    int size;
    int count;
} vector;
void construct_vector(vector *vec)
{
    vec->size = 2;
    vec->vectorl = malloc(vec->size * sizeof(node));
    vec->count = 0;
}
void push_back(vector *vec, char *namein, int datain)
{
    if (vec->count >= vec->size)
    {
        vec->size *= 2;
        vec->vectorl = realloc(vec->vectorl, vec->size * sizeof(node));
    }
    strcpy(vec->vectorl[vec->count].name, namein);
    vec->vectorl[vec->count].register0 = datain;
    vec->count++;
}
void dealloc_vec(vector *vec) { free(vec->vectorl); }
int main(int argc, char *argv[])
{
    int num = 0;
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size;
    FILE *fp = fopen(argv[1], "r");
    vector v;
    construct_vector(&v);
    line_size = getline(&line_buf, &line_buf_size, fp);
     while(line_size>=0)
    {
        int num2 = 0;
        printf("#%s", line_buf);
        char *name = line_buf;
        line_buf[1] = '\0';
        line_buf = line_buf + 4;
        size_t size = strlen(line_buf);
        line_buf[size - 2] = '\0';
        line_buf[size - 1] = ' ';
        char sep[] = " ";
        char **list = malloc(sizeof(char *) * 260);
        char *token;
        token = strtok(line_buf, sep);
        int i = 0;
        list[i] = token;
        size_t size2 = strlen(list[i]);
        i++;
        while (token != NULL)
        {
            token = strtok(NULL, sep);
            list[i] = token;
            i++;
        }
        if(i<3){
            if (isdigit(list[0][0]) || list[0][0] == '-')
        {
            printf("li $s%d,%s\n", num, list[0]);
            push_back(&v, name, num);
            num++;
        }
        else
        {
            for (int g = 0; g < v.count; g++)
            {
                if (list[0][0] == v.vectorl[g].name[0])
                {
                    printf("move $s%d,$s%d\n", num, v.vectorl[g].register0);
                }
            }
            push_back(&v, name, num);
            num++;
        }
        }
        int s=num2;
        int pos=1;
        num2++;
        if(i<7 && i>4){
            pos+=2;
            if(list[1][0]=='+'){
                if (isdigit(list[2][0]) || list[2][0] == '-')
                {
                    for (int q = 0; q < v.count; q++)
                    {
                        if (list[0][0] == v.vectorl[q].name[0])
                        {
                            printf("addi $t%d,$s%d,%s\n", s,v.vectorl[q].register0,list[2]);
                        }
                    }
                }
                else{
                    int r1;
                    int r2;
                    for (int q = 0; q < v.count; q++)
                    {
                        if (list[0][0] == v.vectorl[q].name[0])
                        {
                            r1=q;
                        }
                        else if (list[2][0]==v.vectorl[q].name[0])
                        {
                            r2=q;
                        }   
                    }
                    printf("add $t%d,$s%d,$s%d\n", s,r1 ,r2);

                }

            }
            if(list[1][0]=='-'){
                if (isdigit(list[2][0]) || list[2][0] == '-')
                {
                    for (int q = 0; q < v.count; q++)
                    {
                        if (list[0][0] == v.vectorl[q].name[0])
                        {
                            printf("addi $t%d,$s%d,-%s\n", s,v.vectorl[q].register0,list[2]);
                        }
                    }
                }
                else{
                    int r1;
                    int r2;
                    for (int q = 0; q < v.count; q++)
                    {
                        if (list[0][0] == v.vectorl[q].name[0])
                        {
                            r1=q;
                        }
                        else if (list[2][0]==v.vectorl[q].name[0])
                        {
                            r2=q;
                        }   
                    }
                    printf("sub $t%d,$s%d,$s%d\n", s,r1 ,r2);

                }

            }
            if(list[1][0]=='*'){
                if (isdigit(list[2][0]) || list[2][0] == '-')
                {
                    for (int q = 0; q < v.count; q++)
                    {
                        if (list[0][0] == v.vectorl[q].name[0])
                        {
                            printf("addi $t%d,$s%d,-%s\n", s,v.vectorl[q].register0,list[2]);
                        }
                    }
                }
                else{
                    int r1;
                    int r2;
                    for (int q = 0; q < v.count; q++)
                    {
                        if (list[0][0] == v.vectorl[q].name[0])
                        {
                            r1=q;
                        }
                        else if (list[2][0]==v.vectorl[q].name[0])
                        {
                            r2=q;
                        }   
                    }
                    printf("mul $t%d,$s%d,$s%d\n", s,r1 ,r2);

                }

            }
        }
        else{
            s=13;
        }
        
        for (int g = 3; g < i - 4; g = g + 2, pos+=2)
        {
            if (list[g][0] == '+')
            {
                if (isdigit(list[g+1][0]) || list[g+1][0] == '-')
                {
                    printf("addi $t%d,$t%d,%s", s, s, list[g+1]);
                }
                else
                {
                    for (int q = 0; q < v.count; q++)
                    {
                        if (list[g+1][0] == v.vectorl[q].name[0])
                        {
                            printf("add $s%d,$s%d,$s%d\n", num2,num2 ,v.vectorl[q].register0);
                        }
                    }
                }
            }
            if (list[g][0] == '-')
            {
                if (isdigit(list[g+1][0]) || list[g+1][0] == '-')
                {
                    printf("addi $s%d,$s%d,-%s\n", num, num, list[g+1]);
                }
                else
                {
                    for (int q = 0; q < v.count; q++)
                    {
                        if (list[g+1][0] == v.vectorl[q].name[0])
                        {
                            printf("sub $s%d,$s%d,$s%d\n", num,num,v.vectorl[q].register0);
                        }
                    }
                }
            }
            if (list[g][0] == '*')
            {
                if (isdigit(list[g+1][0]) || list[g+1][0] == '-')
                {
                    printf("li $t%d,%s\n",num2,list[g+1]);
                    printf("mult $s%d,$t%s\n",num-1,num2-1);
                    printf("mflo $s%d\n",num-1);
                }
                else
                {
                    for (int q = 0; q < v.count; q++)
                    {
                        if (list[g+1][0] == v.vectorl[q].name[0])
                        {
                            printf("mult $s%d,$s%d\n", num-1,v.vectorl[q].register0);
                            printf("mflo $s%d\n",num-1);
                        }
                    }
                }
            }
            if (list[g][0] == '/')
            {
                if (isdigit(list[g+1][0]) || list[g+1][0] == '-')
                {
                    printf("li $t%d,%s\n",num2,list[g+1]);
                    printf("div $s%d,$t%s\n",num-1,num2-1);
                    printf("mflo $s%d\n",num-1);
                }
                else
                {
                    for (int q = 0; q < v.count; q++)
                    {
                        if (list[g+1][0] == v.vectorl[q].name[0])
                        {
                            printf("div $s%d,$s%d\n", num-1,v.vectorl[q].register0);
                            printf("mflo $s%d\n",num-1);
                        }
                    }
                }
            }
            if (list[g][0] == '%')
            {
                if (isdigit(list[g+1][0]) || list[g+1][0] == '-')
                {
                    printf("li $t%d,%s\n",num2,list[g+1]);
                    printf("div $s%d,$t%s\n",num-1,num2-1);
                    printf("mfhi $s%d\n",num-1);
                }
                else
                {
                    for (int q = 0; q < v.count; q++)
                    {
                        if (list[g+1][0] == v.vectorl[q].name[0])
                        {
                            printf("div $s%d,$s%d\n", num-1,v.vectorl[q].register0);
                            printf("mfhi $s%d\n",num-1);
                        }
                    }
                }
            }
        }
        if(i>2){
            if(list[pos][0]=='+'){
                if (isdigit(list[pos+1][0]) || list[pos+1][0] == '-')
                {
                    for (int q = 0; q < v.count; q++)
                    {
                        if (list[pos+1][0] == v.vectorl[q].name[0])
                        {
                            printf("addi $s%d,$t%d,%s\n",num ,s,list[pos+1]);
                            push_back(&v, name, num);
                            num++;
                        }
                    }
                }
                else{
                    int r1;
                    int r2;
                    for (int q = 0; q < v.count; q++)
                    {
                        if (list[pos+1][0] == v.vectorl[q].name[0])
                        {
                            r1=q;
                        }   
                    }
                    printf("add $s%d,$t%d,$s%d\n",num, s,r1);
                    push_back(&v, name, num);
                    num++;

                }
                
        }
        if(list[pos][0]=='-'){
                if (isdigit(list[pos+1][0]) || list[pos+1][0] == '-')
                {
                    for (int q = 0; q < v.count; q++)
                    {
                        if (list[pos-1][0] == v.vectorl[q].name[0])
                        {
                            printf("addi $s%d,$t%d,-%s\n",num ,s,list[pos+1]);
                            push_back(&v, name, num);
                            num++;
                        }
                    }
                }
                else{
                   int r1;
                    int r2;
                    for (int q = 0; q < v.count; q++)
                    {
                        if (list[pos+1][0] == v.vectorl[q].name[0])
                        {
                            r1=q;
                        }   
                    }
                    printf("sub $s%d,$t%d,$s%d\n",num, s,r1);
                    push_back(&v, name, num);
                    num++;
                }
                
        }

        }
        

        line_size = getline(&line_buf, &line_buf_size, fp);
    }
}