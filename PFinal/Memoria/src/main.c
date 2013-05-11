int main(int argc, char* argv[]){
	int numElementos = atoi(argv[1]);
	int i, p, myrank;
	MPI_Status status;
	struct timeval t0, t1, t;

	if(argc < 2){
		printf("Usage: mpirun -n numprocs ./radix numElementos \n");
		exit(1);
	}
	/* Inicializacion de MPI*/
	MPI_Init (&argc, &argv);
	/* myrank will contain the rank of the process
	MPI_COMM_WORLD is all processors together */
	MPI_Comm_rank (MPI_COMM_WORLD, &myrank);
	/* p es el numero de procesos */
	MPI_Comm_size (MPI_COMM_WORLD, &p);
	/* n es el numero de elementos de cada proceso*/
	int n = numElementos/p;
	int *a = malloc(n*sizeof(int));
	inicializar_a_creciente(&a[0], n, myrank);
	/* Ejecuta el algoritmo de ordenamiento*/
	radixsort_parallel (a, numElementos, p, myrank);

	if (myrank == 0)
    	MPI_Recv(a, n, MPI_INT, 1, 123, MPI_COMM_WORLD, &status);
   	else
		MPI_Send(a, n, MPI_INT, 0, 123, MPI_COMM_WORLD);

	free(a);
	MPI_Finalize ();
}