Tester: monitor.o cpu.o alu.o register.o memory.o
	gcc monitor.o cpu.o alu.o register.o memory.o -o Tester -lncurses

monitor.o : registerFile.h globals.h cpu.o alu.o register.o memory.o
	 gcc -Wall monitor.c -c -lncurses

cpu.o : cpu.h globals.h alu.o register.o memory.o
	gcc -Wall cpu.c -c

alu.o  : alu.c alu.h globals.h
	 gcc -Wall alu.c -c 

register.o : register.c register.h globals.h
	gcc -Wall register.c -c

memory.o : memory.c memory.h
	gcc -Wall memory.c -c

clean : 
	rm Tester monitor.o cpu.o alu.o register.o memory.o 


