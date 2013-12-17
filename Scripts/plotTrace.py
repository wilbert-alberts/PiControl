#!/usr/bin/python

import subprocess

import string


output = subprocess.check_output (["ssh", "root@rb", "/home/pi/PiControl/Terminal/ARM-Debug/Terminal dumpTraces"])

lines = string.split(output,'\n')

traceOut = open('/tmp/trace.txt','w')
traceOut.write(output)
traceOut.close()


fields=string.split(string.replace(lines[0], "# SampleCounter", ""))


idx=2
datasets=[]
for f in fields:
	datasets.append('"/tmp/trace.txt" using 1:'+str(idx)+' with lines title "' + f + '" ')
	idx=idx+1

cmd="plot " + string.join(datasets, ",")

outfile = open('/tmp/genplot.p','w')
outfile.write(cmd+'\n')
outfile.close()


subprocess.call(["gnuplot", "-persist", "/tmp/genplot.p"])

