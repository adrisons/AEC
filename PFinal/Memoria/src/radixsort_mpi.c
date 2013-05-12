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
		// El P0 se queda esperando por los datos del P1
		if(myrank==0){
			MPI_Recv(a_intercambio, elemTot, MPI_INT, 1, 3, MPI_COMM_WORLD, &status);
			MPI_Recv(&bucket[0], 10, MPI_INT, 1, 4, MPI_COMM_WORLD, &status);
			for(i=0;i<elemTot;i++){
				if(a_intercambio[i]!=-1){
					// Se adapta la posicion del otro proceso
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
		// El P0 envia el array de intercambio al P1 para continuar ordenando
		if(myrank==0){
			MPI_Send(a_intercambio, elemTot, MPI_INT, 1, 5, MPI_COMM_WORLD);
		}
		// El proceso 1 envia los elementos que le corresponden del proceso 0
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
				a[i] = b[i];
			}
		}
		inicializar_array(&b[0], elemTot, -1);
		exp *= 10;
		iter++;
	}
	free(bucket);
}