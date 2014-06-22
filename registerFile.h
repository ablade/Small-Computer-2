/*
 * File:   RegisterFile.h
 * Team: Alvin & Bob
 * Author: Alvin Baldemeca, Bob Landowski
 * Created on January 20, 2013, 6:03 PM
 *
 */

 /**
 * This file contains the structure defenition for the RegisterFileStr.
 * @author Alvin Baldemeca, Bob Landowski
 * @version Winter 2013
 */
#include "register.h"
#ifndef REGISTERFILE_H
#define	REGISTERFILE_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct
    {
        RegisterPtr R[16];
    }RegisterFilePtr;

    RegisterFilePtr regFileConstructor();
    
#ifdef	__cplusplus
}
#endif

#endif	/* REGISTERFILE_H */

