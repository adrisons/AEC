	#pragma omp parallel
	{
		#pragma omp for schedule(static) private(i)
		for (i = 0; i < n; i++){			
			if (a[i] > m){
				m = a[i];
			}
		}
	}