#!/usr/bin/python

import sys

curkey = -1

q = [[],[]]

def handle(rest):
    a, b, c = int(rest.split(',')[0]), int(rest.split(',')[1]), int(rest.split(',')[2])
    q[b].append([a, c]) 
    return 

for x in sys.stdin.readlines():
    splt = x.split('\t')
    if (len(splt) > 1 and len(splt[0]) > 0):
        key = splt[0]
        rest = splt[1]
        if (curkey == -1):
            curkey = key
        if (curkey == key):
            handle(rest)
        else:
            for a in q[0]:
                for b in q[1]:
                    print '%d,%d\t%d' % (a[0],b[0],a[1]*b[1]) 
            q = [[],[]]
            curkey = key
            handle(rest)

for a in q[0]:
    for b in q[1]:
        print '%d,%d\t%d' % (a[0],b[0],a[1]*b[1])

    
