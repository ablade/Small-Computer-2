/*
 * File:   globals.h
 * Team: Alvin & Bob
 * Author: Alvin Baldemeca, Bob Landowski
 * Created on January 20, 2013, 8:11 PM
 */

/**
 * This file contains constants symbolic constant, type defintions, and
 * enumeration used for the SC2 project
 *
 * @author Alvin Baldemeca, Bob Landowski
 * @version Winter 2013
 */
#ifndef GLOBALS_H
#define	GLOBALS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MEMORY_SIZE 0xFFFF
/**
 The value is 0x0001 used to get the last bit in the 16 bit word.
 */
#define LSB_MASK 0x0001

/**
 * The value is 0x1000 this is used to set the overflow flag
 */
#define OVERFLOW_FLAG 0X1000
    
/**
 * The value is 0x2000 this is used to set the carry out flag
 */
#define CARRYOUT_FLAG 0x2000
    
/**
 * The value is 0x4000 this is used to set the zero flag
 */
#define ZERO_FLAG 0x4000
    
/**
 * The value is 0x8000 this is used to set the negative flag
 */
#define NEG_FLAG 0X8000
    
/**
 * The value is 0x0FFF this is used to reset the flags
 */
#define FLAGS_RESET 0x0FFF
    
/**
 * The value is 16, which describes the size for the register file
 */
#define SIZE_OF_REG_FILE 16

/**
 * The value is 16, which describes the bit length of the register
 */
#define BIT_LENGTH_REG 16

/**
 * The value is 0XFF00 which is used to clear the low ordered bits when ORed
 */
#define CLEAR_LOB 0XFF00
    
/**
 * The value is 0X00FF which is used to clear the high ordered bits when ORed
 */
#define CLEAR_HOB 0X00FF
    
/**
 * The value is 0xFFFF 
 */
#define SIGN_EXTEND_NEG 0XFFFF
    
/**
 * The value is 8 which represents the size of a byte
 */
#define BYTE_SHIFT 8
/**
 * The value is 0x0000.  This is used in the cpu.c cpuDecodeExec() to determine
 * if the instruction is a load/store instruction. 
 */    
#define LD_ST_STATE 0X0000
    
/**
 * The value is 0x4000.  This is used in the cpu.c cpuDecodeExec() to determine
 * if the instruction is an ALU instruction. 
 */  
#define ALU_STATE 0X4000
    
/**
 * The value is 0x8000.  This is used in the cpu.c cpuDecodeExec() to determine
 * if the instruction is a branch/jump instruction. 
 */  
#define BR_JSR_STATE 0X8000
    
/**
 * The value is 0xC000.  This is used in the cpu.c cpuDecodeExec() to determine
 * if the instruction is a IO/MISC instruction. 
 */  
#define IO_MISC_STATE 0XC000

/**
 * The value is 0x007F which is used to get the immediate 7 bits.
 */
#define IMM_7_MASK 0x007F

/**
 * The value is 0x0007 which is used to get the last 3 bit or the modifier bits.
 */
#define MODIFIER_MASK 0X0007
    
/**
 * The value is 0x000F which is used to get the last 4 bits or immediate 4 bits
 */
#define IMM_4_MASK 0X000F
    
/**
 * The value is 0x0003 which is used to get the last 2 bits or immediate 2 bits
 */
#define IMM_2_MASK 0x0003  

/**
 * The value is 0x07FF which is used to get the last 11 bits or immediate 11 bits
 */    
#define IMM_11_MASK 0x07FF
    
#define IVT 0x2100    
#define V0 0x2100
#define V1 0x2102
#define V2 0x2104
#define V3 0x2106
#define V4 0x2108
#define V5 0x2110
#define V6 0x2112
#define V7 0x2114
#define VID 0x30
#define KBD 0X38 
#define TRUE 1
#define FALSE 0

typedef unsigned char byte;
enum modifiers{ ADD = 0, SUB, MUL, DIV, AND, OR, XOR, NOT, SHL, SHR};


#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALS_H */

