/**
    Copyright 2008-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <fost/cli>
#include <fost/main.hpp>


FSL_MAIN("Hello", "Hello world!")(fostlib::ostream &, fostlib::arguments &) {
    return 0;
}
