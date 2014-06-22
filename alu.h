/*
 * File:   alu.h
 * Team: Alvin & Bob
 * Author: Alvin Baldemeca, Bob Landowski
 *
 * Created on January 18, 2013, 7:14 PM
 */


 /**
 * This file contains the structure defenition for the AluStr.  It also
 * contains the function prototypes for the alu.c
 *
 * @author Alvin Baldemeca, Bob Landowski
 * @version Winter 2013
 */
#include "register.h"
#include "registerFile.h"
#ifndef ALU_H
#define	ALU_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct{
        unsigned short A;
        unsigned short B;
        unsigned short R;
        RegisterPtr reg0;
        RegisterPtr reg8;
        RegisterPtr SW;
    } AluStr;

    
    typedef struct
    {
        RegisterPtr pc;
        RegisterPtr sw;
        RegisterPtr ir;
        RegisterPtr mdr;
        RegisterPtr mar;
        RegisterPtr oxb;
        RegisterPtr txb;
        
    }IntRegStr;
    
    typedef IntRegStr* IntRegPtr;
    
    typedef AluStr* AluPtr;
    AluPtr aluConstructor(RegisterFilePtr theRegFile, IntRegPtr theIntReg);
    void aluOpcode(AluPtr const the_alu, unsigned short the_modifier);
    void aluAdd(AluPtr const the_alu);
    void aluSub(AluPtr const the_alu);
    void aluMul(AluPtr const the_alu);
    void aluDiv(AluPtr const the_alu);
    void aluBitAnd(AluPtr const the_alu);
    void aluBitOr(AluPtr const the_alu);
    void aluBitXor(AluPtr const the_alu);
    void aluBitNot(AluPtr const the_alu);
    void aluBitShl(AluPtr const the_alu);
    void aluBitShr(AluPtr const the_alu);
    void updateSW(AluPtr const the_alu);

#ifdef	__cplusplus
}
#endif

#endif	/* ALU_H */

