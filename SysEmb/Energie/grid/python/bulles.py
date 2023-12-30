import random

def triBulles(L):
    for i in range(len(L),0,-1):
        for j in range(0,i-1):
            if L[j+1] < L[j]:
                L[j+1] , L[j] = L[j] , L[j+1]
    return L

L=[12,82,42,71,238,72,1,465,23,53]

for i in range(10000):
	L.append(random.randint(0,999))

triBulles(L)
