import sys
import subprocess

sys.stdout = open('output', 'w')

n0=10
n1=500
n2=1000
n3=5000
n4=10000
n5=50000
n6=100000
n7=500000
n8=1000000
n9=5000000
n10=50000000
n11=100000000
n12=500000000
n13=1000000000
n14=5000000000
n15=10000000000
n16=50000000000
n17=100000000000
n18=500000000000
n19=1000000000000

numElementos = [n1,n2,n3]

print "%-*s %-*s %-*s %-*s %-*s %s\n" % (15,"numElementos",15,"t_seq(seg)",15,"t_mpi(seg)",15,"speedup_mpi",20,"t_openmp+mpi(seg)","speedup_total"),
print "--------------------------------------------------------------------------------------------------"

for n in numElementos:
	p = subprocess.Popen(['./radix_seq',str(n)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	t_seq = p.communicate()[0]

	p = subprocess.Popen(['mpirun','-n','2','./radix_mpi',str(n)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	t_mpi = p.communicate()[0]

	t_speed = (float(t_seq) / float(t_mpi))
	"""
	p = subprocess.Popen(['mpirun','-n','2','./radix_mpi_openmp',str(n)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	t_mpi_openmp = p.communicate()[0]

	t_speed_total = (float(t_seq) / float(t_mpi_openmp))
	"""
	t_mpi_openmp = 0
	t_speed_total = 0
	print "%-*d %-*.4f %-*.4f %-*.2f %-*.4f %.2f\n" % (15,n,15,float(t_seq),15,float(t_mpi),15,float(t_speed),20,float(t_mpi_openmp),float(t_speed_total)),