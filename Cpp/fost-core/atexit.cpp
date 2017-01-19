/*
    Copyright 2009-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/atexit.hpp>


using namespace fostlib;


namespace {
    typedef std::vector<std::function<void(void)>> atexit_collection;
    atexit_collection &g_atexit_collection();


    struct atexit_installer {
        static void collection_executor() {
            for ( auto &&f : g_atexit_collection() ) f();
        }
        atexit_installer() {
            std::atexit(collection_executor);
        }
    };


    atexit_collection &g_atexit_collection() {
        static atexit_collection collection;
        static atexit_installer installer;
        return collection;
    }


}


void fostlib::atexit(std::function<void(void)> function) {
    g_atexit_collection().push_back(function);
}

