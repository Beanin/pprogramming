#!/usr/bin/python

import sys
import os

for x in sys.stdin.readlines():
    a = x.split(',')[0]
    b = x.split(',')[1]
    c = x.split(',')[2]
    splt = os.environ["mapreduce_map_input_file"].split('/A')    
    if (len(splt) == 1):
        print '%s\t%s,%d,%s' % (a, b, 1, c)
    else:
        print '%s\t%s,%d,%s' % (b, a, 0, c)
  
    
    
