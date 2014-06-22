/*
 * File:   cpu.c
 * Team: Alvin & Bob
 * Author: Alvin Baldemeca, Bob Landowski
 *
 * Created Feb 2013 
 */

 /**
 * This file represents the SC2 CPU.  It instantiates all the parts of 
 * the CPU namely the ALU, register file, internal registers.  It also 
 * has a pointer to memory
 *
 * @author Alvin Baldemeca, Bob Landowski
 * @version Winter 2013
 */



#include "globals.h"
#include "cpu.h"
#include "alu.h"
#include "memory.h"
#include "register.h"
#include "registerFile.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ncurses.h>
extern WINDOW *my_win;


/**
 * This constructs the CPU.  It allocates memory into the heap for all 
 * the ADT in the CPU.  The memory is separate from the CPU so it is not
 * constructed for but a reference to it is required during construction.
 * @param the_memory the memory that the CPU will be using to store and
 * load programs and values to.
 * @return A pointer to the CPU that has been constructed.
 */
CpuPtr cpuConstructor(MemoryPtr the_memory)
{
    CpuPtr myCpu = (CpuPtr) malloc(sizeof(CpuStr));
    myCpu->cpu_int_reg = intRegConstructor();
    myCpu->cpu_register = regFileConstructor();
    myCpu->cpu_memory = the_memory;
    myCpu->cpu_alu = aluConstructor(myCpu->cpu_register, 
									myCpu->cpu_int_reg);
    myCpu->RB = TRUE;
    return myCpu;
}

/**
 * This is a helper function used by the cpuConstructor to create the 
 * internal register for the CPU.  The ADT are allocated for in the heap.
 * @return a pointer to the IntRegStr that's been constructed.
 */
IntRegPtr intRegConstructor()
{
    IntRegPtr myIntReg = (IntRegPtr) malloc(sizeof(IntRegStr));
    myIntReg->ir = (RegisterPtr) malloc(sizeof(RegisterStr));
    myIntReg->mar = (RegisterPtr) malloc(sizeof(RegisterStr));
    myIntReg->mdr = (RegisterPtr) malloc(sizeof(RegisterStr));
    myIntReg->oxb = (RegisterPtr) malloc(sizeof(RegisterStr));
    myIntReg->pc = (RegisterPtr) malloc(sizeof(RegisterStr));
    myIntReg->sw = (RegisterPtr) malloc(sizeof(RegisterStr));
    myIntReg->txb = (RegisterPtr) malloc(sizeof(RegisterStr));

    return myIntReg;
}

/**
 * Fetches the instructions from the memory and loads it into the 
 * instruction register (IR)
 * @param theCpu the CPU that contains the instruction register
 * @param theMemory the memory address to load into the instruction 
 * register
 */
void cpuFetch(CpuPtr theCpu)
{
    if(theCpu->RB)
    {
       theCpu->cpu_int_reg->ir->reg =  
       get_Mem_Word(theCpu->cpu_memory, theCpu->cpu_int_reg->pc->reg);
       theCpu->cpu_int_reg->pc->reg += 2;
    }
}

/**
 * This functions decodes then executes the instructions given to the 
 * CPU
 * @param theCpu the CPU that will decode and execute the instructions
 */
void cpuDecodeExec(CpuPtr theCpu)
{
    if(theCpu->RB)
    {
        unsigned short typeInst = 
				theCpu->cpu_int_reg->ir->reg & 0xC000;
        unsigned short typeOp = theCpu->cpu_int_reg->ir->reg & 0x3800;
        typeOp = (typeOp << 1) >> 12;
        

        switch(typeInst)
        {
            case LD_ST_STATE   : ld_st(typeOp, theCpu); break;
            case ALU_STATE     : alu(typeOp, theCpu); break;
            case BR_JSR_STATE  : br_jsr(typeOp, theCpu); break;
            case IO_MISC_STATE : io_misc(typeOp, theCpu);break;
            default :
            assert(TRUE == TRUE && "This instruction is not valid "
                "for this ISA"); break;
        }
    }

}

