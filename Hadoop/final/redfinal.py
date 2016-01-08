#!/usr/bin/python

import sys
cur = ''
cnt = 0
for x in sys.stdin.readlines():
#for i in range(1,22): 
#    x = raw_input()
    splt = x.split('\t')
    if (len(splt) > 1 and len(splt[0]) > 0): 
        if (cur == ''):
            cur = splt[0]
        if (cur == splt[0]):
            cnt += int(splt[1])
        else:
            print '%s,%d' % (cur, cnt)
            cur = splt[0]
            cnt = int(splt[1])
if (cur != ''):
    print '%s,%d' % (cur, cnt) 
