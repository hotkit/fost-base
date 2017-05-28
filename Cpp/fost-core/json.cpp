/*
    Copyright 2007-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
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


namespace {
    struct isnull : public boost::static_visitor< bool > {
        bool operator () (t_null) const {
            return true;
        }
        template< typename t >
        bool operator () (const t &) const {
            return false;
        }
    };
}
bool fostlib::json::isnull() const {
    return boost::apply_visitor( ::isnull(), m_element );
}


namespace {
    struct isatom : public boost::static_visitor<bool> {
        bool operator () (t_null) const {
            return false;
        }
        template< typename t >
        bool operator () (const t &) const {
            return true;
        }
        bool operator () (const json::array_p &) const {
            return false;
        }
        bool operator () (const json::object_p &) const {
            return false;
        }
    };
}
bool fostlib::json::isatom() const {
    return boost::apply_visitor( ::isatom(), m_element );
}


namespace {
    struct isarray : public boost::static_visitor< bool > {
        bool operator ()( const json::array_p & ) const {
            return true;
        }
        template< typename t >
        bool operator ()( const t & ) const {
            return false;
        }
    };
}
bool fostlib::json::isarray() const {
    return boost::apply_visitor( ::isarray(), m_element );
}


namespace {
    struct isobject : public boost::static_visitor< bool > {
        bool operator ()( const json::object_p & ) const {
            return true;
        }
        template< typename t >
        bool operator ()( const t & ) const {
            return false;
        }
    };
}
bool fostlib::json::isobject() const {
    return boost::apply_visitor( ::isobject(), m_element );
}


namespace {
    struct comparator : public boost::static_visitor< bool > {
        const json::element_t &r;
        comparator( const json::element_t &r ) : r( r ) {}

        template<typename T>
        bool operator () (const T &t) const {
            auto rvp = boost::get<T>(&r);
            return rvp && t == *rvp;
        }
        bool operator () (const json::string_p &t) const {
            auto rvp = boost::get<json::string_p>(&r);
            return rvp && *t == **rvp;
        }
        bool operator ()( const json::array_p &ta ) const {
            if ( !boost::get<json::array_p>(&r) ) return false;
            else {
                const json::array_p &ra(boost::get<json::array_p>(r));
                json::array_t::const_iterator pr(ra->begin() ), pt(ta->begin());
                for ( ; pr != ra->end() && pt != ta->end(); ++pr, ++pt ) {
                    if ( *pr != *pt ) return false;
                }
                return pr == ra->end() && pt == ta->end();
            }
        }
        bool operator ()(const json::object_p &ta) const {
            if ( !boost::get<json::object_p>(&r) )
                return false;
            else {
                const json::object_t &ra(*boost::get<json::object_p>(r));
                json::object_t::const_iterator pr(ra.begin()), pt(ta->begin());
                for ( ; pr != ra.end() && pt != ta->end(); ++pr, ++pt ) {
                    if ( pr->first != pt->first || pr->second != pt->second ) return false;
                }
                return pr == ra.end() && pt == ta->end();
            }
        }
    };
}
bool fostlib::json::operator ==( const json &r ) const {
    return boost::apply_visitor( ::comparator( r.m_element ), m_element );
}


namespace {
    struct size_finder : public boost::static_visitor< json::array_t::size_type > {
        json::array_t::size_type operator () (t_null) const {
            return 0;
        }
        template<typename T>
        json::array_t::size_type operator () (const T &) const {
            return 1;
        }
        json::array_t::size_type operator () (const json::array_p &a) const {
            return a->size();
        }
        json::array_t::size_type operator () (const json::object_p &o) const {
            return o->size();
        }
    };
}
json::array_t::size_type fostlib::json::size() const {
    return boost::apply_visitor( ::size_finder(), m_element );
}


namespace {
    struct array_has_key : public boost::static_visitor< bool > {
        json::array_t::size_type k;
        array_has_key( json::array_t::size_type k ) : k( k ) {}

        bool operator ()(const json::array_p &a) const {
            return a->size() > k;
        }

        template< typename t >
        bool operator ()( const t & ) const {
            return false;
        }
    };
}
bool fostlib::json::has_key( array_t::size_type k ) const {
    return boost::apply_visitor( ::array_has_key( k ), m_element );
}


namespace {
    struct object_has_key : public boost::static_visitor< bool > {
        string k;
        object_has_key( string k ) : k( k ) {}

        bool operator ()(const json::object_p &o) const {
            return o->find( k ) != o->end();
        }

        template< typename t >
        bool operator ()( const t & ) const {
            return false;
        }
    };
}
bool fostlib::json::has_key( const string &k ) const {
    return boost::apply_visitor( ::object_has_key( k ), m_element );
}


namespace {
    struct path_has_key : public boost::static_visitor< bool > {
        const json &blob; const jcursor &tail; bool has_tail;
        path_has_key( const json &j, const jcursor &p, bool h )
        : blob( j ), tail( p ), has_tail( h ) {
        }

        template< typename key >
        bool operator()( key i ) const {
            if ( has_tail && blob.has_key( i ) )
                return blob[ i ].has_key( tail );
            else
                return blob.has_key( i );
        }
    };
}
bool fostlib::json::has_key( const jcursor &p ) const {
    if ( p.m_position.empty() )
        return !isnull();
    else {
        jcursor tail( ++( p.m_position.begin() ), p.m_position.end() );
        return boost::apply_visitor( ::path_has_key( *this, tail, tail.m_position.size() ), *( p.m_position.begin() ) );
    }
}


namespace {
    struct array_dereference {
        uint64_t p;
        array_dereference(json::array_t::size_type p) : p( p ) {}
        const json &operator ()(const json::array_p &a) const {
            if ( p >= a->size() ) {
                throw exceptions::out_of_range<
                    json::array_t::size_type, uint64_t >(0, a->size(), p);
            } else {
                return (*a)[json::array_t::size_type(p)];
            }
        }
        template< typename T >
        const json &operator ()(const T &t) const {
            throw exceptions::out_of_range<
                json::array_t::size_type, uint64_t>(0, 0, p);
        }
    };
}
const json &fostlib::json::operator [] ( array_t::size_type p ) const {
    try {
        return boost::apply_visitor(::array_dereference( p ), m_element);
    } catch ( exceptions::exception &error ) {
        insert(error.data(), "key", p);
        insert(error.data(), "array-content", *this);
        throw;
    }
}


namespace {
    const json c_empty;
    struct object_dereference {
        string k;
        object_dereference( string k ) : k( k ) {}

        const json &operator () (const json::object_p &o) const {
            json::object_t::const_iterator p(o->find(k));
            if ( p == o->end() ) return c_empty;
            else return p->second;
        }
        const json &operator () (const json::array_p &a) const {
            throw exceptions::json_error(
                "This json instance is an array so cannot be de-indexed with a string", *a);
        }
        template< typename t >
        const json &operator () (const t &v) const {
            throw exceptions::json_error(
                "This json instance does not represent an object so it cannot be de-indexed with a string",
                json(v));
        }
    };
}
const json &fostlib::json::operator []( const string &w ) const {
    try {
        return boost::apply_visitor( ::object_dereference( w ), m_element );
    } catch ( exceptions::exception &e ) {
        insert(e.data(), "json-key", w);
        throw;
    }
}
namespace {
    struct path_walker {
        const json &blob; const jcursor &tail;
        path_walker( const json &j, const jcursor &p ) : blob( j ), tail( p ) {}

        const json &operator()( json::array_t::size_type i ) const {
            return blob[ i ][ tail ];
        }
        const json &operator()( const string &i ) const {
            return blob[ i ][ tail ];
        }
    };
}
const json &fostlib::json::operator[]( const jcursor &p ) const {
    if ( p.m_position.empty() )
        return *this;
    else
        return boost::apply_visitor( ::path_walker( *this, jcursor( ++( p.m_position.begin() ), p.m_position.end() ) ), *( p.m_position.begin() ) );
}


json::const_iterator fostlib::json::begin() const {
    if ( const array_p *a = boost::get<array_p>(&m_element) ) {
        return json::const_iterator(*this, (*a)->begin());
    } else if ( const object_p *o = boost::get<object_p>(&m_element) ) {
        return json::const_iterator(*this, (*o)->begin());
    } else {
        return const_iterator();
    }
}
json::const_iterator fostlib::json::end() const {
    if ( const array_p *a = boost::get<array_p>(&m_element) ) {
        return json::const_iterator(*this, (*a)->end());
    } else if ( const object_p *o = boost::get<object_p>(&m_element) ) {
        return json::const_iterator(*this, (*o)->end());
    } else {
        return const_iterator();
    }
}


/*
    fostlib::json::const_iterator
*/

