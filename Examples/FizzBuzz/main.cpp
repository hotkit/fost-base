/**
    Copyright 2008-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <fost/cli>
#include <fost/main>


namespace {
    // Set the default count to be 100
    const fostlib::setting<int>
            c_count(L"FizzBuzz main.cpp", L"FizzBuzz", L"Count", 100, true);

    void fizzbuzz(fostlib::ostream &out);
}


FSL_MAIN(L"FizzBuzz", L"FizzBuzz\nCopyright (c) 2008-2015 Felspar Co. Ltd.")
(fostlib::ostream &out, fostlib::arguments &args) {
    // Where we can store any numeric setting we're given
    std::unique_ptr<fostlib::setting<int>> count;
    if (args.size() == 2)
        count.reset(new fostlib::setting<int>(
                L"Command line", c_count, fostlib::coerce<int>(args[1])));
    fizzbuzz(out);
    return 0;
}


namespace {
    void fizzbuzz(fostlib::ostream &out) {
        for (int i = 1; i <= c_count.value(); ++i) {
            fostlib::string o;
            if (i % 3 == 0) o += L"Fizz";
            if (i % 5 == 0) o += L"Buzz";
            out << (o.empty() ? fostlib::coerce<fostlib::string>(i) : o) << ' ';
        }
        out << std::endl;
    }
}
