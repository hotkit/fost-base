/**
    Copyright 2008-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core.hpp"
#include <fost/dynlib.hpp>
#include <fost/insert.hpp>


#ifdef WIN32


using namespace fostlib;


struct fostlib::dynlib::impl {};


fostlib::dynlib::dynlib(const string &pathname) : m_lib(NULL) {
    string munged = pathname;
#ifdef _DEBUG
    munged += "-gd";
#endif
    if (::LoadLibrary(munged.c_str()) == NULL)
        if (::LoadLibrary(pathname.c_str()) == NULL)
            throw exceptions::null(
                    "LoadLibrary failed for " + pathname, format_last_error());
}


fostlib::dynlib::~dynlib() {}


#else


#include <dlfcn.h>
#include <fost/atexit.hpp>


using namespace fostlib;


struct fostlib::dynlib::impl {
    impl(void *h) : handle(h) {}
    /**
        We don't want to actually unload the .so yet as there may well still be
        objects that it manages.

        By putting the deletion in the `atexit` handler we could ensure that
        the libraries are unloaded in the right order (that of the destructors).
        But it turns out that `dlclose` may sometimes still assert and cause
        the program to shutdown uncleanly. As a consequence we just leak
        the handle :-(

        ```cpp
        ~impl() {
            if (handle) dlclose(handle);
        }
        ```
     */
    string name;
    void *handle;
};


fostlib::dynlib::dynlib(const string &lib) {
    fostlib::json attempts;
    const auto tryload = [&attempts](string dl) {
        /**
            On Linux (but not POSIX), using `RTLD_NODELETE` may seem like
            a good idea here, but it turns out that things don't work at
            all well with that. `dlclose` may still assert on exit :-(
         */
        void *h = dlopen(dl.shrink_to_fit(), RTLD_NOW);
        if (not h)
            fostlib::insert(
                    attempts, std::move(dl), fostlib::string(dlerror()));
        return h;
    };
    void *handle = tryload(lib);
#if defined(__APPLE__)
    f5::lstring ext = ".dylib";
#else
    f5::lstring ext = ".so";
#endif
    auto next = [tryload, &handle](string dl) {
        if (handle == nullptr) handle = tryload(std::move(dl));
    };
    next("lib" + lib + ext);
    next("lib" + lib + "-d" + ext);
    next(lib + ext);
    next("lib" + lib);
    if (handle == nullptr) {
        fostlib::exceptions::null err("dlopen failed");
        fostlib::insert(err.data(), "attempted", attempts);
        throw err;
    }
    m_lib = std::make_unique<fostlib::dynlib::impl>(handle);
    m_lib->name = lib;
}


fostlib::dynlib::~dynlib() = default;


#endif