/**
 * This function is a helper function for the cpuDecodeExec() function
 * when the decoded state is a ld/st (load or store) instruction.
 * @param theType the type of load and store distinguished by bit 11 to
 * 13 of the instruction. 
 * @param theCpu the reference to the CPU that will perform the 
 * instruction
 */
void ld_st(unsigned short theType, CpuPtr theCpu)
{
    unsigned short ir = theCpu->cpu_int_reg->ir->reg;
    unsigned short modifier = ir & MODIFIER_MASK;
    unsigned short regA = (ir >> 3) & IMM_4_MASK;
    unsigned short regD = (ir >> 7) & IMM_4_MASK;
    RegisterPtr dPtr = theCpu->cpu_register.R[regD];
    RegisterPtr aPtr = theCpu->cpu_register.R[regA];
    unsigned short d_value = dPtr->reg;
    unsigned short a_value = aPtr->reg;
    unsigned short ip0_value = theCpu->cpu_register.R[0xC]->reg;
    unsigned short ip1_value = theCpu->cpu_register.R[0xD]->reg;
    unsigned short  bp_value = theCpu->cpu_register.R[0xE]->reg;
    unsigned short  sp_value = theCpu->cpu_register.R[0xF]->reg;
    short imm7 = ir & IMM_7_MASK;

    switch(theType)
    {
      case 1 : //LDI $Rd, immed7          
				 // $Rd ← SEXT(immed7) 

        set_Reg(dPtr, imm7);
      break;


      case 2 : // LDA     $Rd 
              // $Rd(hob) ← M[PC]; $Rd(lob) ← M[PC+1]
            
        set_Reg(dPtr, get_Mem_Word(theCpu->cpu_memory, 
        theCpu->cpu_int_reg->pc->reg));
        theCpu->cpu_int_reg->pc->reg += 2;
      break;



      case 3 : // LDB

        switch(modifier)
        {
          case 0 : // LDB     $Rd(lob), $Ra
                         // $Rd(lob) ← M[$Ra]  register mode

            set_Reg_LOB(dPtr, 
            get_Mem_Byte(theCpu->cpu_memory, a_value));
          break;

          case 1 : // LDB     $Rd(lob), %$Ra
                   // $Rd(lob) ← M[BP + $Ra]
                   // base-relative

             a_value = a_value + bp_value;
             set_Reg_LOB(dPtr, 
             get_Mem_Byte(theCpu->cpu_memory, a_value));
           break;

           case 2 :  // LDB     &$Rd 
                     // $Rd(lob) ← M[IP-0]; IP-0 ← IP-0 + 1
                     // index, register 0xC
         
             set_Reg_LOB(dPtr, 
             get_Mem_Byte(theCpu->cpu_memory, ip0_value));
             theCpu->cpu_register.R[0xC]->reg++;   
                    
           break;


           case 3 : //LDB     $Rd, @$Ra
                    // $Rd(lob) ← M[M[$Ra]]
                    // indirect mode
                    
             a_value = get_Mem_Word(theCpu->cpu_memory, a_value);
             set_Reg_LOB(dPtr, 
             get_Mem_Byte(theCpu->cpu_memory, a_value));
                    
           break;


           default : 
             assert(TRUE == TRUE && "This instruction is not valid "
             "for this ISA"); break;
        }
        
      break;

      case 4 : //LDW
        switch(modifier)
        {
                
          case 0 :  // LDW     $Rd, $Ra  
                    // $Rd(hob) ← M[$Ra]; $Rd(lob) ← M[$Ra + 1]
                    // register mode
            assert(a_value % 2 == 0 && "The address and/or the base "
            "pointer to load the word to must be even");
            set_Reg(dPtr, get_Mem_Word(theCpu->cpu_memory, a_value));
          break;

          case 1 :  // $Rd(hob) ← M[BP + $Ra];
                    // $Rd(lob) ← M[BP + $Ra + 1]  base-relative	
            a_value = a_value + bp_value;
                
            assert(a_value % 2 == 0 && "The address and the base "
            "pointer to load the word from must be even");
            set_Reg(dPtr, get_Mem_Word(theCpu->cpu_memory, a_value));
          break;

          case 2 : // $Rd(hob) ← M[IP-0]; IP-0 ← IP-0 + 1
                   // $Rd(lob) ← M[IP-0]; IP-0 ← IP-0 + 1  
                     
            assert(ip0_value % 2 == 0 && "The index pointer to load "
            "the word from must be even");      
            set_Reg(dPtr, get_Mem_Word(theCpu->cpu_memory, ip0_value));
            theCpu->cpu_register.R[0xC]->reg +=2;
          break;

          case 3 :  // $Rd(hob) ← M[M[$Ra]];
                    // $Rd(lob) ← M[M[$Ra + 1]]
                    // indirect mode

            a_value = get_Mem_Word(theCpu->cpu_memory, a_value);
            assert(a_value % 2 == 0 && "The address to load the word "
            "to must be even");
            set_Reg(dPtr, get_Mem_Word(theCpu->cpu_memory, a_value));
          break;

          case 4 : // $Rd ← ($Rs)
                   // register to register move
      
            set_Reg(dPtr, a_value);
          break;

          default :
            assert(TRUE == TRUE && "This instruction is not valid "
            "for this ISA"); 
          break;


        }
        break;

      case 5 :  // STB

        switch(modifier)
        {
          case 0 : // M[$Ra] ← ($Rs(lob))
                    
            set_Mem_Byte(theCpu->cpu_memory, a_value, d_value);
          break;

          case 1 :  // M[BP + $Ra] ← ($Rs(lob))

            a_value = a_value + bp_value;
            set_Mem_Byte(theCpu->cpu_memory, a_value, d_value);
          break;

          case 2 :  // M[IP-1] ← ($Rs(lob)); IP-1 ← IP-1 + 1
                    // index mode, register 0xD
                   
            set_Mem_Byte(theCpu->cpu_memory, ip1_value, d_value);
            theCpu->cpu_register.R[0xD]->reg++;
          break;

          case 3 :  // M[M[$Ra]] ← ($Rs(lob))
                    // indirect mode

            a_value = get_Mem_Word(theCpu->cpu_memory, a_value);
            set_Mem_Byte(theCpu->cpu_memory, a_value, d_value);
          break;

          default : 
            assert(TRUE == TRUE && "This instruction is not valid "
            "for this ISA");
          break;
        }
      break;

      case 6 :  //STW
                
        switch(modifier)
        {
          case 0 :// M[$Ra] ← ($Rs(hob)); M[$Ra + 1] ← ($Rs(lob))
              
            assert(a_value % 2 == 0 && "The address must be an even "
            "number to store the word");
            set_Mem_Word(theCpu->cpu_memory, a_value, d_value);        
          break;

          case 1 :  // M[BP + $Ra] ← ($Rs(hob));
                    // M[BP + $Ra + 1] ← ($Rs(lob))

            a_value = a_value + bp_value;
            assert(a_value % 2 == 0 && "The address and the base must "
            "be an even number to store the word");
            set_Mem_Word(theCpu->cpu_memory, a_value, d_value);
          break;


          case 2 :  // M[IP-1] ← ($Rs(hob)); IP-1 ← IP-1 + 1;
                    // M[IP-1] ← ($Rs(lob)); IP-1 ← IP-1 + 1
                    // index mode, register 0xD
            if( theCpu->cpu_register.R[0xD]->reg % 2 !=0)
            {
			  theCpu->cpu_register.R[0xD]->reg++;
			  ip1_value =  theCpu->cpu_register.R[0xD]->reg;
			}
            
            set_Mem_Word(theCpu->cpu_memory, ip1_value, d_value);
            theCpu->cpu_register.R[0xD]->reg+=2;
          break;


          case 3 :  // M[M[$Ra]] ← ($Rs(hob));
                    // M[M[$Ra + 1]] ← ($Rs(lob))
                    // indirect mode

            a_value = get_Mem_Word(theCpu->cpu_memory, a_value);
            assert(a_value % 2 == 0 && "The address of the "
            "memory to store the value to must be even");
            set_Mem_Word(theCpu->cpu_memory, a_value, d_value);
          break;

          default : 
            assert(TRUE == TRUE && "This instruction is not valid "
            "for this ISA"); 
          break;
        }
      break;

      case 7 :  // PUSH & POP
        switch(modifier)
        {
           case 1 :  // PUSHB
                     // SP ← SP - 1; M[SP] ← ($Rs)
                     //TOS points to last value pushed
             theCpu->cpu_register.R[0xF]->reg--;
             set_Mem_Byte(theCpu->cpu_memory, 
             theCpu->cpu_register.R[0xF]->reg, d_value);    
            break;

            case 2 :  // POPB
                      // $Rd(lob) ← M[SP]; SP ← SP + 1
              set_Reg_LOB(dPtr, 
              get_Mem_Byte(theCpu->cpu_memory, sp_value));
              theCpu->cpu_register.R[0xF]->reg++;  
            break;

            case 3 :  // PUSHW
                      // SP ← SP - 1; M[SP] ← ($Rs)(lob);
                      // SP ← SP - 1; M[SP] ← ($Rs)(hob)
              assert(sp_value % 2 == 0 && "The stack pointer must point"
              " to and even memory address to push a word");
              theCpu->cpu_register.R[0xF]->reg -= 2;
              set_Mem_Word(theCpu->cpu_memory, 
              theCpu->cpu_register.R[0xF]->reg, d_value);   
            break;

            case 4 :  // POPW
                      // $Rd(hob) ← M[SP]; SP ← SP + 1;
                      // $Rd(lob) ← M[SP]; SP ← SP + 1
                
                
              assert(sp_value % 2 == 0 && "The stack pointer must be"
              "even to load the value to the register");
              set_Reg(dPtr, get_Mem_Word(theCpu->cpu_memory, sp_value));
              theCpu->cpu_register.R[0xF]->reg += 2;
                
            break;

            default : 
              assert(TRUE == TRUE && "This instruction is not valid "
              "for this ISA"); 
            break;

        }
      break;

      default : 
		assert(TRUE == TRUE && "This instruction is not valid for "
		"this ISA");
      break;


    }
}

