/*
    Copyright 1999-2014, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#if BOOST_VERSION_MAJOR < 42


    #include <uuid/uuid.h>


    fostlib::string fostlib::guid() {
        uuid_t guid;
        uuid_generate( guid );
        char guid_str[ 37 ];
        uuid_unparse( guid, guid_str );
        return string( guid_str );
    }


#else


    #include <boost/uuid/uuid_generators.hpp>
    #include <boost/uuid/uuid_io.hpp>


    fostlib::string fostlib::guid() {
        boost::uuids::uuid guid = boost::uuids::random_generator()();
        std::stringstream ss;
        ss << guid;
        return fostlib::string(ss.str());
    }


#endif
