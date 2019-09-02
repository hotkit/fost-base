#!/usr/bin/python
## Copyright 2008-2019 Red Anchor Trading Co. Ltd.
##
## Distributed under the Boost Software License, Version 1.0.
## See <http://www.boost.org/LICENSE_1_0.txt>

import itertools

def is_prime(v):
    if v < 4:
        return True
    elif v % 2 == 0:
        return False
    else:
        for i in range(3, v, 2):
            if not v % i:
                return False
        return True

def prime_generator():
    base = 2
    while True:
        if is_prime(base):
            yield base
        base += 1

generator = prime_generator()
for i in itertools.islice(generator, 453):
    print i,
print
