/* 
Lab 5: From C to shining C
CSCI-2500 Fall 2021
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>


// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

/******************************************************************************/
/* Function Calls */
/******************************************************************************/

BIT not_gate(BIT A)
{
  if (A)
    return FALSE;
  else
    return TRUE;
}

BIT or_gate(BIT A, BIT B)
{
  // TODO: implement logical OR
  if(A==UNDEF && B==UNDEF)
    return UNDEF;
  if(A || B)
    return TRUE;
  return FALSE;
}

BIT and_gate(BIT A, BIT B)
{
  if(A==UNDEF || B==UNDEF)
    return UNDEF;
  if(A && B)
    return TRUE;
  return FALSE;
  // TODO: implement logical AND
}

BIT xor_gate(BIT A, BIT B)
{
  return and_gate(or_gate(A,B),or_gate(not_gate(A),not_gate(B)));
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3)
{
  *O0=and_gate(not_gate(I0),not_gate(I1));
  *O1=and_gate(not_gate(I0),I1);
  *O2=and_gate(not_gate(I1),I0);
  *O3=and_gate(I0,I1);
}

BIT multiplexor2(BIT S, BIT I0, BIT I1)
{
  // TODO: implement a 2-input multiplexor
  
  return or_gate(and_gate(I0,not_gate(S)),and_gate(I1,S));
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3)
{
  // TODO: implement a 4-input multiplexor
  
  return multiplexor2(S0,multiplexor2(S1,I0,I1),multiplexor2(S1,I2,I3));
}

void adder(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum)
{
  *Sum=xor_gate(xor_gate(A,B),CarryIn);
  *CarryOut=or_gate(and_gate(A,B),and_gate(or_gate(A,B),CarryIn));
  
}

void ALU(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Op0, BIT Op1, 
    BIT* Result, BIT* CarryOut)

{
  B=multiplexor2(Binvert,B,not_gate(B));
  BIT sum=TRUE;
  adder(A,B,CarryIn,CarryOut,&sum);
  *Result=multiplexor4(Op0,Op1,and_gate(A,B),or_gate(A,B),sum,FALSE);
  *CarryOut=multiplexor4(Op0,Op1,FALSE,FALSE,*CarryOut,FALSE);
  if(Op0 && Op1)
    *Result=UNDEF;
  
}


