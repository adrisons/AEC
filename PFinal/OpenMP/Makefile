default: compile execute

compile: 
	gcc  -O3 -fopenmp -o radix_openmp radix_openmp.c -lm

execute:
	export OMP_NUM_THREADS=30
	./radix_openmp 10000

clean:
	rm radix_openmp