#include <stdio.h>
#include <stdlib.h>
int main ( )
{
/* I n s e r t your e i g h t b y t e s o f ASCII f o r your s e c r e t message */
/* The 0x p r e f i x bel ow i n d i c a t e s a hex a dec im al number */
long int z =0x6E616874616E6F4A  ;
char* c ;
int i ;
printf("%ld\n",sizeof z);
for ( c = ( char*)&z , i = 0 ; i < sizeof z ; c++, i++ ) {
printf("%c"  , * c ) ;
}
printf( "\n" ) ;
return EXIT_SUCCESS;
}
//You use commas to allow you to perform multiple assignments in that same part of the for loop. 

//The output does not change from either of the changes.

//The sizeof tells us the number of bytes in the long int which tells us how many times to print. 

//little endian
