/**
    Copyright 2007-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core.hpp"
#include <fost/json.hpp>
#include <fost/insert.hpp>
#include <fost/unicode.hpp>
#include <fost/detail/coerce.hpp>
#include <fost/detail/utility.hpp>
#include <fost/parse/jcursor.hpp>

#include <fost/exception/json_error.hpp>
#include <fost/exception/not_implemented.hpp>
#include <fost/exception/not_null.hpp>
#include <fost/exception/null.hpp>
#include <fost/exception/out_of_range.hpp>
#include <fost/exception/parse_error.hpp>


/**
 * ## fostlib
 */


bool fostlib::operator==(
        const fostlib::jcursor::value_type &lhs, f5::u8view rhs) {
    auto lhs_sp = std::get_if<fostlib::string>(&lhs);
    if (lhs_sp) {
        return *lhs_sp == rhs;
    } else {
        return false;
    }
}


/**
 * ## fostlib::jcursor
 */


fostlib::jcursor::jcursor() {}
fostlib::jcursor::jcursor(int i) {
    m_position.push_back(fostlib::coerce<std::size_t>(i));
}
fostlib::jcursor::jcursor(json::array_t::size_type i) {
    m_position.push_back(i);
}
fostlib::jcursor::jcursor(nliteral n) {
    m_position.push_back(fostlib::string(n));
}
fostlib::jcursor::jcursor(wliteral n) {
    m_position.push_back(fostlib::string(n));
}
fostlib::jcursor::jcursor(f5::u8view s) { m_position.push_back(s); }
fostlib::jcursor::jcursor(fostlib::string &&s) {
    m_position.push_back(std::move(s));
}
fostlib::jcursor::jcursor(const json &j) {
    nullable<int64_t> i = j.get<int64_t>();
    if (i) {
        m_position.push_back(coerce<json::array_t::size_type>(i.value()));
    } else {
        auto s = j.get<f5::u8view>();
        if (s) {
            m_position.push_back(s.value());
        } else {
            throw exceptions::json_error(
                    "The jcursor location must be a string or integer", j);
        }
    }
}

fostlib::jcursor::jcursor(stack_t::const_iterator b, stack_t::const_iterator e)
: m_position(b, e) {}


fostlib::jcursor fostlib::jcursor::split(const string &s, const string &c) {
    fostlib::split_type path = fostlib::split(s, c);
    fostlib::jcursor position;
    for (fostlib::split_type::const_iterator part(path.begin());
         part != path.end(); ++part) {
        try {
            int index = fostlib::coerce<int>(*part);
            position /= index;
        } catch (fostlib::exceptions::parse_error &) { position /= *part; }
    }
    return position;
}


fostlib::jcursor &fostlib::jcursor::operator/=(json::array_t::size_type i) {
    m_position.push_back(i);
    return *this;
}
fostlib::jcursor &fostlib::jcursor::operator/=(f5::u8view i) {
    m_position.push_back(i);
    return *this;
}
fostlib::jcursor &fostlib::jcursor::operator/=(const json &j) {
    return operator/=(jcursor(j));
}
fostlib::jcursor &fostlib::jcursor::operator/=(const jcursor &r) {
    m_position.insert(
            m_position.end(), r.m_position.begin(), r.m_position.end());
    return *this;
}

fostlib::jcursor &fostlib::jcursor::enter() {
    m_position.push_back(0u);
    return *this;
}
fostlib::jcursor &fostlib::jcursor::enter(const string &i) {
    m_position.push_back(i);
    return *this;
}
fostlib::jcursor &fostlib::jcursor::pop() {
    m_position.pop_back();
    return *this;
}

fostlib::jcursor &fostlib::jcursor::operator++() {
    if (m_position.empty())
        throw fostlib::exceptions::null{"cannot increment an empty jcursor"};
    else if (std::get_if<json::array_t::size_type>(&*m_position.rbegin()) == NULL)
        throw fostlib::exceptions::null{
                "The current jcursor isn't into an array position"};
    else
        ++std::get<json::array_t::size_type>(*m_position.rbegin());
    return *this;
}


