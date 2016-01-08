#!/usr/bin/python

import sys

cur = ''
cnt = 0
for x in sys.stdin.readlines():
    if (len(x.split('\t')) > 1):
        tid = x.split('\t')[0]
        cur = tid
        text = x.split('\t')[1]
    else:
        tid = cur
        text = x
    for i in text.split():
        q =''
        for c in i:
            if str.isalpha(c):
                q = q + c
        if q != '':
            print '%s\t%s' % (str.lower(q), tid)  
