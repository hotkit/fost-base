/**
    Copyright 2007-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core.hpp"
#include <fost/json.hpp>
#include <fost/detail/coerce.hpp>

#include <fost/exception/json_error.hpp>
#include <fost/exception/not_implemented.hpp>
#include <fost/exception/null.hpp>
#include <fost/exception/not_null.hpp>
#include <fost/exception/out_of_range.hpp>

#include <fost/insert.hpp>

#include <boost/version.hpp>


using namespace fostlib;


void fostlib::json::raise(f5::lstring msg) const {
    throw exceptions::json_error(msg, *this);
}


namespace {
    struct isnull {
        bool operator()(std::monostate) const { return true; }
        template<typename t>
        bool operator()(const t &) const {
            return false;
        }
    };
}
bool fostlib::json::isnull() const { return std::visit(::isnull(), m_element); }


namespace {
    struct isatom {
        bool operator()(std::monostate) const { return false; }
        template<typename t>
        bool operator()(const t &) const {
            return true;
        }
        bool operator()(const json::array_p &) const { return false; }
        bool operator()(const json::object_p &) const { return false; }
    };
}
bool fostlib::json::isatom() const { return std::visit(::isatom(), m_element); }


namespace {
    struct isarray {
        bool operator()(const json::array_p &) const { return true; }
        template<typename t>
        bool operator()(const t &) const {
            return false;
        }
    };
}
bool fostlib::json::isarray() const {
    return std::visit(::isarray(), m_element);
}


namespace {
    struct isobject {
        bool operator()(const json::object_p &) const { return true; }
        template<typename t>
        bool operator()(const t &) const {
            return false;
        }
    };
}
bool fostlib::json::isobject() const {
    return std::visit(::isobject(), m_element);
}


namespace {
    struct comparator {
        const json::element_t &r;
        comparator(const json::element_t &r) : r(r) {}

        template<typename T>
        bool operator()(const T &t) const {
            auto rvp = std::get_if<T>(&r);
            return rvp && t == *rvp;
        }
        bool operator()(json::string_t const &t) const {
            auto rvp = std::get_if<json::string_t>(&r);
            return rvp && t == *rvp;
        }
        bool operator()(const json::array_p &ta) const {
            if (!std::get_if<json::array_p>(&r))
                return false;
            else {
                const json::array_p &ra(std::get<json::array_p>(r));
                json::array_t::const_iterator pr(ra->begin()), pt(ta->begin());
                for (; pr != ra->end() && pt != ta->end(); ++pr, ++pt) {
                    if (*pr != *pt) return false;
                }
                return pr == ra->end() && pt == ta->end();
            }
        }
        bool operator()(const json::object_p &ta) const {
            if (!std::get_if<json::object_p>(&r))
                return false;
            else {
                const json::object_t &ra(*std::get<json::object_p>(r));
                json::object_t::const_iterator pr(ra.begin()), pt(ta->begin());
                for (; pr != ra.end() && pt != ta->end(); ++pr, ++pt) {
                    if (pr->first != pt->first || pr->second != pt->second)
                        return false;
                }
                return pr == ra.end() && pt == ta->end();
            }
        }
    };
}
bool fostlib::json::operator==(const json &r) const {
    return std::visit(::comparator(r.m_element), m_element);
}

namespace {
    struct comparator_string {
        f5::u8view with;
        bool operator()(f5::lstring s) { return s == with; }
        bool operator()(json::string_t const &s) { return s == with; }
        template<typename T>
        bool operator()(const T &) {
            return false;
        }
    };
}
bool fostlib::json::operator==(f5::u8view v) const {
    return std::visit(::comparator_string{v}, m_element);
}


namespace {
    struct size_finder {
        json::array_t::size_type operator()(std::monostate) const { return 0; }
        template<typename T>
        json::array_t::size_type operator()(const T &) const {
            return 1;
        }
        json::array_t::size_type operator()(const json::array_p &a) const {
            return a->size();
        }
        json::array_t::size_type operator()(const json::object_p &o) const {
            return o->size();
        }
    };
}
json::array_t::size_type fostlib::json::size() const {
    return std::visit(::size_finder(), m_element);
}


namespace {
    struct array_has_key {
        json::array_t::size_type k;
        array_has_key(json::array_t::size_type k) : k(k) {}

        bool operator()(const json::array_p &a) const { return a->size() > k; }

        template<typename t>
        bool operator()(const t &) const {
            return false;
        }
    };
}
bool fostlib::json::has_key(array_t::size_type k) const {
    return std::visit(::array_has_key(k), m_element);
}


namespace {
    struct object_has_key {
        f5::u8view k;
        object_has_key(f5::u8view k) : k(k) {}

        bool operator()(const json::object_p &o) const {
            return o->find(k) != o->end();
        }

        template<typename t>
        bool operator()(const t &) const {
            return false;
        }
    };
}
bool fostlib::json::has_key(f5::u8view k) const {
    return std::visit(::object_has_key(k), m_element);
}


namespace {
    struct path_has_key {
        const json &blob;
        const jcursor &tail;
        bool has_tail;
        path_has_key(const json &j, const jcursor &p, bool h)
        : blob(j), tail(p), has_tail(h) {}

        template<typename key>
        bool operator()(key i) const {
            if (has_tail && blob.has_key(i))
                return blob[i].has_key(tail);
            else
                return blob.has_key(i);
        }
    };
}
bool fostlib::json::has_key(const jcursor &p) const {
    if (p.m_position.empty())
        return !isnull();
    else {
        jcursor tail(++(p.m_position.begin()), p.m_position.end());
        return std::visit(
                ::path_has_key(*this, tail, tail.m_position.size()),
                *(p.m_position.begin()));
    }
}


namespace {
    struct array_dereference {
        uint64_t p;
        array_dereference(json::array_t::size_type p) : p(p) {}
        const json &operator()(const json::array_p &a) const {
            if (p >= a->size()) {
                throw exceptions::out_of_range<
                        json::array_t::size_type, uint64_t>(0, a->size(), p);
            } else {
                return (*a)[json::array_t::size_type(p)];
            }
        }
        template<typename T>
        const json &operator()(const T &) const {
            throw exceptions::out_of_range<json::array_t::size_type, uint64_t>(
                    0, 0, p);
        }
    };
}
const json &fostlib::json::operator[](array_t::size_type p) const {
    try {
        return std::visit(::array_dereference(p), m_element);
    } catch (exceptions::exception &error) {
        insert(error.data(), "key", p);
        insert(error.data(), "array-content", *this);
        throw;
    }
}


namespace {
    const json c_empty;
    struct object_dereference {
        string k;
        object_dereference(string k) : k(k) {}

        const json &operator()(const json::object_p &o) const {
            json::object_t::const_iterator p(o->find(k));
            if (p == o->end())
                return c_empty;
            else
                return p->second;
        }
        const json &operator()(const json::array_p &a) const {
            throw exceptions::json_error(
                    "This json instance is an array so cannot be de-indexed "
                    "with a string",
                    *a);
        }
        template<typename t>
        const json &operator()(const t &v) const {
            throw exceptions::json_error(
                    "This json instance does not represent an object so it "
                    "cannot be de-indexed with a string",
                    json(v));
        }
    };
}
const json &fostlib::json::operator[](const string &w) const {
    try {
        return std::visit(::object_dereference(w), m_element);
    } catch (exceptions::exception &e) {
        insert(e.data(), "json-key", w);
        throw;
    }
}
namespace {
    struct path_walker {
        const json &blob;
        const jcursor &tail;
        path_walker(const json &j, const jcursor &p) : blob(j), tail(p) {}

        const json &operator()(json::array_t::size_type i) const {
            return blob[i][tail];
        }
        const json &operator()(const string &i) const { return blob[i][tail]; }
    };
}
const json &fostlib::json::operator[](const jcursor &p) const {
    if (p.m_position.empty())
        return *this;
    else
        return std::visit(
                ::path_walker(
                        *this,
                        jcursor(++(p.m_position.begin()), p.m_position.end())),
                *(p.m_position.begin()));
}


json::const_iterator fostlib::json::begin() const {
    if (const array_p *a = std::get_if<array_p>(&m_element)) {
        return json::const_iterator(*this, (*a)->begin());
    } else if (const object_p *o = std::get_if<object_p>(&m_element)) {
        return json::const_iterator(*this, (*o)->begin());
    } else {
        return const_iterator();
    }
}
json::const_iterator fostlib::json::end() const {
    if (const array_p *a = std::get_if<array_p>(&m_element)) {
        return json::const_iterator(*this, (*a)->end());
    } else if (const object_p *o = std::get_if<object_p>(&m_element)) {
        return json::const_iterator(*this, (*o)->end());
    } else {
        return const_iterator();
    }
}


/*
    fostlib::json::const_iterator
*/

