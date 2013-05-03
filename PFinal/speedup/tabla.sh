#!/bin/bash

output="output.txt"

n1=10000
n2=500000
n3=1000000000

echo -ne "numElementos\t" > output	
echo -ne "Tiempo seq(seg:mseg)\t" >> output
echo -ne "Tiempo mpi(seg:mseg)\t" >> output
echo -ne "Speedup\n" >> output

	echo -ne $n1 "\t" >> output	
	t_seq=$(./radix_seq $n1) 
	echo -n $t_seq >> output
	echo -ne "\t\t" >> output
	t_mpi=$(mpirun -np 2 ./radix_mpi $n1)
	echo -n $t_mpi >> output
	echo -ne "\t\t" >> output
	echo -n $(($t_seq / $t_mpi)) >> output
	echo -ne "\n" >> output


