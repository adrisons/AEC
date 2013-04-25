#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define MAX 100000000

void radixsor(int *a, int n){
int i, m = a[0], exp = 1;
struct timeval t0, t1, t;
int *b = malloc(MAX*sizeof(int)+1);

assert (gettimeofday (&t0, NULL) == 0);

for (i = 0; i < n; i++){
	if (a[i] > m){
		m = a[i];
	}
}
while (m / exp > 0){
	int bucket[10] = {0};
	for (i = 0; i < n; i++){
		bucket[a[i] / exp % 10]++;
	}
	for (i = 1; i < 10; i++)
		bucket[i] += bucket[i - 1];
	for (i = n - 1; i >= 0; i--)
		b[--bucket[a[i] / exp % 10]] = a[i];
	for (i = 0; i < n; i++){
		a[i] = b[i];
	}
	exp *= 10;
	}

	assert (gettimeofday (&t1, NULL) == 0);
	timersub(&t1, &t0, &t);
	printf ("Tiempo      = %ld:%ld(seg:mseg)\n", t.tv_sec, t.tv_usec/1000);

}
 
 
int main(int argc, char* argv[]){
	int numElementos = atoi(argv[1]);
	int i;
	int *array = malloc(MAX*sizeof(int)+1);
	static int n;

	for (n = 0; n < numElementos; n++){
		int r = rand()%1000000;
		array[n] = r;
	}	
	radixsor (&array[0], numElementos);
/*	for (i = 0; i < numElementos; i++){
		printf("%d ", array[i]);
	}
*/
	return 0;
}
