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
 * This is a helper function that help the main format the printing for 
 * the values of the registers and memory
 * @param theCpu the CPU to get values from
 * @param startDump the start address to dump memory
 */
void updateDisplay(const CpuPtr theCpu, unsigned short startDump);

/**
 * External global variable used so the CPU can manipulate the window
 */ 
WINDOW *my_win;

/**
 * The main method that will run the SC2 simulator
 * @param argc the number of command line argument
 * @param argv command line argument
 * @return 0 if it is successful.
 */
int main(int argc, char* argv[])
{
    /**
     * Constructs the memory used by the CPU
     */ 
    MemoryPtr myMem = MemoryConstructor();
    
    /**
     * Constructs the CPU to be used by the program
     */ 
    CpuPtr myCpu = cpuConstructor(myMem);
    
    /**
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
     * This will hold the string for the file name.  The size of this 
     * array is 80.
     */
    char filename[FILENAME_STRING_LENGTH];
    
    /**
     * Used to store user command line response.  It is set to valid 
     * once the program is complete.  
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
     * Used to hold the instructions read and is used to store this 
     * value into the memory
     */
    unsigned short instruction = 0x3000;
    unsigned short begin;
    
   
    initscr();  //Starts ncurses mode 
    cbreak();
    getmaxyx(stdscr, row, col);
    updateDisplay(myCpu, start_memory_dump);
    //Magic numbers through out the file are just x and y co-ordiantes 
    //or abitrary height and width values chosen 
    int height = 18;
    int width = col - 35;
    int x_coord = 0;
    int y_coord = 33;
    WINDOW *boarder = newwin(height, width, x_coord, y_coord);
    height = 16;
    width = col - 37;
    x_coord = 1;
    y_coord = 34;
    my_win = newwin(height, width, x_coord, y_coord);
    box(boarder, 0, 0);
    wrefresh(boarder);
    move(21, 0);
    
    while(!valid)
    {
        user_command = getch(); 
        updateDisplay(myCpu, start_memory_dump);
        switch(user_command)
        {
			case '1' :
        
            mvprintw(21, 0, "Enter a file name : "); 
            refresh();
            echo();
            getstr(filename);
			
            while(!fopen(filename, "r"))
            {
                mvprintw(21, 0,"That file does not exist.  "
                        "Enter a file name: ");
                getstr(filename);
                move(21, 0);
                clrtoeol();
                move(22, 0);
                clrtoeol();
                refresh();
                
            }
            
            	fp = fopen(filename, "r");
	
			    
			char newline;
			fscanf(fp,"%X", &instruction);
			newline = fgetc(fp);
            myCpu->cpu_int_reg->pc->reg = instruction;
            begin = instruction;
            memory_address = instruction;
            while(!feof(fp))
            {  
                fscanf(fp,"%X", &instruction);
                newline = fgetc(fp);
                set_Mem_Word(myMem, begin, instruction);
                begin += 2;
                memory_address +=2;
                               
            }
            begin -=2;
            set_Mem_Word(myMem, begin, 0);
            start_memory_dump = 0x3000; //0x3000 is a memory address
            updateDisplay(myCpu, start_memory_dump);
            load_successful = TRUE;
            myCpu->RB = TRUE;
            mvprintw(22, 0,"File loaded:  %s", filename);
            move(21, 0);
            fclose(fp);
		
                     
        break;
        
        case '2' :
        
           if(load_successful == 0)
           {
               mvprintw(21, 0, "You need to load a file to .  Enter a"
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
                
  //The conditional statement below will simulate the monitor///////////
					if( get_Mem_Byte(myMem, VID))
					{
					wprintw(my_win, "%c", get_Mem_Byte(myMem, VID + 2));
					set_Mem_Byte(myMem,VID, 0);	
					wrefresh(my_win);
					}
                
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
                  mvprintw(21, 0,"Choose a Command:  ");
                  clrtoeol();
               }
           }
            
        break;
        
        case '3' :
        
           if(load_successful == 0)
           {
               mvprintw(21, 0, "You need to load a file to .  Enter a"
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
                mvprintw(21 , 0, "Command : Step");
                
 //The conditional statement below will simulate the monitor///////////
                if( get_Mem_Byte(myMem, VID))
                {
					wprintw(my_win, "%c", get_Mem_Byte(myMem, VID + 2));	
					set_Mem_Byte(myMem,VID, 0);
					wrefresh(my_win);
				}

               }
               else
               {
               
                char quit;
                mvprintw(21, 0,"The program has ended.  Quit Y/N?  ");
                quit = getch();
                if('Y' == quit || 'y' == quit )
                {
                   valid = TRUE;
                }
                else
                {
                  mvprintw(21, 0,"Choose a Command:  ");
                  clrtoeol();
                }
                
               }
                   
           }
        break;
        
        case '4' :
        
           if(load_successful == 0)
           {
             mvprintw(21, 0, "You need to load a file to .  Enter a"
                       " command: ");
             refresh();
           }
           else
           { 
             mvprintw(21, 0, "Please enter the start of memory address"
                    " to dump: ");
             echo();
             scanw("%X", &start_memory_dump);
             updateDisplay(myCpu, start_memory_dump);
           }
            
        break;
       default :
        
           mvprintw(21, 0,"You did not pick a valid choice. Enter a"
                   "command : ");
          
           refresh();
        break;
		}
    }
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
    mvprintw(18, 0, "SW : %.4X", theCpu->cpu_int_reg->sw->reg);
    mvprintw(18, 20, "IR : %.4X", theCpu->cpu_int_reg->ir->reg);
    mvprintw(18, 40, "PC : %.4X", theCpu->cpu_int_reg->pc->reg);
    mvprintw(19, 0, "------------------------------------------------"
            "--------------------------------------\n");
    mvprintw(20, 0,"Command: 1) Load Program, 2) run program 3) step "
            "program 4) dump memory row \n");
    mvprintw(0,0, "Registers");
    mvprintw(0,15, "Memory");
    for(i = 0; i < SIZE_OF_REG_FILE; i++)
    {
      mvprintw(i + 1, 0, "$R%X : %.4X", i, 
		theCpu->cpu_register.R[i]->reg);  
      memAddress = startDump + i*2;
      mvprintw( i + 1, 15, "%.4X : %.2X  %.2X", memAddress, 
		theCpu->cpu_memory->mem[memAddress], 
        theCpu->cpu_memory->mem[memAddress +1]);
    }
    
    move(21, 0);
    clrtoeol();
    refresh();
    noecho();

}


