qsub -l arch=amd,num_proc=1,s_rt=00:05:00,s_vmem=2G,h_fsize=20G job.sh
https://www.cesga.es/es/infraestructuras/computacion/svg#Acceso

compute --memory 8 
module load impi

Uso: compute [--arch <ARCH>] [--memory <MEM>] [--stack <STACK>] [--node {c4-1|c4-2}]

