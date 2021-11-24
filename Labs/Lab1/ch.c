#include<stdio.h>
#include<string.h>
#include<math.h>

void t1(int n){
    for(int i=n;i>0;i--){
        for(int z=(i*2)-1;z>0;z--){
            printf("*");
        }
        printf("\n");
    }
}
void t2(int n){
    char string[(n*2)-1];
    memset(string,'*',(n*2)-1);
    for(int i=n;i>0;i--){
        printf("%.*s\n",((i*2)-1),string);


    }

}
void hyp(double n){
    double tmp= (n*2)-1;
    double base= pow(tmp,2);
    double height=pow(n,2);
    double hypotnuse= sqrt(base+height); 
    printf("Length of hypotenuse: %0.2f\n",hypotnuse);


}
int fib_calc(int n){
    if(n<2){
        return n;

    }
    return fib_calc(n-1)+fib_calc(n-2);
}
int main(){
    int n;
    printf("What is n? ");
    scanf("%d",&n);
    t1(n);
    printf("\n");
    t2(n);
    hyp(n);
    printf("What is n? ");
    scanf("%d",&n);
    printf("Fibonacci: %d\n",fib_calc(n));
    return 0;

}