namespace {
    struct take_step {
        const fostlib::json &orig;
        fostlib::json::element_t &element;
        bool isnull;

        take_step(const fostlib::json &o, fostlib::json::element_t &j, bool n)
        : orig(o), element(j), isnull(n) {}

        fostlib::json *operator()(fostlib::json::array_t::size_type k) const {
            if (isnull)
                element = std::make_shared<fostlib::json::array_t>();
            else if (!std::get_if<fostlib::json::array_p>(&element)) {
                throw fostlib::exceptions::json_error(
                        "Cannot walk through a JSON "
                        "object/value which is not an array using an integer "
                        "key",
                        orig);
            }
            // Copy the array
            fostlib::json::array_t &array =
                    *std::get<fostlib::json::array_p>(element);
            auto copy(std::make_shared<fostlib::json::array_t>(
                    array.begin(), array.end()));
            while (copy->size() <= k) copy->push_back(fostlib::json{});
            element = copy;
            return &(*copy)[k];
        }
        fostlib::json *operator()(const fostlib::string &k) const {
            if (isnull)
                element = std::make_shared<fostlib::json::object_t>();
            else if (!std::get_if<fostlib::json::object_p>(&element)) {
                throw fostlib::exceptions::json_error(
                        "Cannot walk through a JSON "
                        "array/value which is not an object using a string key",
                        orig);
            }
            /// Copy the object and return the address of the value at the
            /// requested key
            fostlib::json::object_t &object =
                    *std::get<fostlib::json::object_p>(element);
            auto copy(std::make_shared<fostlib::json::object_t>(
                    object.begin(), object.end()));
            element = copy;
            return &(*copy)[k];
        }
    };
}
fostlib::json &fostlib::jcursor::operator()(json &j) const {
    try {
        json *loc = &j;
        for (stack_t::const_iterator p(m_position.begin());
             p != m_position.end(); ++p) {
            loc = std::visit(take_step(j, loc->m_element, loc->isnull()), *p);
        }
        return *loc;
    } catch (exceptions::exception &e) {
        fostlib::insert(e.data(), "jcursor", *this);
        fostlib::insert(e.data(), "traversing", j);
        throw;
    }
}


fostlib::json &fostlib::jcursor::push_back(json &j, json &&v) const {
    json &array = (*this)(j);
    if (array.isnull()) {
        json::array_t na;
        na.push_back(std::move(v));
        array = na;
    } else if (array.isarray()) {
        std::get<json::array_p>(array.m_element)->push_back(v);
    } else
        throw exceptions::json_error(
                "Can only push onto the back of a JSON array");
    return j;
}


fostlib::json &fostlib::jcursor::insert(json &j, json &&v) const {
    if (!j.has_key(*this)) {
        (*this)(j) = std::move(v);
    } else {
        exceptions::not_null error(
                "There is already some JSON at this key position");
        fostlib::insert(error.data(), "json", j);
        fostlib::insert(error.data(), "value", v);
        fostlib::insert(error.data(), "key", *this);
        throw error;
    }
    return j;
}


fostlib::json &fostlib::jcursor::replace(json &j, json &&v) const {
    if (j.has_key(*this))
        (*this)(j) = std::move(v);
    else
        throw exceptions::null(
                "There is nothing to replace at this key position",
                json::unparse(j, true) + "\n" + json::unparse(v, true));
    return j;
}


fostlib::json &fostlib::jcursor::set(json &j, json &&v) const {
    (*this)(j) = std::move(v);
    return j;
}


namespace {
    struct del_key {
        fostlib::json::element_t &element;
        del_key(fostlib::json::element_t &j) : element(j) {}