namespace {
    typedef boost::variant<
        t_null,
        json::array_t::const_iterator,
        json::object_t::const_iterator
    > iterator_variant;
}

fostlib::json::const_iterator::const_iterator()
: m_iterator(null), m_parent(NULL) {
}
fostlib::json::const_iterator::const_iterator( const json &parent, array_t::const_iterator i )
: m_iterator( i ), m_parent( &parent ) {
}
fostlib::json::const_iterator::const_iterator( const json &parent, object_t::const_iterator i )
: m_iterator( i ), m_parent( &parent ) {
}

namespace {
    struct iter_key : boost::static_visitor< json > {
        json operator () ( t_null ) const {
            throw exceptions::not_implemented( L"fostlib::json::const_iterator::key() const -- for a null iterator" );
        }
        json operator () ( const json::array_t::const_iterator &i ) const {
            throw exceptions::not_implemented( L"fostlib::json::const_iterator::key() const -- for an array iterator" );
        }
        json operator () ( const json::object_t::const_iterator &i ) const {
            return json( i->first );
        }
    };
}
fostlib::json fostlib::json::const_iterator::key() const {
    return boost::apply_visitor( iter_key(), this->m_iterator );
}

namespace {
    struct iter_eq : boost::static_visitor< bool > {
        const iterator_variant *self;
        iter_eq( const iterator_variant &s ) : self( &s ) {}

        bool operator () ( t_null ) const {
            return boost::get< t_null >( self );
        }
        template< typename iter >
        bool operator () ( const iter &i ) const {
            const iter *s = boost::get< iter >( self );
            return s && *s == i;
        }
    };
}
bool fostlib::json::const_iterator::operator == ( const_iterator r ) const {
    return boost::apply_visitor( iter_eq( this->m_iterator ), r.m_iterator );
}

namespace {
    struct iter_deref {
        const json &operator () ( t_null ) const {
            throw exceptions::null( L"Cannot dereference a null iterator" );
        }
        const json &operator () (const json::array_t::const_iterator &i) const {
            return *i;
        }
        const json &operator () (const json::object_t::const_iterator &i) const {
            return i->second;
        }
    };
}
const json &fostlib::json::const_iterator::operator * () const {
    return boost::apply_visitor( iter_deref(), m_iterator );
}

namespace {
    struct iter_inc : boost::static_visitor< void > {
        void operator () ( t_null ) const {
            throw exceptions::null( L"Cannot increment a null iterator" );
        }
        template< typename iter >
        void operator () ( iter &i ) const {
            ++i;
        }
    };
}
json::const_iterator &json::const_iterator::operator ++ () {
    boost::apply_visitor( iter_inc(), m_iterator );
    return *this;
}
