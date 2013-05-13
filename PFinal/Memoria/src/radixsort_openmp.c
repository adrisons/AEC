void radixsor(int *a, int n){
	int i, m = a[0], exp = 1;
	int *b = malloc(MAX*sizeof(int)+1);

	#pragma omp parallel
	{
		#pragma omp for schedule(static) private(i)
		for (i = 0; i < n; i++){			
			if (a[i] > m){
				m = a[i];
			}
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
		
		#pragma omp parallel
		{
			#pragma omp for schedule(static) private(i)
			for (i = 0; i < n; i++){
				a[i] = b[i];
			}
		}
		exp *= 10;
	}

}