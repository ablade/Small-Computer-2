/*
 * File:   alu.c
 * Team: Alvin & Bob
 * Author: Alvin Baldemeca, Bob Landowski
 * Created on January 18, 2013, 7:01 PM
 */

 /**
 * This file simulates the operation that the alu for the SC-2 computer
 * can perform.  It can be used for other object oriented projects that
 * requires an abstract data type of this alu.
 *
 * @author Alvin Baldemeca, Bob Landowski
 * @version Winter 2013
 */
#include <stdio.h>
#include <stdlib.h>
#include "alu.h"
#include "globals.h"
#include "register.h"
#include "registerFile.h"
#include "cpu.h"

/**
*  Constructs an Alu structure (Abstract Data Type) and
*  @return  an AluPtr to that structure.
*/
AluPtr aluConstructor(RegisterFilePtr theRegFile, IntRegPtr theIntReg)
{
    AluPtr my_alu = (AluPtr) malloc(sizeof(AluStr));
    my_alu->reg0 = theRegFile.R[0];
    my_alu->reg8 = theRegFile.R[8];
    my_alu->SW = theIntReg->sw;
    return my_alu;
}

/**
 *  Performs alu functions given the alu and the action to perform.
 *  @param the_alu - the pointer to the alu that will perfrom the
 *  calculation
 *
 *  @param the_modifier - the enum modifiers to consider see global.h
 */
void aluOpcode(AluPtr const the_alu, unsigned short the_modifier)
{
	switch(the_modifier)
	{
	  case ADD : aluAdd(the_alu); break;
	  case SUB : aluSub(the_alu); break;
	  case MUL : aluMul(the_alu); break;
	  case DIV : aluDiv(the_alu); break;
	  case AND : aluBitAnd(the_alu); break;
	  case OR  : aluBitOr(the_alu); break;
	  case XOR : aluBitXor(the_alu); break;
	  case NOT : aluBitNot(the_alu); break;
	  case SHL : aluBitShl(the_alu); break;
	  case SHR : aluBitShr(the_alu); break;
	  default: break;
	}
}

/**
 * Adds the value contained in alu's A and B register and stores the
 * result of register R into the the pointer address that reg0
 * points to.
 * @param the_alu - the pointer to the alu that will perfrom the
 * calculation
 */
void aluAdd(AluPtr const the_alu)
{
	signed short temp1 = the_alu->A;
	signed short temp2 = the_alu->B;
	signed short temp3 = 0;
	signed short temp4 = 0;
	the_alu->R = temp1 + temp2;
	the_alu->reg0->reg = the_alu->R;
	int i;
	the_alu->SW->reg = the_alu->SW->reg & FLAGS_RESET;
	for(i = 0; i < SIZE_OF_REG_FILE; i++)
	{
		temp3 = (temp1 & LSB_MASK) + (temp2 & LSB_MASK) + temp3;
		temp1 = temp1>>1;
		temp2 = temp2>>1;
		if(i == SIZE_OF_REG_FILE - 2)
		{
	      temp4 = temp3>>1;
		}
		temp3 = temp3>>1;
	}
	
	if(temp3 == 1)
	{
	  the_alu->SW->reg = the_alu->SW->reg | CARRYOUT_FLAG;
	}

	if( temp3 ^ temp4)
	{
      the_alu->SW->reg = the_alu->SW->reg | OVERFLOW_FLAG; // overflow
	}
	updateSW(the_alu);
}

/**
 * Subtracts the value contained in alu's A and B register and
 * stores the result of register R into the the pointer address that
 * reg0 points to.
 * @param the_alu - the pointer to the alu that will perfrom the
 * calculation
 */
void aluSub(AluPtr const the_alu)
{
	signed short temp1 = the_alu->A;
	signed short temp2 = the_alu->B;
	signed short temp3 = temp1 - temp2;
	signed short temp4 = 0;
	the_alu->R = temp3;
	the_alu->reg0->reg = the_alu->R;
	temp2 = ~(the_alu->B) + 1;


	int i;
	the_alu->SW->reg = the_alu->SW->reg & FLAGS_RESET;
	for(i = 0; i < SIZE_OF_REG_FILE; i++)
	{
	  temp3 = (temp1 & LSB_MASK) + (temp2 & LSB_MASK) + temp3;
	  temp1 = temp1>>1;
	  temp2 = temp2>>1;

	  if(i == SIZE_OF_REG_FILE - 2)
	  {
	    temp4 = temp3>>1;
	  }
	  temp3 = temp3>>1;
	}
	
	if(temp3 == 1)
	{
	  the_alu->SW->reg = the_alu->SW->reg | CARRYOUT_FLAG;
	}

	if(temp4 ^ temp3)
    {
	  the_alu->SW->reg = the_alu->SW->reg | OVERFLOW_FLAG; // overflow
	}
	updateSW(the_alu);
}

