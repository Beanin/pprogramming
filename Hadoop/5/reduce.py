#!/usr/bin/python
import sys
cur = ''
Dict = {}
for x in sys.stdin.readlines():
    q = x.split('\t')[0]
    w = int(x.split('\t')[1])
    if cur == '':
        cur = q
    if cur == q:
        if w not in Dict:
            Dict[w] = 1
        else:
            Dict[w] = Dict[w] + 1
    else:
        res = ''
        if len(Dict) == 1:
            for i in Dict:
                res += str(i) + ',' + str(Dict[i])
        else:
            count = 0
            for i in Dict:
                res = res + str(i) + ','
                count += 1  
            res = res + str(count)                    
        print '%s\t%s' % (cur, res)
        cur = q
        Dict = {w : 1}
if len(Dict) > 0:
    res = ''
    if len(Dict) == 1:
        for i in Dict:
            res = str(i) + ',' + str(Dict[i])     
    else:
        count = 0
        for i in Dict:
            res = res + str(i) + ','
            count += 1
        res = res + str(count)
    print '%s\t%s' % (cur, res)
    Dict = {}

 
         
