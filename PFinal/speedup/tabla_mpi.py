import sys
import subprocess

sys.stdout = open('output2', 'w')


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
"""
n13=500000000

n14=1000000000
n14=5000000000
n15=10000000000
n16=50000000000
n17=100000000000
n18=500000000000
n19=1000000000000
"""
numElementos = [n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12]

"""
print "%-*s %-*s %-*s %s\n" % (15,"numElementos",15,"t_seq(seg)",15,"t_mpi(seg)","speedup_mpi"),
print "-----------------------------------------------------------"
"""
for n in numElementos:
	p = subprocess.Popen(['./radix_seq',str(n)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	t_seq = (p.communicate()[0]).strip()
	"""
	print "str>   t_seq = %s" % (t_seq)
	print "float> t_seq = %f" % float(t_seq)
	"""
	p = subprocess.Popen(['mpirun','-n','2','./radix_mpi',str(n)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	t_mpi = (p.communicate()[0]).strip()
	"""
	print "str>   t_mpi = %s" % (t_mpi)
	print "float> t_mpi = %f" % float(t_mpi)
	"""
	t_speed = "%.4f" % (float(t_seq) / float(t_mpi))

	print "%-*d %-*.4f %-*.4f %.2f\n" % (15,n,15,float(t_seq),15,float(t_mpi),float(t_speed)),