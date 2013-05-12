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
	(n)? (printf("\n")):printf("");

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

void inicializar_a_creciente(int * a, int n, int myrank){
	int i,j=0;
	int inicio;
	int fin;

	if(myrank==0){
		fin = n;
		inicio = 0;
	}else{
		fin = 2*n;
		inicio = n;		
	}
	for(i = inicio; i < fin; i++){
		a[j] = i;
		j++;
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
	int* b = malloc(sizeof(int)*n);
	int* a_intercambio = malloc(sizeof(int)*elemTot);
	int* bucket = malloc(sizeof(int)*10);
	int otro_rank = (myrank+1) % p;
	int iter = 0;
	MPI_Status status;
	
	inicializar_array(&a_intercambio[0], elemTot, -1);
	inicializar_array(&b[0], n, -1);

	m = maximo(a, n);
	MPI_Allreduce(&m, &m, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

	while (m / exp > 0){
		
		inicializar_array(bucket,10,0);

		// Se calcula el bucket
		for (i = 0; i < n; i++){
			bucket[a[i] / exp % 10]++;
		}

		// Se comunican los procesadores para tener los mismos valores en bucket
		MPI_Allreduce(&bucket[0], &bucket[0], 10, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

		if(myrank==1){
			// Se calculan las frecuencias acumulativas de los elementos del bucket 
			for (i = 1; i < 10; i++)
				bucket[i] += bucket[i - 1];
		}


		// El P0 se queda esperando por los datos del padre
		if(myrank==0){
			MPI_Recv(a_intercambio, elemTot, MPI_INT, 1, 3, MPI_COMM_WORLD, &status);
			MPI_Recv(&bucket[0], 10, MPI_INT, 1, 4, MPI_COMM_WORLD, &status);

			for(i=0;i<elemTot;i++){
				if(a_intercambio[i]!=-1){
					// Se adapta la posición del otro proceso
					pos_envio = i % n;
					b[pos_envio] = a_intercambio[i];
				}
			}
			inicializar_array(&a_intercambio[0], elemTot, -1);
		}


		// Usando el bucket se guardan en b los elementos de a ordenados
		for (i = n - 1; i >= 0; i--){
			
			int pos = (--bucket[a[i] / exp % 10]);

			rank_recv = floor(pos / n);
			rank_send = myrank;

			if( rank_send == rank_recv ){ 
				if(myrank==1)
					pos = pos % n;
				b[ pos ] = a[i];
			}else{
				if(myrank==0)
					pos = pos % n;				
				a_intercambio [pos] = a[i];
			}
		}

		// El P1 envía el array de intercambio al padre para continuar ordenando
		if(myrank==0){
			MPI_Send(a_intercambio, elemTot, MPI_INT, 1, 5, MPI_COMM_WORLD);
		}


		// El proceso padre envía los elementos que le corresponden del proceso hijo
		if(myrank==1){
			MPI_Send(a_intercambio, elemTot, MPI_INT, 0, 3, MPI_COMM_WORLD);
			MPI_Send(&bucket[0], 10, MPI_INT, 0, 4, MPI_COMM_WORLD);
			MPI_Recv(a_intercambio, elemTot, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);

			for(i=0;i<n;i++){
				if(a_intercambio[i]!=-1){
					b[i] = a_intercambio[i];
				}
			}
			inicializar_array(&a_intercambio[0], elemTot, -1);
		}
		// Se guardan en a los elementos ordenados
		for (i = 0; i < elemTot; i++){
			if(b[i] != -1){
//				int pos_a = i % n;
				a[i] = b[i];
			}
		}
		inicializar_array(&b[0], elemTot, -1);

		exp *= 10;
		iter++;
	}
	free(bucket);
//	free(b);
//	free(a_intercambio);
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
	struct timeval t0, t1, t;

	if(argc < 2){
		printf("Usage: mpirun -n numprocs ./radix numElementos \n");
		exit(1);
	}

	assert (gettimeofday (&t0, NULL) == 0);

	// Inicialización de MPI
	MPI_Init (&argc, &argv);


	/* myrank will contain the rank of the process
	MPI_COMM_WORLD is all processors together */
	MPI_Comm_rank (MPI_COMM_WORLD, &myrank);

	/* p es el número de procesos */
	MPI_Comm_size (MPI_COMM_WORLD, &p);
	
	/* n es el numero de elementos de cada proceso*/
	int n = numElementos/p;

	int *a = malloc(n*sizeof(int));

	inicializar_array_random(&a[0], n);


	// Ejecuta el algoritmo de ordenamiento
	radixsort_parallel (a, numElementos, p, myrank);

 

	if (myrank == 0){

//		printf("Sol: \n");
//		print_array(a, n, 0);
    	MPI_Recv(a, n, MPI_INT, 1, 123, MPI_COMM_WORLD, &status);
//    	print_array(a, n, 1);

   	}else{
		MPI_Send(a, n, MPI_INT, 0, 123, MPI_COMM_WORLD);
	}

	free(a);

	MPI_Finalize ();

	if (myrank == 0){
		assert (gettimeofday (&t1, NULL) == 0);
		timersub(&t1, &t0, &t);
		printf("%ld.%06ld", (long int)t.tv_sec, (long int)t.tv_usec);
//		printf ("Tiempo      = %ld:%ld(seg:mseg)\n", t.tv_sec, t.tv_usec/1000);


	}

}
