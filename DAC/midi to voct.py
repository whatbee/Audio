import sys
if sys.version_info[0] < 3:
    from __future__ import division # why did python 3 change the way division works???
import math
maxbits = (2**12) # bit depth of DAC
tablesize = 128   # number of notes in table
semitones = 12    # number of semitones per octave

f= open("voct table.txt","w")
f.write("const uint16_t voctTable [] = {")
for i in range(tablesize):
	x= math.trunc(round((((i/semitones)*maxbits)/((tablesize-1)/semitones))))	
	f.write("%s" % x)
	if i<(tablesize-1):
		f.write(",")
	i+=1
f.write("}")
f.close()