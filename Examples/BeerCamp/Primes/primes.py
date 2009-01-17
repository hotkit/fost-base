#!/usr/bin/python
#
#    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
#    Distributed under the Boost Software License, Version 1.0.
#    See accompanying file LICENSE_1_0.txt or copy at
#        http://www.boost.org/LICENSE_1_0.txt
#

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
    base = [2]
    def next_prime():
        while not is_prime(base[0]):
            base[0] += 1
        base[0] += 1
        return base[0] - 1
    return next_prime

generator = prime_generator()
for i in range(0, 453):
    print generator(),
print
