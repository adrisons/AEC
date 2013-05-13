import sys
import subprocess
from subprocess import call

sys.stdout = open('output_openmp', 'w')

n0=10
n1=500
n2=1000
n3=5000
n4=10000
n5=50000
n6=200000
n7=500000
n8=1000000
n9=5000000
n10=10000000
n11=50000000
n12=100000000

numElementos = [n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12]

print "%-*s %-*s %-*s %s\n" % (15,"numElementos",15,"t_seq(seg)",15,"t_openmp(seg)","speedup"),
print "--------------------------------------------------------------------"

for n in numElementos:
	p = subprocess.Popen(['./radix_seq',str(n)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

	t_seq = float(p.communicate()[0])
	"""
	p = subprocess.Popen(['mpirun','-n','2','./radix_mpi',str(n)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	t_mpi = float(p.communicate()[0])
	
	t_speed = (t_seq / t_mpi)
	"""
	p = subprocess.Popen(['./radix_openmp',str(n)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

	t_openmp = float(p.communicate()[0])

	t_speed = (t_seq / t_openmp)
	
	print "%-*d %-*.4f %-*.4f %-*.2f\n" % (15,n,15,t_seq,15,t_openmp,15,t_speed),