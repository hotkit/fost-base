/*
    Copyright 1999-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_SCHEMA_STATIC_HPP
#define FOST_SCHEMA_STATIC_HPP
#pragma once


#include <fost/schema/dynamic.hpp>


namespace fostlib {


    class FOST_SCHEMA_DECLSPEC model_base {
    public:
        model_base();
        virtual ~model_base();
    };


    template< typename model_type, typename superclass_type = t_null >
    class model : public model_base {
    public:
        model() : model_base() {}

        static const struct factory {
            boost::shared_ptr< model_type > create() const {
                return boost::shared_ptr< model_type >( new model_type );
            }
        } s_factory;

        template< typename field_type >
        class attribute {
        public:
        };
    };


}


#endif // FOST_SCHEMA_STATIC_HPP