/**
 * Multiplys the value contained in alu's A and B's 8 low order bit and
 * stores the result of register R into the the pointer address that
 * reg0 points to.
 * @param the_alu - the pointer to the alu that will perfrom the
 * calculation
 */
void aluMul(AluPtr const the_alu)
{
	signed char OpA = the_alu->A;
	signed char OpB = the_alu->B;
	the_alu->R = (signed short) OpA * OpB;  
	the_alu->reg0->reg = the_alu->R; //(signed short) OpA * OpB;
	the_alu->SW->reg = the_alu->SW->reg & FLAGS_RESET;
	updateSW(the_alu);
}

/**
 * Divides the value contained in alu's A and B registers and
 * stores the result of register R into the the pointer address that
 * reg0 points to and the remainder where reg8 points  to.
 * @param the_alu - the pointer to the alu that will perfrom the
 * calculation
 */
void aluDiv(AluPtr const the_alu )
{
	signed short temp1 = the_alu->A;
	signed short temp2 = the_alu->B;
	signed short temp3 = temp1 / temp2;
	the_alu->R = temp3;
	the_alu->reg0->reg = temp3;
	the_alu->reg8->reg = (the_alu->A) % (the_alu->B);
	the_alu->SW->reg = the_alu->SW->reg & FLAGS_RESET;
	updateSW(the_alu);
}

/**
 * Performs a "Bit AND" between the value contained in alu's A and B
 * registers and stores the result of register R into the the
 * pointer address that reg0 points to.
 * @param the_alu - the pointer to the alu that will perfrom the
 * calculation
 */
void aluBitAnd(AluPtr const the_alu)
{
	the_alu->R = the_alu->A & the_alu->B;
	the_alu->reg0->reg = the_alu->R;
	the_alu->SW->reg = the_alu->SW->reg & FLAGS_RESET;
	updateSW(the_alu);
}

/**
 * Performs a "Bit OR" between the value contained in alu's A and B
 * registers and stores the result of register R into the the
 * pointer address that reg0 points to.
 * @param the_alu - the pointer to the alu that will perfrom the
 * calculation
 */
void aluBitOr(AluPtr const the_alu)
{
	the_alu->R = the_alu->A | the_alu->B;
	the_alu->reg0->reg = the_alu->R;
	the_alu->SW->reg = the_alu->SW->reg & FLAGS_RESET;
	updateSW(the_alu);
}

/**
 * Performs a "Bit XOR" between the value contained in alu's A and B
 * registers and stores the result of register R into the the
 * pointer address that reg0 points to.
 * @param the_alu - the pointer to the alu that will perfrom the
 * calculation
 */
void aluBitXor(AluPtr const the_alu)
{
	the_alu->R = the_alu->A ^ the_alu->B;
	the_alu->reg0->reg = the_alu->R;
	the_alu->SW->reg = the_alu->SW->reg & FLAGS_RESET;
	updateSW(the_alu);
}

/**
 * Performs a "Bit NOT" in the value contained in alu's A register
 * and stores the result of register R into the the pointer address
 * that reg0 points to.
 * @param the_alu - the pointer to the alu that will perfrom the
 * calculation
 */
void aluBitNot(AluPtr const the_alu)
{
	the_alu->R = ~(the_alu->A);
	the_alu->reg0->reg = the_alu->R;
	the_alu->SW->reg = the_alu->SW->reg & FLAGS_RESET;
	updateSW(the_alu);
}

/**
 * Performs a bit shift left in the value contained in alu's A
 * register and stores the result of register R into the the pointer
 * address that reg0 points to.
 * @param the_alu - the pointer to the alu that will perfrom the
 * calculation
 */
void aluBitShl(AluPtr const the_alu)
{
	the_alu->R = (the_alu->A) << 1;
	the_alu->reg0->reg = the_alu->R;
	the_alu->SW->reg = the_alu->SW->reg & FLAGS_RESET;
	updateSW(the_alu);
}

/**
 * Performs a bit shift right in the value contained in alu's A
 * register and stores the result of register R into the the pointer
 * address that reg0 points to.
 * @param the_alu - the pointer to the alu that will perfrom the
 * calculation
 */
void aluBitShr(AluPtr const the_alu)
{
	the_alu->R = (the_alu->A)>> 1;
	the_alu->reg0->reg = the_alu->R;
	the_alu->SW->reg = the_alu->SW->reg & FLAGS_RESET;
	updateSW(the_alu);
}

/**
 * Updates the register that the alu's SW pointer points to whenever
 * the alu perfroms a task.
 * @param the_alu - the pointer to the alu that will perfrom the
 * calculation
 */
void updateSW(AluPtr const the_alu)
{
	int temp3 = the_alu->R;
	if(temp3 == 0)
	{
	  the_alu->SW->reg = (the_alu->SW->reg | ZERO_FLAG); //Zero
	}

	temp3 = temp3>> (BIT_LENGTH_REG - 1);
	if(temp3 == 1)
	{
	  the_alu->SW->reg = the_alu->SW->reg | NEG_FLAG; //negative
	}
}
