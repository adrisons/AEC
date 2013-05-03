default: compile execute

compile: 
	mpicc -o radix_mpi radix_mpi.c


execute:
	mpirun -n 2 ./radix_mpi 1000000