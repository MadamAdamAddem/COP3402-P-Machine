/*
Assignment:
vm.c - Implement a P-machine virtual machine

Authors: <Ernesto Lugo, Anthony Casseus>

Language: C(only)

To Compile:
  gcc -O2 -Wall -std = c11 -o vm vm.c

To Execute (on Eustis):
  ./vm input.txt

where:
  input.txt is the name of the file containing PM /0 instructions;
  each line has three integers (OP L M)

Notes:
  -Implements the PM /0 virtual machine described in the homework
  instructions.
  -No dynamic memory allocation or pointer arithmetic.
  -Does not implement any VM instruction using a separate function.
  -Runs on Eustis.

Class: COP 3402 - Systems Software - Fall 2025

Instructor: Dr. Jie Lin

Due Date: Friday, September 12th, 2025
*/



#include <stdio.h>





/*----- ENUMERATIONS -----*/
enum IR_SEGMENTS
{
  OP,
  L,
  M
};

enum INSTRUCTIONS
{
  LIT = 1,
  OPR,
  LOD,
  STO,
  CAL,
  INC,
  JMP,
  JPC,
  SYS = 9
};

enum OPERATIONS
{
  RTN = 0,
  ADD,
  SUB,
  MUL,
  DIV,
  EQL,
  NEQ,
  LSS,
  LEQ,
  GTR,
  GEQ = 10
};

enum SYSCALLS
{
  PRINT = 1,
  READ,
  HALT = 3
};


enum CYCLES
{
  FETCH,
  EXECUTE
};
/*------------------------*/





int PAS[500];

//registers
int PC = 499;
int SP;
int BP;
int IR[3];





/* Find base L levels down from the current activation record */
int base (int numlevels) 
{
  int arb = BP ; // activation record base

  while (numlevels>0) 
  {
    arb = PAS[arb]; // follow static link
    numlevels--;
  }

  return arb;
}

void print(char* str){printf("%s\n", str);}


int main(int argc, char* argv[])
{
  /*----- Opening and Verifying File -----*/
  if(argc != 2)
  {
    print("Expected 1 argument");
    return 1;
  }

  FILE* fp = fopen(argv[1], "r");
  if(fp == NULL)
  {
    print("File unable to be opened");
    return 1;
  }
  /*----- Opening and Verifying File -----*/

  
  /*----- Loading Text Segment -----*/
  while(fscanf(fp, " %d %d %d", &PAS[PC], &PAS[PC-1], &PAS[PC-2]) != 3)
    PC -= 3;

  BP = PC;
  SP = BP + 1;
  

  PC = 499;

  fclose(fp);
  /*----- Loading Text Segment -----*/


  /*----- Main Loop -----*/
  while(1)
  {
    /* Fetch */
    IR[OP] = PAS[PC];
    IR[L] = PAS[--PC];
    IR[M] = PAS[--PC];
    --PC;
    
    switch(IR[OP])
    {
      case LIT:
        PAS[--SP] = IR[M];
        break;

      case LOD:
        PAS[--SP] = PAS[base(IR[L]) - IR[M]];
        break;

      case STO:
        PAS[base(IR[L]) - IR[M]] = PAS[SP++];
        break;

      case CAL:
        PAS[SP-1] = base(IR[L]);
        PAS[SP-2] = BP;
        PAS[SP-3] = PC;
        BP = SP - 1;
        PC = IR[M];
        break;

      case INC:
        SP -= IR[M];
        break;

      case JMP:
        PC = IR[M];
        break;

      case JPC:
        if(PAS[SP] == 0) PC = IR[M];
        SP += 1;
        break;

      case SYS:
      
        if(IR[M] == PRINT)
          printf("Output result is: %d\n", PAS[SP++]);
        else if(IR[M] == READ)
        {
          printf("Please Enter an Integer: ");
          int input;
          scanf("%d", &input);
          PAS[--SP] = input;
        }
        else
          return 0;
      

      case OPR:
        switch(IR[M])
        {
          case RTN:
            SP = BP + 1;
            BP = PAS[SP-2];
            PC = PAS[SP-3];
            break;

          case ADD:
            PAS[SP+1] = PAS[SP+1] + PAS[SP];
            ++SP;
            break;

          case SUB:
            PAS[SP+1] = PAS[SP+1] - PAS[SP];
            ++SP;
            break;

          case MUL:
            PAS[SP+1] = PAS[SP+1] * PAS[SP];
            ++SP;
            break;

          case DIV:
            PAS[SP+1] = PAS[SP+1] / PAS[SP];
            ++SP;
            break;

          case EQL:
            PAS[SP+1] = PAS[SP+1] - PAS[SP];
            ++SP;
            break;

          case NEQ:
            PAS[SP+1] = PAS[SP+1] != PAS[SP];
            ++SP;
            break;

          case LSS:
            PAS[SP+1] = PAS[SP+1] < PAS[SP];
            ++SP;
            break;

          case LEQ:
            PAS[SP+1] = PAS[SP+1] <= PAS[SP];
            ++SP;
            break;

          case GTR:
            PAS[SP+1] = PAS[SP+1] > PAS[SP];
            ++SP;
            break;

          case GEQ:
            PAS[SP+1] = PAS[SP+1] >= PAS[SP];
            ++SP;
            break;

          default:
            break;
        }
              
      default:
        break;
    }


  }
  /*----- Main Loop -----*/







  return 0;
}