/* 
 * File:   cpu.h
 * Author: alvin
 *
 * Created on February 5, 2013, 1:16 PM
 */


#include "alu.h"
#include "globals.h"
#include "memory.h"
#include "register.h"
#include "registerFile.h"
#ifndef CPU_H
#define	CPU_H
#ifdef	__cplusplus
extern "C" {
#endif

    /*
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
    */
    typedef struct
    {
        AluPtr cpu_alu;
        MemoryPtr cpu_memory;
        RegisterFilePtr cpu_register;
        IntRegPtr cpu_int_reg;
        int RB;        
        
    }CpuStr;
    
    typedef CpuStr* CpuPtr;
    
    CpuPtr cpuConstructor(MemoryPtr the_memory);
    IntRegPtr intRegConstructor();
    void cpuFetch(CpuPtr theCpu);
    void cpuDecodeExec(CpuPtr theCpu);
    void ld_st(const unsigned short theType, CpuPtr theCpu);
    void alu(const unsigned short theType, CpuPtr theCpu);
    void br_jsr(const unsigned short theType, CpuPtr theCpu);
    void io_misc(const unsigned short theType, CpuPtr theCpu);



#ifdef	__cplusplus
}
#endif

#endif	/* CPU_H */

