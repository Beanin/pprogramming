#!/usr/bin/python

import sys

cnt = 0
Dict = {}
for x in sys.stdin.readlines():
    char = x.split('\t')[0]
    text = x.split('\t')[1]
    for i in text.split():
        q =''
        for c in i:
            if str.isalpha(c):
                q = q + c
        if q != '':
            if q not in Dict:
                Dict[q] = 1
            else:
                Dict[q] += 1
    for word in Dict:
        print '%s\t%d,%d' % (word, Dict[word], 1)
    Dict = {}          
