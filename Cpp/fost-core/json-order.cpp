/*
    Copyright 2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/json-order.hpp>


namespace {
    struct compare_bool : public boost::static_visitor<bool> {
        const bool left;
        compare_bool(bool l)
        : left(l) {
        }
        bool operator () (bool right) const {
            return left < right;
        }
        template <typename O>
        bool operator () (const O &o) const {
            return true;
        }
    };
    struct compare_int : public boost::static_visitor<bool> {
        const int64_t left;
        compare_int(int64_t l)
        : left(l) {
        }
        bool operator () (bool right) const {
            return false;
        }
        bool operator () (int64_t right) const {
            return left < right;
        }
        bool operator () (double right) const {
            return double(left) < right;
        }
        template <typename O>
        bool operator () (const O &o) const {
            return true;
        }
    };
    struct compare_double : public boost::static_visitor<bool> {
        const double left;
        compare_double(double l)
        : left(l) {
        }
        bool operator () (bool right) const {
            return false;
        }
        bool operator () (int64_t right) const {
            return left < double(right);
        }
        bool operator () (double right) const {
            return left < right;
        }
        template <typename O>
        bool operator () (const O &o) const {
            return true;
        }
    };
    struct compare_string : public boost::static_visitor<bool> {
        const fostlib::string &left;
        compare_string(const fostlib::string &l)
        : left(l) {
        }
        template <typename O>
        bool operator () (const O &o) const {
            return false;
        }
    };

    struct compare_variant_right : public boost::static_visitor<bool> {
        const fostlib::variant &right;
        compare_variant_right(const fostlib::variant &r)
        : right(r) {
        }
        bool operator () (bool left) const {
            return boost::apply_visitor(::compare_bool(left), right);
        }
        bool operator () (int64_t left) const {
            return boost::apply_visitor(::compare_int(left), right);
        }
        bool operator () (double left) const {
            return boost::apply_visitor(::compare_double(left), right);
        }
        bool operator () (const fostlib::string &left) const {
            return boost::apply_visitor(::compare_string(left), right);
        }
        template <typename O>
        bool operator () (const O &o) const {
            throw fostlib::exceptions::not_implemented("compare_variant_right", typeid(O).name());
        }
    };
    struct compare_variant_left : public boost::static_visitor<bool> {
        const fostlib::variant &left;
        compare_variant_left(const fostlib::variant &l)
        : left(l) {
        }
        bool operator () (const fostlib::variant &right) const {
            return boost::apply_visitor(::compare_variant_right(right), left);
        }
        template <typename O>
        bool operator () (const O &o) const {
            throw fostlib::exceptions::not_implemented("compare_variant_left", typeid(O).name());
        }
    };

    struct compare_json : public boost::static_visitor<bool> {
        const fostlib::json &right;
        compare_json(const fostlib::json &r)
        : right(r) {
        }

        bool operator () (const fostlib::variant &left) const {
            return boost::apply_visitor(::compare_variant_left(left), right);
        }
        template <typename O>
        bool operator () (const O &o) const {
            throw fostlib::exceptions::not_implemented("compare_json", typeid(O).name());
        }
    };
}


bool std::less<fostlib::json>::operator () (const fostlib::json& lhs, const fostlib::json& rhs) const {
    if ( lhs.isnull() ) {
        return not rhs.isnull();
    } else if ( rhs.isnull() ) {
        return false;
    } else {
        return boost::apply_visitor(::compare_json(rhs), lhs);
    }
}