void alu(const unsigned short theType, CpuPtr theCpu)
{
    unsigned short ir = theCpu->cpu_int_reg->ir->reg;
    unsigned short modifier = ir & MODIFIER_MASK;
    unsigned short regA = (ir >> 3) & IMM_4_MASK;
    unsigned short regD = (ir >> 7) & IMM_4_MASK;
    
    theCpu->cpu_alu->A = theCpu->cpu_register.R[regD]->reg;
    theCpu->cpu_alu->B = theCpu->cpu_register.R[regA]->reg;
    
    switch(theType)
    {
      case 0 : //Shift left
        aluOpcode(theCpu->cpu_alu, SHL);
      break;
        
      case 1 : //ADD, SUB, MUL, DIV, AND, OR, XOR, NOT
        aluOpcode(theCpu->cpu_alu, modifier);        
      break;
            
      case 2 :  //Shift 
        aluOpcode(theCpu->cpu_alu, SHR);
      break;
            
      default: 
        assert(TRUE == TRUE && "This instruction is not valid for "
        "this ISA");
      break;
    }
}

void br_jsr(const unsigned short theType, CpuPtr theCpu)
{
    unsigned short ir = theCpu->cpu_int_reg->ir->reg;
    unsigned short modifier = ir & IMM_2_MASK;
    unsigned short regD = (ir >> 7) & IMM_4_MASK;
    unsigned short d_value = theCpu->cpu_register.R[regD]->reg;
    unsigned short  sp_value = theCpu->cpu_register.R[0xF]->reg;
    unsigned short imm11 = ir & IMM_11_MASK;
    unsigned short imm9 = imm11 >> 2;
    short pc = theCpu->cpu_int_reg->pc->reg;
    RegisterPtr pcPtr = theCpu->cpu_int_reg->pc;
    RegisterPtr swPtr = theCpu->cpu_int_reg->sw;
    unsigned char negative = (unsigned char) (swPtr->reg >> 15);
    unsigned char zero = (unsigned char) 
		((swPtr->reg >> 14) & LSB_MASK);
    unsigned char carryout = (unsigned char)
		((swPtr->reg >> 13) & LSB_MASK);
    unsigned char overflow = (unsigned char) 
		((swPtr->reg >> 12) & LSB_MASK);
    
 
    if(imm9 >= 0x0100)  //Sign extending immediate 9
    {
        imm9 = imm9 | 0xFE00;
    }
    
    switch(theType)
    {
	  case 0 : // PC ← (PC) + SEXT(immed11) - local branch
            
        if(imm11 >= 0x0400)  //Sign Extend if its negative
        {
          imm11 = imm11 | 0xF800;
        }
        
        theCpu->cpu_int_reg->pc->reg = pc + imm11;
      break;
       
      case 1 :  // PC ← ($Rr)

        theCpu->cpu_int_reg->pc->reg = d_value;
      break;
            
      case 2 : 
        switch(modifier)
        {
          case 0 : // PC ← PC + SEXT(immed9) if N=1

            if(negative)
            {
              theCpu->cpu_int_reg->pc->reg = pc + imm9;            
            }
          break;
          
          case 1 :  // PC ← PC + SEXT(immed9) if Z=1
                    
            if(zero)
            {
              theCpu->cpu_int_reg->pc->reg = pc + imm9;
            }
          break;
          
          case 2 :  // PC ← PC + SEXT(immed9) if C=1
                    
            if(carryout)
            {
              theCpu->cpu_int_reg->pc->reg = pc + imm9;
            }
          break;
                
          case 3 :  // PC ← PC + SEXT(immed9) if O=1
                    
            if(overflow)
            {
              theCpu->cpu_int_reg->pc->reg = pc + imm9;
            }
          break;
                 
          default : //ERROR
            assert(TRUE == TRUE && "This instruction is not valid "
            "for this ISA");
          break;
        }
      break;
            
      case 3 : 
        switch(modifier)
        {
          case 0 :  // PC ← PC + $Rr  if N=1 
            if(negative)
            {
              theCpu->cpu_int_reg->pc->reg = pc + d_value;
            }
          break;
          
          case 1 :  // PC ← PC + $Rr  if Z=1
            if(zero)
            {
              theCpu->cpu_int_reg->pc->reg = pc + d_value;
            }
          break;
          
          case 2 :  // PC ← PC + $Rr  if C=1 
            if(carryout)
            {
              theCpu->cpu_int_reg->pc->reg = pc + d_value;
            }
          break;
          
          case 3 :  // PC ← PC + $Rr  if O=1
            if(overflow)
            {
              theCpu->cpu_int_reg->pc->reg = pc + d_value;
            }
          break;
                 
          default : //ERROR
            assert(TRUE == TRUE && "This instruction is not valid "
            "for this ISA");
          break;
        }
      break;
            
      case 4 : // PUSHW(PC); PC ← ($Rr)

        theCpu->cpu_register.R[0xF]->reg -= 2;
        sp_value = theCpu->cpu_register.R[0xF]->reg;
        set_Mem_Word(theCpu->cpu_memory, sp_value, pc);
        theCpu->cpu_int_reg->pc->reg = d_value;
            
      break;
             
      case 5 : 
        switch(modifier)
        {
          case 0 : // PUSHW(PC); PC ← ($Rr) if N=1
            if(negative)
            {
              theCpu->cpu_register.R[0xF]->reg -= 2;
              sp_value = theCpu->cpu_register.R[0xF]->reg;
              set_Mem_Word(theCpu->cpu_memory, sp_value, pc);
              theCpu->cpu_int_reg->pc->reg = d_value; 
            }
          break;
          
          case 1 : // PUSHW(PC); PC ← ($Rr) if Z=1
            if(zero)
            {
              theCpu->cpu_register.R[0xF]->reg -= 2;
              sp_value = theCpu->cpu_register.R[0xF]->reg;
              set_Mem_Word(theCpu->cpu_memory, sp_value, pc);
              theCpu->cpu_int_reg->pc->reg = d_value; 
            }
          break;
                    
          case 2 : // PUSHW(PC); PC ← ($Rr) if C=1
            if(carryout)
            {
              theCpu->cpu_register.R[0xF]->reg -= 2;
              sp_value = theCpu->cpu_register.R[0xF]->reg;
              set_Mem_Word(theCpu->cpu_memory, sp_value, pc);
              theCpu->cpu_int_reg->pc->reg = d_value; 
            }
          break;
          
          case 3 : // PUSHW(PC); PC ← ($Rr) if O=1
            if(overflow)
            {
              theCpu->cpu_register.R[0xF]->reg -= 2;
              sp_value = theCpu->cpu_register.R[0xF]->reg;
              set_Mem_Word(theCpu->cpu_memory, sp_value, pc);
              theCpu->cpu_int_reg->pc->reg = d_value; 
            }
          break;
                    
          default : 
            assert(TRUE == TRUE && "This instruction is not valid "
            "for this ISA");
          break;
        }
      break;
            
      case 6 : // PUSHW(PC); PUSHW(SW); PC ← (IVT) + SEXT(vect6)
        
        theCpu->cpu_register.R[0xF]->reg -= 2;
        sp_value = theCpu->cpu_register.R[0xF]->reg;
        set_Mem_Word(theCpu->cpu_memory, sp_value, pc);
        theCpu->cpu_register.R[0xF]->reg -= 2;
        sp_value = theCpu->cpu_register.R[0xF]->reg;
        set_Mem_Word(theCpu->cpu_memory, sp_value, 
        theCpu->cpu_int_reg->sw->reg);
            
        unsigned short vect6 = imm11 >> 5;//Getting bits 6-11 of the IR
        if(vect6 >= 0x0020) //Sign extending vect6
        {
          vect6 = vect6 | 0xFFC0;
        }
            
        theCpu->cpu_int_reg->pc->reg = IVT + vect6;
      break;
            
      case 7 : 
        switch(modifier)
        {
          case 0 : // PC ← POPW
            set_Reg(pcPtr, get_Mem_Word(theCpu->cpu_memory, sp_value));
			theCpu->cpu_register.R[0xF]->reg += 2;
          break;
          
          case 1 : // SW ← POPW; PC ← POPW
            set_Reg(swPtr, get_Mem_Word(theCpu->cpu_memory, sp_value));
            theCpu->cpu_register.R[0xF]->reg += 2;
            sp_value = theCpu->cpu_register.R[0xF]->reg;
            set_Reg(pcPtr, get_Mem_Word(theCpu->cpu_memory, sp_value));
            theCpu->cpu_register.R[0xF]->reg += 2;
          break;
                    
        } 
      break;
            
      default : //ERROR
        assert(TRUE == TRUE && "This instruction is not valid "
        "for this ISA"); 
      break;
    }
    
}

