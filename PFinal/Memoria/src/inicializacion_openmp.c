	#pragma omp parallel
	{
		#pragma omp for schedule(static) private(i)
		for (n = 0; n < numElementos; n++){
			int r = rand()%100;
			array[n] = r;
		}
	}