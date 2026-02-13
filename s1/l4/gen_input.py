#!/usr/bin/env python3

import random

def round2(x, n):
    return round(x * (10**n))/(10 ** n)

random.seed(5692)

nums = []
for _ in range(65):
    if random.random() > 0.5:
        nums += [round2(1 - 2*random.random(), 2)]
    else:
        nums += [random.choice(nums)]

print(*nums, file=open("input_gen.txt", "w"))
