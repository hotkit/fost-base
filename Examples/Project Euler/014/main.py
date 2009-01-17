#
#    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
#    Distributed under the Boost Software License, Version 1.0.
#    See accompanying file LICENSE_1_0.txt or copy at
#        http://www.boost.org/LICENSE_1_0.txt
#

def next(n):
    if n%2:
        return 3 * n + 1
    else:
        return n / 2

def chain_length(n):
    if n == 1:
        return 1
    else:
        return 1 + chain_length(next(n))

value, longest = 0, 0
for i in range(250000):
    v = i * 2 + 500001
    length = chain_length(v)
    if length > longest:
        value, longest = v, length
        print value, " has length ", length
