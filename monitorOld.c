/*
 * File: monitor.c
 * Team: Alvin & Bob
 * Author: Alvin Baldemeca, Bob Landowski
 *
 * Created Feb 2013 
 */

 /**
 * This file represents a debug monitor simulator for the SC2 CPU.  
 * 
 *
 * @author Alvin Baldemeca, Bob Landowski
 * @version Winter 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "cpu.h"
#include "memory.h"
#include "alu.h"
#include "registerFile.h"
#include "register.h"
#include "globals.h"
#define  FILENAME_STRING_LENGTH 80

/**
 * This is a helper function that help the main format the printing for the
 * values of the registers and memory
 * @param theCpu the CPU to get values from
 * @param startDump the start address to dump memory
 */
void updateDisplay(const CpuPtr theCpu, unsigned short startDump);

/**
 * 
 * @param argc the number of command line argument
 * @param argv command line argument
 * @return 0 if it is successful.
 */
int main(int argc, char* argv[])
{
    //Constructs the memory
    MemoryPtr myMem = MemoryConstructor();
    
    //Constructs the CPU
    CpuPtr myCpu = cpuConstructor(myMem);
    
    /*
     * Program file pointer
     */ 
    FILE *fp;
    
    /**
     * Window row value/size
     */
    int row;
    
    /**
     *  Window column value/size
     */
    int col;
    
    /**
     * Used to load instructions to the memory address 
     */
    unsigned short memory_address;
    
    /**
     * Use to hold the value for the start address of the memory dump.
     * It is initialize to 0x3000 to display the program hex values
     */  
    unsigned short start_memory_dump = 0x3000;
    
    /**
     * This will hold the string for the file name.  The size of this array
     * is 80.
     */
    char filename[FILENAME_STRING_LENGTH];
    
    /**
     * Used to store user command line response.  It is set to valid once the 
     * program is complete.  
     */
    int valid = FALSE;
    
    /**
     * This is set to 1 if the file given by the user exist.
     */
    int load_successful = FALSE;
    
    /**
     * Stores the command the user choose.
     */
    char user_command;
    
    /**
     * Used to hold the instructions read and is used to store this value into
     * the memory
     */
    unsigned short instruction = 0x3000;
    
   
    initscr();  //Starts ncurses mode 
    cbreak();
    getmaxyx(stdscr, row,col);
    updateDisplay(myCpu, start_memory_dump);
    
    while(!valid)
    {
        user_command = getch(); 
        updateDisplay(myCpu, start_memory_dump);
        if(user_command == '1')
        {
            //load_successful = FALSE;
            mvprintw(row - 2, 0, "Enter a file name : "); 
            refresh();
            echo();
            getstr(filename);
			
            while(!fopen(filename, "r"))
            {
                mvprintw(row - 2, 0,"That file does not exist.  "
                        "Enter a file name: ");
                getstr(filename);
                move(row - 2, 0);
                clrtoeol();
                move(row - 1, 0);
                clrtoeol();
                refresh();
                
            } 
            
            char newline;
            fp = fopen(filename, "r");
            fscanf(fp,"%X", &instruction);
            newline = fgetc(fp);
            myCpu->cpu_int_reg->pc->reg = instruction;
            memory_address = instruction;
            while(!feof(fp))
            {  
                fscanf(fp,"%X", &instruction);
                newline = fgetc(fp);
                set_Mem_Word(myMem, memory_address, instruction);
                memory_address +=2;
                               
            }
            start_memory_dump = 0x3000;
            updateDisplay(myCpu, start_memory_dump);
            load_successful = TRUE;
            myCpu->RB = TRUE;
            mvprintw(row - 1, 0,"File loaded:  %s", filename);
            move(row - 2, 0);
            fclose(fp);
                     
        }
        
        else if(user_command == '2')
        {
           if(load_successful == 0)
           {
               mvprintw(row - 2, 0, "You need to load a file to .  Enter a"
                       " command: ");
               refresh();
           }
           else
           { 
               while(myCpu->RB)
               {
                cpuFetch(myCpu);
                cpuDecodeExec(myCpu);
                updateDisplay(myCpu, start_memory_dump);
               }
               char quit;
               printw("The program has ended.  Quit Y/N?  ");
               quit = getch();
               if('Y' == quit  || 'y' == quit )
               {
                  valid = TRUE;
               }
               else
               {
                  mvprintw(row -2, 0,"Choose a Command:  ");
                  clrtoeol();
               }
           }
            
        }
        else if(user_command == '3')
        {
           if(load_successful == 0)
           {
               mvprintw(row - 2, 0, "You need to load a file to .  Enter a"
                       " command: ");
               refresh();
           }
           else
           {  
               if(myCpu->RB)
               {
                cpuFetch(myCpu);
                cpuDecodeExec(myCpu);
                updateDisplay(myCpu, start_memory_dump);
                mvprintw(row -2 , 0, "Command : Step");
               }
               else
               {
               
                    char quit;
                    mvprintw(row -2, 0,"The program has ended.  Quit Y/N?  ");
                    quit = getch();
                    if('Y' == quit || 'y' == quit )
                    {
                        valid = TRUE;
                    }
                    else
                    {
                        
                        mvprintw(row -2, 0,"Choose a Command:  ");
                        clrtoeol();
                    }
               }
                   
           }
        }
        else if(user_command == '4')
        {
           if(load_successful == 0)
           {
               mvprintw(row - 2, 0, "You need to load a file to .  Enter a"
                       " command: ");
               refresh();
           }
           else
           { 
            mvprintw(row - 2, 0, "Please enter the start of memory address"
                    " to dump: ");
            echo();
            scanw("%X", &start_memory_dump);
            updateDisplay(myCpu, start_memory_dump);
           }
            
        }
       else
        {
           mvprintw(row -2, 0,"You did not pick a valid choice. Enter a"
                   "command : ");
          
           refresh();
        }
    }
     /*
    printw("The program has ended.  Thanks for testing it!!! Press any key"
            "to quit.");
    refresh();
    user_command = getch();
      */ 
    endwin();

    return EXIT_SUCCESS;
}



/**
 * This is a helper method that formats the print output.
 * @param theCpu the CPU to get values from
 * @param startDump the start address of the memory to dump
 */
void updateDisplay(const CpuPtr theCpu, unsigned short startDump)
{
    int row, col;
    int i;
    int memAddress;
    startDump = startDump >> 1 << 1;
    getmaxyx(stdscr, row,col);
    mvprintw(row - 5, 0, "SW : %.4X", theCpu->cpu_int_reg->sw->reg);
    mvprintw(row - 5, 20, "IR : %.4X", theCpu->cpu_int_reg->ir->reg);
    mvprintw(row - 5, 40, "PC : %.4X", theCpu->cpu_int_reg->pc->reg);
    mvprintw(row - 4, 0, "------------------------------------------------"
            "--------------------------------------\n");
    mvprintw(row - 3, 0,"Command: 1) Load Program, 2) run program 3) step "
            "program 4) dump memory row \n");
    mvprintw(0,0, "Registers");
    mvprintw(0,15, "Memory");
    for(i = 0; i < SIZE_OF_REG_FILE; i++)
    {
      mvprintw(i + 1, 0, "$R%X : %.4X", i, theCpu->cpu_register.R[i]->reg);  
      memAddress = startDump + i*2;
      mvprintw( i + 1, 15, "%.4X : %.2X  %.2X", memAddress, 
              theCpu->cpu_memory->mem[memAddress], 
              theCpu->cpu_memory->mem[memAddress +1]);
    }
    
    move(row - 2, 0);
    clrtoeol();
    refresh();
    noecho();

}
