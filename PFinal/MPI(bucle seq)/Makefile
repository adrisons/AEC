default: normal
	
normal: compile execute


gdb: compile_gdb execute_gdb

compile_gdb: 
	mpicc -g -o radix radix_parallel.c


execute_gdb:
	mpirun -np 2 xterm -e gdb ./radix



compile: 
	mpicc -o radix radix_parallel.c


execute:
	mpirun -n 2 ./radix 12