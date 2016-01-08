#!/usr/bin/python

import sys
char = ''
wcount = 0
rcount = 0
for x in sys.stdin.readlines():
    c = x.split('\t')[0]
    rest = x.split('\t')[1]
    wc = int(rest.split(',')[0])
    rc = int(rest.split(',')[1])
    if (char == ''):
        char = c
    if (char == c):
        wcount += wc
        rcount += rc
    else:
        print ('%s\t%d,%d,%f') % (char, wcount, rcount, 1.0*wcount/rcount)
        wcount = wc
        rcount = rc
        char = c   
if char!='':
     print ('%s\t%d,%d,%f') % (char, wcount, rcount, 1.0*wcount/rcount)   
