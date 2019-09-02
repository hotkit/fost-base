/**
    Copyright 2009-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


// A simple prime number test which is fairly efficient
bool is_prime(unsigned int v) {
    if (v % 2 == 0)
        return v == 2;
    else {
        for (unsigned int i = 3; i < v; i += 2)
            if (v % i == 0) return false;
        return true;
    }
}
