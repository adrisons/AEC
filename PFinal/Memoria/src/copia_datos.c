		#pragma omp parallel
		{
			#pragma omp for schedule(static) private(i)
			for (i = 0; i < n; i++){
				a[i] = b[i];
			}
		}