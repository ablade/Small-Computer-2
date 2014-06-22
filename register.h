/*
 * File:   register.h
 * Team: Alvin & Bob
 * Author: Alvin Baldemeca, Bob Landowski
 * Created on January 20, 2013, 6:03 PM
 *
 */

 /**
 * This file contains the structure defenition for the RegisterStr.
 * It also contains the function prototypes for the register.c
 *
 * @author Alvin Baldemeca, Bob Landowski
 * @version Winter 2013
 */
#include "globals.h"
#ifndef REGISTER_H
#define	REGISTER_H

#ifdef	__cplusplus
extern "C" {
#endif
    typedef struct{
        unsigned short reg;
    }RegisterStr;

    typedef RegisterStr* RegisterPtr;


    RegisterPtr RegisterConstructor(void);
    void set_Reg_LOB(RegisterPtr const sc_register, byte const the_byte);
    void set_Reg_HOB(RegisterPtr const sc_register, byte const the_byte);
    void set_Reg(RegisterPtr const sc_register, unsigned short const the_data );
    unsigned short read_Reg(RegisterPtr const sc_register);
    



#ifdef	__cplusplus
}
#endif

#endif	/* REGISTER_H */

