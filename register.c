/*
 * File:   register.C
 * Team: Alvin & Bob
 * Author: Alvin Baldemeca, Bob Landowski
 *
 * Created on January 20, 2013, 6:03 PM
 *
 */

/**
 *
 * This file represent a register that can set low order bytes, high
 * order bytes or words using big indean notation.  It can only read or
 * get words which are 16 bits. It can be used for other object oriented
 * projects that requires an abstract data of a register with these
 * functions.
 * @author Alvin Baldemeca, Bob Landowski
 * @version Winter 2013
 */

#include <stdlib.h>
#include <stdio.h>
#include "register.h"
#include "registerFile.h"
#include "globals.h"

/**
  *  Constructs a Register structure (Abstract Data Type)
  *  @return  a RegisterPtr to that structure.
  */
RegisterPtr RegisterConstructor(void)
{
    RegisterPtr sc_register = (RegisterPtr)malloc(sizeof(RegisterStr));
    return sc_register;
}

/**
 * Sets the low order byte (lower 8 bits) in the register
 * @param sc_register - the register to set the byte to
 * @param the_byte - the data to set in the register
 */
void set_Reg_LOB(RegisterPtr const sc_register, byte const the_byte)
{
    sc_register->reg = sc_register->reg & CLEAR_LOB;
    sc_register->reg = sc_register->reg | the_byte;
}

/**
 * Sets the high order byte (upper 8 bits) in the register
 * @param sc_register - the register to set the byte to
 * @param the_byte - the data to set in the register
 */
void set_Reg_HOB(RegisterPtr const sc_register, byte const the_byte)
{
    unsigned short temp = 0;
    temp = temp | the_byte;
    temp = temp << 8;
    sc_register->reg = sc_register->reg & CLEAR_HOB;
    sc_register->reg = sc_register->reg | temp;
}

/**
 * Sets all 16 bits in the register with the data provided
 * @param sc_register - the register to set the word to
 * @param the_data - the 16 bit word to set in the register
 */
void set_Reg(RegisterPtr sc_register, unsigned short the_data )
{
            sc_register->reg = the_data;
}

/**
 * Reads or gets the 16 bit data from the register
 * @param sc_register - the register to read the data from
 * @return the unsinged short (16 bit) data contained in the register
 */
unsigned short read_Reg(RegisterPtr const sc_register)
{
    return sc_register->reg;
}

RegisterFilePtr regFileConstructor()
{
    RegisterFilePtr myRegisterFile;
    int i;
    //Creates all 16 Register
    for(i = 0; i < SIZE_OF_REG_FILE; i++)
    {
      myRegisterFile.R[i] = RegisterConstructor(); 
    }
    
    return myRegisterFile;
}

