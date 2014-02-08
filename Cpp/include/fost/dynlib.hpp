/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_DYNLIB_HPP
#define FOST_DYNLIB_HPP


namespace fostlib {


    class FOST_CORE_DECLSPEC dynlib : boost::noncopyable {
        struct impl;
        impl *m_lib;
    public:
        dynlib( const string &path );
        ~dynlib();
    };


}


#endif // FOST_DYNLIB_HPP