/* main() provides some unit testing */
int main()
{
  /* Unit test for NOT gate:
   *      _
   *  A | A
   * ---|---
   *  0 | 1
   *  1 | 0
   */
  printf("===== Unit test for not_gate =====\n");
  printf("not_gate( A ) | RESULT\n");
  printf("--------------|-------\n");  
  for (BIT A = 0; A < 2; ++A)
    printf("not_gate( %d ) | %d\n", A, not_gate(A));
  

  // Unit test for OR gate:
  printf("\n===== Unit test for or_gate =====\n");
  printf("or_gate( A, B ) | RESULT\n");
  printf("----------------|-------\n");
  for (BIT A = 0; A < 2; ++A)
    for (BIT B = 0; B < 2; ++B)
      printf("or_gate( %d, %d ) | %d\n", A, B, or_gate( A, B ));

  
  // Unit test for AND gate:
  printf( "\n===== Unit test for and_gate =====\n" );
  printf( "and_gate( A, B ) | RESULT\n" );
  printf( "-----------------|-------\n" );
  for (BIT A = 0; A < 2; ++A)
    for (BIT B = 0; B < 2; ++B)
      printf("and_gate( %d, %d ) | %d\n", A, B, and_gate(A, B));


  // Unit test for XOR gate:
  printf( "\n===== Unit test for xor_gate =====\n" );
  printf( "xor_gate( A, B ) | RESULT\n" );
  printf( "-----------------|-------\n" );
  for (BIT A = 0; A < 2; ++A)
    for (BIT B = 0; B < 2; ++B)
      printf("xor_gate( %d, %d ) | %d\n", A, B, xor_gate(A, B));


  // Unit test for 2-input decoder:
  printf( "\n===== Unit test for 2-input decoder =====\n" );
  printf( "decoder( I0, I1 ) | ( O0, O1, O2, O3 )\n" );
  printf( "------------------|-------------------\n" );
  BIT O0, O1, O2, O3;
  O0 = O1 = O2 = O3 = UNDEF;
  for (BIT I0 = 0; I0 < 2; ++I0)
    for (BIT I1 = 0; I1 < 2; ++I1) {
      decoder2(I0, I1, &O0, &O1, &O2, &O3);
      printf("decoder( %2d, %2d ) | ( %2d, %2d, %2d, %2d )\n",
          I0, I1, O0, O1, O2, O3);
    }  
  
  
  // Unit test for 2-input multiplexer
  printf( "\n===== Unit test for 2-input multiplexor =====\n" );
  printf( "multiplexor( S ) | ( I0, I1 ) | RESULT\n" );
  printf( "-----------------|------------|-------\n" );
  for (BIT S = 0; S < 2; ++S)
    for (BIT I0 = 0; I0 < 2; ++I0)
      for (BIT I1 = 0; I1 < 2; ++I1)
        printf("multiplexer( %d ) | ( %2d, %2d ) | %2d\n",
            S, I0, I1, multiplexor2(S, I0, I1));
     
                  
  // Unit test for 4-input multiplexer
  printf("\n===== Unit test for 4-input multiplexor =====\n" );
  printf("multiplexor( S0, S1 ) | ( I0, I1, I2, I3 ) | RESULT\n");
  printf("----------------------|--------------------|-------\n");
  for (BIT S0 = 0; S0 < 2; ++S0)
    for (BIT S1 = 0; S1 < 2; ++S1)
      for (BIT I0 = 0; I0 < 2; ++I0)
        for (BIT I1 = 0; I1 < 2; ++I1)
          for (BIT I2 = 0; I2 < 2; ++I2)
            for (BIT I3 = 0; I3 < 2; ++I3)
              printf( "multiplexer( %2d, %2d ) | ( %2d, %2d, %2d, %2d ) | %2d\n",
                  S0, S1, I0, I1, I2, I3, multiplexor4(S0, S1, I0, I1, I2, I3));


  // Unit test for 1-bit adder:
  printf( "\n===== Unit test for 1-bit adder =====\n" );
  printf( "adder( A, B, CI ) | ( CO, Sum )\n" );
  printf( "------------------|------------\n" );
  BIT CarryOut, Sum;
  CarryOut = Sum = UNDEF;
  for (BIT A = 0; A < 2; ++A)
    for (BIT B = 0; B < 2; ++B)
      for (BIT CarryIn = 0; CarryIn < 2; ++CarryIn) {
        adder(A, B, CarryIn, &CarryOut, &Sum);
        printf("adder( %d, %d, %2d ) | ( %2d, %3d )\n",
            A, B, CarryIn, CarryOut, Sum);
      }  
    
    
  // Unit test for 1-bit ALU
  printf("\n===== Unit test for 1-Bit ALU =====\n");
  printf("ALU( A, B ) | ( Bi, CI, Op0, Op1 ) | ( Re, CO )\n" );
  printf("------------|----------------------|-----------\n" );
  BIT Result = UNDEF;
  for (BIT A = 0; A < 2; ++A)
    for (BIT B = 0; B < 2; ++B)
      for (BIT Binvert = 0; Binvert < 2; ++Binvert)
        for (BIT CarryIn = 0; CarryIn < 2; ++CarryIn)
          for (BIT Op0 = 0; Op0 < 2; ++Op0)
            for (BIT Op1 = 0; Op1 < 2; ++Op1) {
              ALU(A, B, Binvert, CarryIn, Op0, Op1, &Result, &CarryOut);
              printf( "ALU( %d, %d ) | ( %2d, %2d, %3d, %3d ) | ( %2d, %2d )\n",
                  A, B, Binvert, CarryIn, Op0, Op1, Result, CarryOut);
            }

  return 0;
}

