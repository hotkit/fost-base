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

        template< typename model_type >
        struct factory {
            boost::shared_ptr< model_type > create() const {
                return boost::shared_ptr< model_type >( new model_type );
            }
        };
    };


    /*
        This handles the case for model types that inherit from other model type
    */
    template< typename model_type, typename superclass_type = t_null >
    class model : public superclass_type {
    public:
        model() : superclass_type() {}

        typedef model_base::factory< model_type > factory;
        static const factory s_factory;
    };


    /*
        A specialisation that handles the case when a model type does not
        inherit from another model.
    */
    template< typename model_type >
    class model< model_type, t_null > : public model_base {
    public:
        model() : model_base() {}

        template< typename field_type >
        class attribute {
        public:
        };

        typedef model_base::factory< model_type > factory;
        static const factory s_factory;
    };


}


#endif // FOST_SCHEMA_STATIC_HPP
