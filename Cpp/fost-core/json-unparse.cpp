/**
    Copyright 2007-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core.hpp"
#include <fost/json.hpp>
#include <fost/unicode.hpp>


using namespace fostlib;


namespace {


    const setting<int> c_tabwidth(
            "fost-base/json-unparse.cpp", "JSON", "Unparse tab width", 4, true);


    inline void tab(std::string &into, std::size_t tabs) {
        const std::size_t chars = coerce<std::size_t>(c_tabwidth.value());
        for (std::size_t t{0}; t != tabs * chars; ++t) into += ' ';
    }
    inline void to_hex(std::string &into, utf16 c, std::size_t digits = 3) {
        wchar_t l = (c & 0xf) + L'0';
        if (l > L'9') l = l - L'0' - 10 + L'a';
        if (digits) to_hex(into, c >> 4, digits - 1);
        into += l;
    }
    inline void string_to_json(std::string &into, f5::u8view sv) {
        f5::cord::const_u8buffer s{sv};
        into += '"';
        for (unsigned char i : s) {
            if (i < 0x20) {
                switch (i) {
                case L'\n': into += "\\n"; break;
                case L'\r': into += "\\r"; break;
                case L'\t': into += "\\t"; break;
                default:
                    into += "\\u";
                    to_hex(into, i);
                    break;
                }
            } else if (i == '\\') {
                into += "\\\\";
            } else if (i == '\"') {
                into += "\\\"";
            } else {
                into += i;
            }
        }
        into += '\"';
    }


    struct to_json {
        std::string &into;

        to_json(std::string &i) : into(i) {}

        void operator()(std::monostate) const { into += "null"; }
        void operator()(bool b) const {
            if (b) {
                into += "true";
            } else {
                into += "false";
            }
        }
        void operator()(int64_t v) const { into += std::to_string(v); }
        void operator()(double d) const {
            into += static_cast<std::string_view>(
                    fostlib::coerce<fostlib::string>(d));
        }
        void operator()(f5::lstring s) const { string_to_json(into, s); }
        void operator()(const json::string_t &s) const {
            string_to_json(into, s);
        }
        void operator()(const json::array_p &t) const {
            into += '[';
            for (json::array_t::const_iterator i(t->begin()); i != t->end();
                 ++i) {
                into += (i == t->begin() ? "" : ",");
                json::unparse(into, *i, false);
            }
            into += ']';
        }
        void operator()(const json::object_p &t) const {
            into += '{';
            for (json::object_t::const_iterator i(t->begin()); i != t->end();
                 ++i) {
                into += (i == t->begin() ? "" : ",");
                string_to_json(into, i->first);
                into += ':';
                json::unparse(into, i->second, false);
            }
            into += '}';
        }
    };


    struct to_pretty : public to_json {
        mutable std::size_t indentation;

        to_pretty(std::string &i) : to_json(i), indentation(0) {}

        using to_json::operator();

        void operator()(const json::array_p &t) const {
            if (t->size() == 0) {
                into += "[]";
            } else if (t->size() == 1) {
                into += '[';
                (*t)[0].apply_visitor(*this);
                into += ']';
            } else {
                into += '[';
                ++indentation;
                for (json::array_t::const_iterator i(t->begin()); i != t->end();
                     ++i) {
                    into += (i == t->begin() ? "\n" : ",\n");
                    tab(into, indentation);
                    i->apply_visitor(*this);
                }
                --indentation;
                into += '\n';
                tab(into, indentation);
                into += ']';
            }
        }
        void operator()(const json::object_p &t) const {
            if (t->size() == 0) {
                into += "{}";
            } else {
                into += '{';
                ++indentation;
                for (json::object_t::const_iterator i(t->begin());
                     i != t->end(); ++i) {
                    into += (i == t->begin() ? "\n" : ",\n");
                    tab(into, indentation);
                    string_to_json(into, i->first);
                    into += " : ";
                    i->second.apply_visitor(*this);
                }
                --indentation;
                into += '\n';
                tab(into, indentation);
                into += '}';
            }
        }
    };


}


void fostlib::json::unparse(std::string &into, const json &json, bool pretty) {
    if (pretty) {
        json.apply_visitor(::to_pretty(into));
        into += '\n';
    } else {
        json.apply_visitor(::to_json(into));
    }
}


void fostlib::json::unparse(std::string &into, const string &str) {
    string_to_json(into, str);
}
