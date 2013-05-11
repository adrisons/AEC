default: compile execute

compile: 
	mpicc -o radix_par radix_parallel.c


execute:
	mpirun -n 2 ./radix_par 2000000