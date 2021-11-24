#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>
#define TRUE 1
typedef struct
{
    char name[2];
    int register0;
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
//sets member variable 
void remove_spaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while ((*s++ = *d++));
}
//removes all the spaces by shifting charachters 
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
//adds things to the vector 
void dealloc_vec(vector *vec) { free(vec->vectorl); }
int isPowerOfTwo (int x)
{
 unsigned int numberOfOneBits = 0;

 while(x && numberOfOneBits <=1)
   {
    if ((x & 1) == 1)  
      numberOfOneBits++;
    x >>= 1;        
   }

 return (numberOfOneBits == 1); 
}
//detects powers of two 
void simple_one_liner(char ** l,vector * vec, int *num, char* name){
    int data;
    if(sscanf(l[1],"%d",&data)==1){
        if(l[0][0]=='-'){
            data=data*-1;
        }
        printf("li $s%d,%d\n", *num,data);

    }
    else{
        for (int g = 0; g < vec->count; g++)
            {
                if (l[1][0] == vec->vectorl[g].name[0])
                {
                    printf("move $s%d,$s%d\n", *num, vec->vectorl[g].register0);
                }
            }
        }
    push_back(vec, name, *num);
    *num+=1;
}
//handles load immidiate and move 
void add(int pos, char * store, char ** l,char val1,int rnum,vector * vec,int *num2){
    if(*num2>9){
        *num2=0;
    }
    int data;
    if(sscanf(l[pos+2],"%d",&data)==1){
        if(l[pos+1][0]=='-'){
            data=data*-1;
        }
        printf("addi $%s,$%c%d,%d\n", store,val1,rnum,data);
        //detects integers and prints out an addi statement 

    }
    else{
        int f=0;
        for (int g = 0; g < vec->count; g++)
            {
                if (l[pos+2][0] == vec->vectorl[g].name[0])
                {
                    printf("add $%s,$%c%d,$s%d\n", store,val1,rnum,vec->vectorl[g].register0);
                    f=1;
                }
                //prints and add statement if the second argument is able to be found 
            }
        if(!f){
            printf("sub $%s,$%c%d,$s%d\n", store,val1,rnum,vec->vectorl[vec->count-1].register0+1);
            // prints and add statement if the second variable couldnt be found 
        }

        
    }
    if(store[0]=='t'){
            *num2+=1;
        }
    //if the value is being stored in a t register incriment it to denote the usage of a t register 
    
}
//handles the add instructions 
void sub(int pos, char * store, char ** l,char val1,int rnum,vector * vec, int * num2){
    int data;
    if(sscanf(l[pos+2],"%d",&data)==1){
        if(l[pos+1][0]=='-'){
            data=data*-1;
        }
        data=data*-1;
        printf("addi $%s,$%c%d,%d\n", store,val1,rnum,data);

    }
    else{
        int f=0;
        for (int g = 0; g < vec->count; g++)
            {
                if (l[pos+2][0] == vec->vectorl[g].name[0])
                {
                    printf("sub $%s,$%c%d,$s%d\n", store,val1,rnum,vec->vectorl[g].register0);
                    f=1;
                }
            }
        if(!f){
            printf("sub $%s,$%c%d,$s%d\n", store,val1,rnum,vec->vectorl[vec->count-1].register0+1);
        }
    }
    if(store[0]=='t'){
            *num2+=1;
        }
    //if the value is being stored in a t register incriment it to denote the usage of a t register 
}
//handles subtraction instructions 
void mult(int pos, char * store, char ** l,char val1,int rnum,vector * vec,int *num2){
    int data;
    if(sscanf(l[pos+2],"%d",&data)==1){
        int shift=log(data)/log(2.0);
        if(data==0){
            printf("li $%s,%d\n",store,data);
            if(store[0]=='t'){
            *num2+=1;
        }
        }
        //does multiplication by 1 
        else if(data>1){
            printf("sll $t%d,$%c%d,%d\n",*num2,val1,rnum,shift);
            printf("move $t%d,$t%d\n",*num2+1,*num2);
            data=data-pow(2.0,shift); 
            while(data>1){
            shift=log(data)/log(2.0);
            printf("sll $t%d,$%c%d,%d\n",*num2,val1,rnum,shift);
            printf("add $t%d,$t%d,$t%d\n",*num2+1,*num2+1,*num2);
            data=data-pow(2.0,shift); 
        }
        //does multiplication by numbers that arent 1 or 0 
        if(l[pos+1][0]=='-'){
            printf("add $t%d,$t%d,$%c%d\n",*num2+1,*num2+1,val1,rnum);
            printf("sub $%s,$zero,$%c%d\n",store,'t',*num2+1);
        
        }
        //deals with negatives
        else{
            printf("add $t%d,$t%d,$%c%d\n",*num2+1,*num2+1,val1,rnum);
            printf("move $%s,$t%d\n",store,*num2+1);
        }
        //normal positive 
        
        }
        else{
            printf("move $t%d,$%c%d\n",*num2,val1,rnum);
            *num2+=1;
            if(l[pos+1][0]=='-'){
            printf("sub $%s,$zero,$%c%d\n",store,'t',*num2-1);
        }
        //more negative processing with 1  
        else{
           
            printf("move $%s,$t%d\n",store,*num2-1);
        }
        //positive 1 
        }
    }
    else{
        int f=0;
        for (int g = 0; g < vec->count; g++)
            {
                if (l[pos+2][0] == vec->vectorl[g].name[0])
                {
                    f=1;
                    printf("mult $%c%d,$s%d\n", val1,rnum,vec->vectorl[g].register0);
                    printf("mflo $%s\n",store);
                }
            }
            if(!f){
                printf("mult $%c%d,$s%d\n", val1,rnum,vec->vectorl[vec->count-1].register0+1);
                printf("mflo $%s\n",store);
        }
        //two register multiplication 
    }
    if(store[0]=='t'){
            *num2+=1;
        }
        //denotes the use of a t registar when one is used and incriments it 
}
//handles multiplication instructions
void div1(int pos, char * store, char ** l,char val1,int rnum,int * num2,vector * vec,int * lnum){
    int data;
    if(sscanf(l[pos+2],"%d",&data)==1){
        if(l[pos+1][0]=='-'){
            data=data*-1;
        }
        if(data!=1 && data!=-1){
        if(data<0 && isPowerOfTwo(abs(data))){
            printf("bltz $%c%d,L%d\n",val1,rnum,*lnum);
            int shift=log(abs(data))/log(2.0);
            printf("srl $%s,$%c%d,%d\n",store,val1,rnum,shift);
            printf("sub $%s,$zero,$%s\n",store,store);
            printf("j L%d\n",*lnum+1);
            printf("L%d:\n",*lnum);
            *lnum+=2;
        }
        //deals with simplification of negative powers of two 
        else if(data>0 && isPowerOfTwo(abs(data))){
            printf("bltz $%c%d,L%d\n",val1,rnum,*lnum);
            int shift=log(abs(data))/log(2.0);
            printf("srl $%s,$%c%d,%d\n",store,val1,rnum,shift);
            printf("j L%d\n",*lnum+1);
            printf("L%d:\n",*lnum);
            *lnum+=2;

        }
        //deals with the simplification of positive powers of two
        if(data!=0){
            printf("li $t%d,%d\n",*num2,data);
            printf("div $%c%d,$t%d\n", val1,rnum,*num2);
            if(store[0]=='t'){
                store[1]='0'+*num2+1;
            }
            *num2+=1;
            printf("mflo $%s\n",store);
        }
        //deals with non powers of two 
        if(isPowerOfTwo(abs(data)) && data !=0){
            printf("L%d:\n",*lnum-1);   
        }
        //prints the last lable 
        }
        else{
            if(l[pos+1][0]=='-'){
                printf("sub $%s,$zero,$%c%d\n",store,val1,rnum);
            }
            else{
            printf("move $%s,$%c%d\n",store,val1,rnum);
            }
            //division by negative 1
        }
        if(store[0]=='t'){
            *num2+=1;
        }
        //deals with monitoring and calculating t register usage 
    }
    else{
        int f=0;
        for (int g = 0; g < vec->count; g++)
            {
                if (l[pos+2][0] == vec->vectorl[g].name[0])
                {
                    printf("div $%c%d,$s%d\n", val1,rnum,vec->vectorl[g].register0);
                    printf("mflo $%s\n",store);
                    f=1;
                }
            }
            if(!f){
                printf("div $%c%d,$s%d\n", val1,rnum,vec->vectorl[vec->count-1].register0+1);
                printf("mflo $%s\n",store);
        }
        //division by two variables 
    }

}
//processes division 
void mod(int pos, char * store, char ** l,char val1,int rnum,vector * vec,int *num2){
    int data;
    if(sscanf(l[pos+2],"%d",&data)==1){
        if(l[pos+1][0]=='-'){
            data=data*-1;
        }
        printf("li $t%d,%d\n",*num2,data);
        printf("div $%c%d,$t%d\n", val1,rnum,*num2);
        printf("mfhi $%s\n",store);
        //modulo with a constant 

    }
    else{
        int f=0;
        for (int g = 0; g < vec->count; g++)
            {
                if (l[pos+2][0] == vec->vectorl[g].name[0])
                {
                    printf("div $%c%d,$s%d\n", val1,rnum,vec->vectorl[g].register0);
                    printf("mfhi $%s\n",store);
                    f=1;
                }
            }
            //modulo with two registars
        if(!f){
                printf("div $%c%d,$s%d\n", val1,rnum,vec->vectorl[vec->count-1].register0+1);
                printf("mfhi $%s\n",store);
        }
        //if the variable cant be found in the symbol table 
            
        
    }
    if(store[0]=='t'){
            *num2+=1;
        }
    //monitore t registar usage 

}
//processes modulo 
char* first_instruction(char ** l,vector * vec, int *num, int* num2, char* name,int *lnum){
    if(*num2>9){
        *num2=0;
    }
    //resets the t registars 
    char *store=malloc(sizeof(char)*2);
    store[0]='t';
    store[1]='0'+*num2;
    //calculates the registar where the value should be stored 
    char val1='s';
    int rnum;
    for (int g = 0; g < vec->count; g++)
        {
            if (l[1][0] == vec->vectorl[g].name[0])
            {
                rnum=vec->vectorl[g].register0;
            }
        }
    //find first register
    switch (l[2][0])
    {
    case '+':
        add(2,store,l,val1,rnum,vec,num2);
        break;
    case '-':
        sub(2,store,l,val1,rnum,vec,num2);
        break;
    case '*':
        mult(2,store,l,val1,rnum,vec,num2);
        break;
    case '/':
        div1(2,store,l,val1,rnum,num2,vec,lnum);
        break;
    case '%':
        mod(2,store,l,val1,rnum,vec,num2);
        break;
    }
    //switch cases to call all the opperation function 
    if(*num2>9){
        *num2=0;
    }
    //resets t registars 
    store[1]='0'+*num2;
    return store;
    //returns the place where it stashed the data 

}
//process the first instruction if there is more than one 
void mid_instruction(char ** l,vector * vec, int *num, int* num2, char* name, char * store,int i,int * lnum){
     if(*num2>9){
        *num2=0;
    }
    //reset the t registars 
     char *store3=malloc(sizeof(char)*3);
     char store2='&';
     int place=0;
     if(store!=NULL){
        store2=store[0];
        store3[0]=store[0];
        store3[1]=store[1];
        place=store[1]-'0'-1;

     }
     //calculates the storage location 
    for(int c=5;c<i-5;c+=3){
    switch (l[c][0])
    {
    case '+':
        add(c,store3,l,store2,place,vec,num2);
        break;
    case '-':
        sub(c,store3,l,store2,place,vec,num2);
        break;
    case '*':
        mult(c,store3,l,store2,place,vec,num2);
        break;
    case '/':
        div1(c,store3,l,store2,place,num2,vec,lnum);
        break;
    case '%':
        mod(c,store3,l,store2,place,vec,num2);
        break;
        
    }
    if(*num2>9){
        *num2=0;
    }
    //goes through all the middle opperations and uses the switch cases to call the right functions 
    store3[1]='0'+*num2;
    place=*num2-1;
    if(place<0){
        place=9;
    }
    }
}
//deals with the middle instructions 
void last(char ** l,vector * vec, int *num, char* name,int i,char * storloc,int * num2,int *lnum){
    if(*num2>9){
        *num2=0;
    }
    //resets the t registar count 
    char *store=malloc(sizeof(char)*3);
    store[0]='s';
    store[1]='0'+*num;
    //calculates the storage registar 
    char val1;
    int rnum;
    //other information on where the data is stored 
    if(storloc==NULL){
        val1='s';
        //more storage location calculations 
        for (int g = 0; g < vec->count; g++)
        {
            if (l[i-6][0] == vec->vectorl[g].name[0])
            {
                rnum=vec->vectorl[g].register0;
            }
        }
        //calculates the regisar used 
        
    }
    else{
        val1='t';
        rnum=*num2-1;
    }
    //more storage calculation 
    switch (l[i-5][0])
    {
    case '+':
        add(i-5,store,l,val1,rnum,vec,num2);
        break;
    case '-':
        sub(i-5,store,l,val1,rnum,vec,num2);
        break;
    case '*':
        mult(i-5,store,l,val1,rnum,vec,num2);
        break;
    case '/':
        div1(i-5,store,l,val1,rnum,num2,vec,lnum);
        break;
    case '%':
        mod(i-5,store,l,val1,rnum,vec,num2);
        break;
    }
    //switch case for opperations 
    push_back(vec, name, *num);
    *num+=1;
    if(*num2>9){
        *num2=0;
    }
    //stores and resets the t registar count 
}

