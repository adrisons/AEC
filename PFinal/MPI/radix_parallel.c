#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#define MAX 100000000




void print_array( int * a, int numElementos, int n){

	int i;
	for (i = 0; i < numElementos; i++){
		printf("%d ", a[i]);
	}
	(n)? (printf("\n\n")):printf("");

}
/* Dado un array a y su número de elementos se calcula el máximo*/
int maximo(int * a, int numElementos){
	int m = a[0], i;
	for (i = 0; i < numElementos; i++){
		if (a[i] > m){
			m = a[i];
		}
	}	
	return m;
}

/*Inicializa aleatoriamente el array a, de numElementos elementos*/
void inicializar_array_random(int * a, int numElementos){
	int n;
	for (n = 0; n < numElementos; n++){
		int r = rand()%1000000;
		a[n] = r;
	}	
}

void inicializar_a_decreciente(int * a, int n, int myrank){
	int i,j=0;
	int inicio;
	int fin;

	if(myrank==0){
		inicio = 2*n -1;
		fin = n;
	}else{
		inicio = n -1;
		fin = 0;		
	}
	for(i = inicio; i >= fin; i--){
		a[j] = i;
		j++;
	}

}

void inicializar_array(int* a, int size, int num){

	int i;
	for (i = 0; i < size; i++){
		a[i] = num;
	}	
}


void radixsort_parallel(int* a, int elemTot, int p, int myrank){
	int i, m, exp = 1;
	struct timeval t0, t1, t;
	int rank_send, rank_recv;
	int n = elemTot/p;
	int pos_envio = -1;
	int* b = malloc(n*sizeof(int)*elemTot);
	int* a_intercambio = malloc(n*sizeof(int)*elemTot);
	int otro_rank = (myrank+1) % p;
	int iter = 0;
	MPI_Status status;

	inicializar_array(&a_intercambio[0], n, -1);
	inicializar_array(&b[0], n, -1);
	
	m = maximo(a, n);
printf("----------------------------------------\n");
printf("[rank=%d]:",myrank);
print_array(a,n,1);
printf("----------------------------------------\n");
	while (m / exp > 0){
		printf("[rank=%d] ITERACION %d\n",myrank,iter);
		int bucket[10] = {0};
		// Se calcula el bucket
		for (i = 0; i < n; i++){
			bucket[a[i] / exp % 10]++;
		}

		// Se comunican los procesadores para tener los mismos valores en bucket
		MPI_Allreduce(&bucket[0], &bucket[0], 10, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

		// Se calculan las frecuencias acumulativas de los elementos del bucket 
		for (i = 1; i < 10; i++)
			bucket[i] += bucket[i - 1];


		// Usando el bucket se guardan en b los elementos de a ordenados
		for (i = n - 1; i >= 0; i--){
			
			int pos = (--bucket[a[i] / exp % 10]);
			

			rank_recv = floor(pos / n);
			rank_send = myrank;

			if( rank_send == rank_recv ){ 
				b[ pos ] = a[i];
			}else{
				// Se adapta la posición para el otro proceso
				pos_envio = pos % n;
//				printf("[rank=%d]	pos_envio = (pos=)%d mod %d = %d\n",myrank,pos,n,pos_envio);

				a_intercambio [pos_envio] = a[i];
//				printf("[rank=%d]	a_intercambio: ",myrank);
//				print_array(a_intercambio,n,1);
			}
		}
//			printf("[rank=%d]	a_intercambio FUERA: ",myrank);
//			print_array(a_intercambio,n,1);

		// Se guardan en a los elementos ordenados
		for (i = 0; i < n; i++){
			a[i] = b[i];
		}

		if(pos_envio != -1){
			printf("	[rank=%d]-->>[rank=%d]: ",myrank,otro_rank);
			print_array(a_intercambio,n,1);
			
			MPI_Send(a_intercambio, n, MPI_INT, otro_rank, 3, MPI_COMM_WORLD);
			MPI_Recv(a_intercambio, n, MPI_INT, otro_rank, 3, MPI_COMM_WORLD, &status);
			
			printf("	[rank=%d]<<--[rank=%d]: ",myrank,otro_rank);
			print_array(a_intercambio,n,1);
		}
/*
	if (myrank == 0){
		MPI_Send(a_intercambio, n, MPI_INT, 1, 3, MPI_COMM_WORLD);
		MPI_Recv(a_intercambio, n, MPI_INT, 0, 3, MPI_COMM_WORLD, &status);

   	}else{
		MPI_Send(a_intercambio, n, MPI_INT, 0, 3, MPI_COMM_WORLD);
    	MPI_Recv(a_intercambio, n, MPI_INT, 1, 3, MPI_COMM_WORLD, &status);
	}

*/

		for (i = 0; i < n; i++){
			if(a_intercambio[i]!=-1){
				a[i] = a_intercambio[i];
			}
		}		
		printf("[rank=%d] a = ",myrank);
		print_array(a,n,1);

		exp *= 10;
		printf("[rank=%d]fin del while...\n",myrank);
		iter++;
		

	}
}
 
 

void print_whoami(int myrank){
	
	if (myrank == 0){
		printf("Padre [rank = %d]\n", myrank);
   	}else{
		printf("Hijo [rank = %d]\n", myrank);  	
	}
}


int main(int argc, char* argv[]){
	int numElementos = atoi(argv[1]);
	int i, p, myrank;
	MPI_Status status;

	printf("argc=%d\n",argc);
	if(argc < 2){
		printf("Usage: mpirun -n numprocs ./radix numElementos \n");
		exit(1);
	}
	// Inicialización de MPI
	MPI_Init (&argc, &argv);



	/* myrank will contain the rank of the process
	MPI_COMM_WORLD is all processors together */
	MPI_Comm_rank (MPI_COMM_WORLD, &myrank);
	
	printf("INICIO...");
	print_whoami(myrank);

	/* p es el número de procesos */
	MPI_Comm_size (MPI_COMM_WORLD, &p);
	
	/* n es el numero de elementos de cada proceso*/
	int n = numElementos/p;

	int *a = malloc(n*sizeof(int));

	inicializar_a_decreciente(&a[0], n, myrank);


	// Ejecuta el algoritmo de ordenamiento
	radixsort_parallel (a, numElementos, p, myrank);

 

	if (myrank == 0){
printf("\nRESULTADO:\n");
printf("----------------------------------------\n");		
		printf("Padre|Hijo\n");
		printf("[");
		print_array(a, n, 0);
		printf("|");
    	MPI_Recv(a, n, MPI_INT, 1, 123, MPI_COMM_WORLD, &status);

    	print_array(a, n, 0);
    	printf("]\n");
printf("----------------------------------------\n");    	
   	}else{
		MPI_Send(a, n, MPI_INT, 0, 123, MPI_COMM_WORLD);
	}
    

	printf("FIN ");
	print_whoami(myrank);

	MPI_Finalize ();



	return 0;
}
