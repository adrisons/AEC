int main(int argc, char* argv[]){
	int numElementos = atoi(argv[1]);
	int i;
	int *array = malloc(MAX*sizeof(int)+1);
	static int n;
	struct timeval t0, t1, t;


	assert (gettimeofday (&t0, NULL) == 0);
	#pragma omp parallel
	{
		#pragma omp for schedule(static) private(i)
		for (n = 0; n < numElementos; n++){
			int r = rand()%100;
			array[n] = r;
		}
	}	
	radixsor (&array[0], numElementos);

	assert (gettimeofday (&t1, NULL) == 0);
	timersub(&t1, &t0, &t);
	printf("%ld.%06ld", (long int)t.tv_sec, (long int)t.tv_usec);

	return 0;
}