#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define MAX 100000000




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
		int r = rand()%100;
		a[n] = r;
	}	
}


void radixsor(int* a, int numElementos){
	int i, m, exp = 1;
	struct timeval t0, t1, t;
	int *b = malloc(MAX*sizeof(int)+1);


	assert (gettimeofday (&t0, NULL) == 0);


	inicializar_array_random(a, numElementos);
	printf("a inicial: ");
	for (i = 0; i < numElementos; i++){
		printf("%d ", a[i]);
	}
	printf("\n");
	m = maximo(a, numElementos);
	printf("\nMAXIMO= %d\n", m);
	while (m / exp > 0){

		printf("\nNUEVA ITERACION\n");
		int bucket[10] = {0};
		for (i = 0; i < numElementos; i++){
			bucket[a[i] / exp % 10]++;
		}

		printf("bucket   : ");
		for (i = 0; i < 10; i++){
		printf("%d ", bucket[i]);
		}
		printf("\n");
		for (i = 1; i < 10; i++)
			bucket[i] += bucket[i - 1];
		
		printf("bucket AC: ");
		for (i = 0; i < 10; i++){
		printf("%d ", bucket[i]);
		}
		printf("\n");

		for (i = numElementos - 1; i >= 0; i--) {  // array a: i , i_global(i,rank,P)
			int pos = --bucket[a[i] / exp % 10];
			b[ pos ] = a[i];
			int rank_send = (i<numElementos/2)?0:1;
			int rank_recv = (pos<numElementos/2)?0:1;
			printf("==>> MPI process rank=%d maps a[%d]=%d  to position b[%d]=%d  ",rank_send,i,a[i],pos,b[pos]);
			if( rank_send != rank_recv ) printf("in MPI process rank=%d ",rank_recv);
			printf("\n");
		
		}

//        printf("paso while %d : ,",paso_while);
		printf("b     : ");
		for (i = 0; i < numElementos; i++){
		printf("%d ", b[i]);
		}
		printf("\n");

		printf("bucket FN: ");
		for (i = 0; i < 10; i++){
		printf("%d ", bucket[i]);
		}
		printf("\n");

		for (i = 0; i < numElementos; i++){
			a[i] = b[i];
		}

		exp *= 10;
		}

	assert (gettimeofday (&t1, NULL) == 0);
	timersub(&t1, &t0, &t);
	printf ("Tiempo      = %ld:%d(seg:mseg)\n", t.tv_sec, t.tv_usec/1000);

}
 
 
int main(int argc, char* argv[]){
	int numElementos = atoi(argv[1]);
	int *a = malloc(MAX*sizeof(int)+1);
	int i;
	
	radixsor (&a[0],numElementos);
printf("a final:    ");
	for (i = 0; i < numElementos; i++){
		printf("%d ", a[i]);
	}
	printf("\n");

	return 0;
}