namespace {
    using iterator_variant = std::variant<
            std::monostate,
            json::array_t::const_iterator,
            json::object_t::const_iterator>;
}

fostlib::json::const_iterator::const_iterator()
: m_iterator(), m_parent(NULL) {}
fostlib::json::const_iterator::const_iterator(
        const json &parent, array_t::const_iterator i)
: m_iterator(i), m_parent(&parent) {}
fostlib::json::const_iterator::const_iterator(
        const json &parent, object_t::const_iterator i)
: m_iterator(i), m_parent(&parent) {}

namespace {
    struct iter_key {
        json operator()(std::monostate) const {
            throw exceptions::not_implemented(
                    L"fostlib::json::const_iterator::key() const -- for a null "
                    L"iterator");
        }
        json operator()(json::array_t::const_iterator const &) const {
            throw exceptions::not_implemented(
                    L"fostlib::json::const_iterator::key() const -- for an "
                    L"array iterator");
        }
        json operator()(const json::object_t::const_iterator &i) const {
            return json(i->first);
        }
    };
}
fostlib::json fostlib::json::const_iterator::key() const {
    return std::visit(iter_key(), this->m_iterator);
}

namespace {
    struct iter_eq {
        const iterator_variant *self;
        iter_eq(const iterator_variant &s) : self(&s) {}

        bool operator()(std::monostate) const {
            return std::get_if<std::monostate>(self);
        }
        template<typename iter>
        bool operator()(const iter &i) const {
            const iter *s = std::get_if<iter>(self);
            return s && *s == i;
        }
    };
}
bool fostlib::json::const_iterator::operator==(const_iterator r) const {
    return std::visit(iter_eq(this->m_iterator), r.m_iterator);
}

namespace {
    struct iter_deref {
        const json &operator()(std::monostate) const {
            throw exceptions::null(L"Cannot dereference a null iterator");
        }
        const json &operator()(const json::array_t::const_iterator &i) const {
            return *i;
        }
        const json &operator()(const json::object_t::const_iterator &i) const {
            return i->second;
        }
    };
}
const json &fostlib::json::const_iterator::operator*() const {
    return std::visit(iter_deref(), m_iterator);
}

namespace {
    struct iter_inc {
        void operator()(std::monostate) const {
            throw exceptions::null(L"Cannot increment a null iterator");
        }
        template<typename iter>
        void operator()(iter &i) const {
            ++i;
        }
    };
}
json::const_iterator &json::const_iterator::operator++() {
    std::visit(iter_inc(), m_iterator);
    return *this;
}