        void operator()(fostlib::json::array_t::size_type k) const {
            fostlib::json::array_p *arr(
                    std::get_if<fostlib::json::array_p>(&element));
            if (!arr)
                throw fostlib::exceptions::json_error(
                        "A numeric key can only be used to delete from a JSON "
                        "array");
            if (k >= (*arr)->size())
                throw fostlib::exceptions::out_of_range<std::size_t>(
                        "Trying to erase beyond the end of the array", 0,
                        (*arr)->size(), k);
            (*arr)->erase((*arr)->begin() + k);
        }
        void operator()(const fostlib::string &k) const {
            fostlib::json::object_p *obj(
                    std::get_if<fostlib::json::object_p>(&element));
            if (not obj)
                throw fostlib::exceptions::json_error(
                        "A string key can only be deleted from JSON objects");
            fostlib::json::object_t::iterator p((*obj)->find(k));
            if (p == (*obj)->end())
                throw fostlib::exceptions::json_error(
                        "Key can't be removed from object as it doesn't exist "
                        "in the object",
                        **obj);
            (*obj)->erase(p);
        }
    };
}
fostlib::json &fostlib::jcursor::del_key(json &j) const {
    try {
        if (m_position.size() == 0) {
            throw exceptions::out_of_range<std::size_t>(
                    "The jcursor must have at least one level of items in it",
                    1, std::numeric_limits<std::size_t>::max(),
                    m_position.size());
        } else if (j.has_key(*this)) {
            (*this)(j);
            jcursor head(begin(), --end());
            auto &from = const_cast<json::element_t &>(j[head].m_element);
            std::visit(::del_key(from), *m_position.rbegin());
        } else {
            throw exceptions::json_error(
                    "The key cannot be deleted because it doesn't exist");
        }
        return j;
    } catch (exceptions::exception &e) {
        fostlib::insert(e.data(), "json", j);
        fostlib::insert(e.data(), "path", *this);
        throw;
    }
}


bool fostlib::jcursor::operator==(const jcursor &j) const {
    return m_position == j.m_position;
}


/**
 * ## JSON pointer
 */


fostlib::jcursor fostlib::jcursor::parse_json_pointer_string(f5::u8view s) {
    jcursor ret;
    auto pos = f5::cord::make_u32u16_iterator(s.begin(), s.end());
    const json_pointer_parser<
            f5::cord::const_u32u16_iterator<f5::u8view::const_iterator>>
            parser;
    if (boost::spirit::qi::parse(pos.first, pos.second, parser, ret)
        && pos.first == pos.second) {
        return ret;
    } else {
        throw exceptions::parse_error(
                "Whilst parsing JSON pointer string",
                string(pos.first.u32_iterator(), pos.second.u32_iterator()));
    }
}
fostlib::jcursor fostlib::jcursor::parse_json_pointer_fragment(f5::u8view s) {
    jcursor ret;
    auto *pos = s.data(), *end = s.data() + s.bytes();
    const json_pointer_fragment_parser<decltype(pos)> parser;
    if (boost::spirit::qi::parse(pos, end, parser, ret) && pos == end) {
        return ret;
    } else {
        throw exceptions::parse_error(
                "Whilst parsing JSON pointer fragment",
                f5::u8view{pos, std::size_t(end - pos)});
    }
}


fostlib::ascii_printable_string fostlib::jcursor::as_json_pointer() const {
    struct proc {
        std::string pointer;

        char digit(utf8 dig) {
            if (dig < 0x0a) return dig + '0';
            if (dig < 0x10) return dig + 'A' - 0x0a;
            throw fostlib::exceptions::out_of_range<int>(
                    "Number to convert to hex digit is too big", 0, 0x10, dig);
        }

        void hex(utf8 ch) {
            pointer += '%';
            pointer += digit((ch & 0xf0) >> 4);
            pointer += digit(ch & 0x0f);
        }

        void operator()(const string &s) {
            pointer += '/';
            for (const auto c : s) {
                if (c == '~') {
                    pointer += "~0";
                } else if (c == '/') {
                    pointer += "~1";
                } else if (c == 0x25) {
                    pointer += "%25";
                } else if (c < 0x7f) { // 7 bit safe
                    pointer += c;
                } else {
                    auto [bytes, chars] = f5::cord::u8encode(c);
                    for (char b{}; b < bytes; ++b) { hex(chars[b]); }
                }
            }
        }
        void operator()(std::size_t s) {
            pointer += '/';
            pointer += std::to_string(s);
        }
    } visitor;
    for (const auto &p : m_position) { std::visit(visitor, p); }
    return fostlib::string{std::move(visitor.pointer)};
}
