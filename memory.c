/*
 * File:   memory.c
 * Team: Alvin & Bob
 * Author: Alvin Baldemeca, Bob Landowski
 *
 * Created on January 18, 2013, 7:01 PM
 */

 /**
 * This file represent a 8-bit wide 256 deep big indean memory object.
 * can perform.  It can be used for other object oriented projects that
 * requires an abstract data type of this kind of memory.
 *
 * @author Alvin Baldemeca, Bob Landowski
 * @version Winter 2013
 */
#include "globals.h"
#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


/**
 *  Constructs a Memory structure (Abstract Data Type)
 *  @return  a MemoryPtr to that structure.
 */
MemoryPtr MemoryConstructor(void)
{
	MemoryPtr the_memory = (MemoryPtr) malloc(sizeof(MemoryStr));
	return the_memory;
}


/**
 * Sets the byte at the given memory address
 * @param the_memory - the pointer to the memory we want to set
 * @param address - the address of the memory we want to set
 * @param the_data - the bit stream we want to set the memory to
 */
void set_Mem_Byte(MemoryPtr const the_memory, unsigned short const
				address, byte const the_data)
{
	the_memory->mem[address] = the_data;
}

/**
 * Gets the byte at the memory address provided
 * @param the_memory - the memory to get the byte from
 * @param address - the address of the memory we want to get the
 * byte from
 * @return - the byte contained in the memory we selected
 */
byte get_Mem_Byte(MemoryPtr the_memory, unsigned short address)
{
	return the_memory->mem[address];
}

/**
 * Sets the word at the given memory address using big indean.  The
 * address given is set to the lower 8 bits the next higher address
 * is then set to be the 8 upper bits, 16 bits are set in the register
 * when these bits are concatenated.
 * @param the_memory - the pointer to the memory we want to set
 * @param address - the address of the memory we want to set
 * @param the_data - the bit stream we want to set the memory to
 */
void set_Mem_Word(MemoryPtr the_memory, unsigned short address,
				unsigned short the_data)
{
	the_memory->mem[address + 1] = the_data;
	the_data = the_data >> BYTE_SHIFT;
	the_memory->mem[address] = the_data;
}

/**
 * Gets the word at the given memory address using big indean.  The
 * address given gets to the lower 8 bits the next higher address
 * is retrieved to be the 8 upper bits, 16 bits are then returned
 * when these bits are concatenated.
 * @param the_memory - the pointer to the memory we want to get the
 * bit stream from
 * @param address - the address of the memory we want to get
 * @return the 16 bit data from the memory address provided.
 */
int get_Mem_Word(MemoryPtr the_memory, unsigned short address)
{
	int temp = the_memory->mem[address];
	temp = temp << BYTE_SHIFT;
	temp = temp | the_memory->mem[address + 1];
	return temp;

}

