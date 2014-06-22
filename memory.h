/*
 * File:   memory.h
 * Team: Alvin & Bob
 * Author: Alvin Baldemeca, Bob Landowski
 * Created on January 20, 2013, 8:05 PM
 */

/**
 * This file contains the structure defenition for the MemoryStr.  It
 * also contains the function prototypes for the memory.c
 *
 * @author Alvin Baldemeca, Bob Landowski
 * @version Winter 2013
 */

#include "globals.h"
#ifndef MEMORY_H
#define	MEMORY_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct
    {
        unsigned char mem[MEMORY_SIZE];
    }MemoryStr;

    typedef MemoryStr* MemoryPtr;

    MemoryPtr MemoryConstructor(void);
    void set_Mem_Byte(MemoryPtr const the_memory, unsigned short const address,
            byte const the_data);
    byte get_Mem_Byte(MemoryPtr const the_memory, unsigned short const address);
    void set_Mem_Word(MemoryPtr const the_memory, unsigned short const address, 
            unsigned short the_data);
    int get_Mem_Word(MemoryPtr const the_memory, unsigned short const address);





#ifdef	__cplusplus
}
#endif

#endif	/* MEMORY_H */

