#!/usr/bin/env python

import random
import subprocess
import pareto

N=1000
K=4

cmd=""
raw_vectors=[]
for k in range(K):
    with open("/tmp/pareto.%d"%k,"w") as f:
        cmd+="-l /tmp/pareto.%d "%k
        vec=[]
        for n in range(N):
            v=random.random()
            f.write("%f\n"%v)
            vec.append(v)
    raw_vectors.append(vec)

subprocess.call("./pareto -a nondominatedsort %s > /tmp/pareto.result.c"%cmd,shell=True)

c_resultfile=open("/tmp/pareto.result.c","r")
c_results=[int(v.strip()) for v in c_resultfile.readlines()]

dataset=pareto.create_dataset(raw_vectors)
pareto.nondominated_sort(dataset)
for n,s in enumerate(dataset):
    if s.paretoStatus != c_results[n]:
        print "Discrepancy!"
        print n,s, s.paretoStatus, c_results[n]
print "Test Done"

subprocess.call("rm /tmp/pareto.*",shell=True)