void io_misc(const unsigned short theType, CpuPtr theCpu)
{   
    short i_o = theCpu->cpu_int_reg->ir->reg & LSB_MASK;
    unsigned short ir = theCpu->cpu_int_reg->ir->reg;
    unsigned short regD = (ir >> 7) & IMM_4_MASK;
    RegisterPtr dPtr = theCpu->cpu_register.R[regD];
    unsigned short d_value = theCpu->cpu_register.R[regD]->reg;
    
    switch(theType)
    {
    
      case 0 : 
        if(i_o)
        {
		  char status = VID + 0;
		  //char ctrl = VID + 1;
		  char data = VID + 2;
		
		  set_Mem_Byte(theCpu->cpu_memory, data, d_value);
		  set_Mem_Byte(theCpu->cpu_memory, status, 1);    
        }
        else
        {
		  int y,x;
		  getyx(my_win, y, x);
		  wmove(my_win, y, x);
		  char letter = wgetch(my_win);
          set_Reg(dPtr, letter);
        }       
      break;
      
      case 1 : 
        ;
      break;
      
      case 2 :  // HALT // RB ← 0
                //(run bit set to zero to stop clock,
                // requires reset to start)
        theCpu->RB = FALSE;
      break;
      
      case 3 : //NOP      
        ; 
      break;
      
      case 4 : 
        ; 
      break;
      
      case 5 : 
        ; 
      break;
      
      default : 
        assert(TRUE == TRUE && "This instruction is not valid "
        "for this ISA"); 
      break;

    }
}

 
