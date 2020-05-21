/**
    Copyright 2002-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <locale>
#include <fost/unicode.hpp>


string fostlib::coercer<string, int64_t>::coerce(int64_t t) {
    stringstream ss;
    ss << t;
    return string(ss.str());
}
string fostlib::coercer<string, uint64_t>::coerce(uint64_t t) {
    stringstream ss;
    ss << t;
    return string(ss.str());
}
string fostlib::coercer<string, double>::coerce(double f) {
    stringstream ss;
    ss.imbue(std::locale("C"));
    ss.precision(std::numeric_limits<double>::digits10 + 2);
    ss << f;
    return string(ss.str());
}
