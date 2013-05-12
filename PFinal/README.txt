qsub -l arch=amd,num_proc=1,s_rt=00:05:00,s_vmem=2G,h_fsize=20G job.sh
https://www.cesga.es/es/infraestructuras/computacion/svg#Acceso

compute --memory 8 
module load impi



qsub -l arch=sandy,num_proc=2,s_rt=1:00:00,s_vmem=10G,h_fsize=20G -pe 2 tabla_mpi.py

-----
Trabajos secuenciales:

qsub -l arch=[amd|sandy],num_proc=1,s_rt=1:00:00,s_vmem=2G,h_fsize=20G trabajo.sh

Trabajos paralelos openmp:

qsub -l arch=[amd|sandy],num_proc=16,s_rt=1:00:00,s_vmem=2G,h_fsize=20G trabajo.sh

Trabajos paralelos MPI:

qsub -l arch=[amd|sandy],num_proc=1,s_rt=1:00:00,s_vmem=2G,h_fsize=20G -pe mpi 16 test.sh