int main(int argc, char *argv[])
{
    int num = 0;
    int lnum=0;
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    ssize_t line_size;
    FILE *fp = fopen(argv[1], "r");
    if(fp==NULL){
        printf("File could not be opened\n");
        exit(1);
    }
    //opens a file if it exsists 
    vector v;
    construct_vector(&v);
    //initializes the vector 
    char sep[] = {'+','-','/','%','*'};
    //the list of delimiters 
    line_size = getline(&line_buf, &line_buf_size, fp);
    //the line size calls get line 
    int num2=0;
    while(line_size>=0)
    { 
    printf("# %s", line_buf);
    //prints the line 
    char * name=line_buf;
    //gets the name
    remove_spaces(line_buf);
    //removes spaces 
    size_t size = strlen(line_buf);
    //calculates the size
    line_buf[size - 2] = '\0';
    line_buf[size - 1] = '\0';
    //sets the last two charachter to null terminating 
    line_buf+=2;
    //incriments the pointer by two 
    char* cpy=malloc(sizeof(char)*260);
    for(int i=0; i<size;i++){
        cpy[i]=line_buf[i];
    }
    //makes a copy
    char **list = malloc(sizeof(char *) * 260);
    //creats a list of strings where the equation
    list[0]=malloc(sizeof(char)*2);
    list[0][0]='&';
    char *token;
    token = strtok(line_buf, sep);
    int i = 0;
    int g=1;
    list[g] = token;
    i++;
    //starts the list with the first value 
    while (token != NULL)
    {
        g+=3;
        token = strtok(NULL, sep);
        list[g] = token;
        i+=1;
        //uses strtok to split the string 
    }
    size_t size3=strlen(cpy);
    if(cpy[0]=='-'){
        list[0][0]='-';
    }
    int index=2;
    for(int z=0;z<size3;z++){
        for(int q=0; q<5; q++){
            if(cpy[z]==sep[q]){
                list[index]=&cpy[z];
                if(cpy[z+1]=='-'){
                    list[index+1]=malloc(sizeof(char)*2);
                    list[index+1][0]='-';
                }
                else{
                    list[index+1]=malloc(sizeof(char)*2);
                    list[index+1][0]='&';
                }
                cpy[z+1]='\0';
                index+=3;
                z++;
            }
        }

    }
    //adds the operatrors back in 
    char * store=NULL;
    if(i<3){
        simple_one_liner(list,&v,&num,name);
    }
    if(i>=4){
        store=first_instruction(list,&v,&num,&num2,name,&lnum);
    }
    mid_instruction(list,&v,&num,&num2,name,store,g,&lnum);
    if(i>=3){
        last(list,&v,&num,name,g,store,&num2,&lnum);
    }
    //calls all the functions to parse the data  
    line_size = getline(&line_buf, &line_buf_size, fp);
    }
}

