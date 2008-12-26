/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/json.hpp>
#include <fost/string/coerce.hpp>

#include <fost/exception/not_implemented.hpp>
#include <fost/exception/null.hpp>
#include <fost/exception/not_null.hpp>
#include <fost/exception/out_of_range.hpp>


using namespace fostlib;


fostlib::json::json()
: m_element( variant() ) {
}


namespace {
    struct isnull : public boost::static_visitor< bool > {
        bool operator ()( const json::atom_t &t ) const {
            return t.isnull();
        }
        template< typename t >
        bool operator ()( const t & ) const {
            return false;
        }
    };
}
bool fostlib::json::isnull() const {
    return boost::apply_visitor( ::isnull(), m_element );
}
namespace {
    struct isatom : public boost::static_visitor< bool > {
        bool operator ()( const json::atom_t &t ) const {
            return !t.isnull();
        }
        template< typename t >
        bool operator ()( const t & ) const {
            return false;
        }
    };
}
bool fostlib::json::isatom() const {
    return boost::apply_visitor( ::isatom(), m_element );
}
namespace {
    struct isarray : public boost::static_visitor< bool > {
        bool operator ()( const json::array_t & ) const {
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
        bool operator ()( const json::object_t & ) const {
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
        bool operator ()( const json::atom_t &t ) const {
            return boost::get< json::atom_t >( &r ) && t == boost::get< json::atom_t >( r );
        }
        bool operator ()( const json::array_t &ta ) const {
            if ( !boost::get< json::array_t >( &r ) )
                return false;
            else {
                const json::array_t &ra( boost::get< json::array_t >( r ) );
                json::array_t::const_iterator pr( ra.begin() ), pt( ta.begin() );
                for ( ; pr != ra.end() && pt != ta.end(); ++pr, ++pt )
                    if ( **pr != **pt )
                        return false;
                return pr == ra.end() && pt == ta.end();
            }
        }
        bool operator ()( const json::object_t &ta ) const {
            if ( !boost::get< json::object_t >( &r ) )
                return false;
            else {
                const json::object_t &ra( boost::get< json::object_t >( r ) );
                json::object_t::const_iterator pr( ra.begin() ), pt( ta.begin() );
                for ( ; pr != ra.end() && pt != ta.end(); ++pr, ++pt )
                    if ( pr->first != pt->first || *(pr->second) != *(pt->second) )
                        return false;
                return pr == ra.end() && pt == ta.end();
            }
        }
    };
}
bool fostlib::json::operator ==( const json &r ) const {
    return boost::apply_visitor( ::comparator( r.m_element ), m_element );
}


namespace {
    struct atom_size_finder : public boost::static_visitor< json::array_t::size_type > {
        json::array_t::size_type operator ()( t_null ) const {
            return 0;
        }
        template< typename t >
        json::array_t::size_type operator ()( const t & ) const {
            return 1;
        }
    };
    struct size_finder : public boost::static_visitor< json::array_t::size_type > {
        json::array_t::size_type operator ()( const json::atom_t &a ) const {
            return boost::apply_visitor( ::atom_size_finder(), a );
        }
        template< typename t >
        json::array_t::size_type operator ()( const t &a ) const {
            return a.size();
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

        bool operator ()( const json::array_t &a ) const {
            return a.size() > k;
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

        bool operator ()( const json::object_t &o ) const {
            return o.find( k ) != o.end();
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
    struct array_dereference : public boost::static_visitor< const json & > {
        uint64_t p;
        array_dereference( json::array_t::size_type p ) : p( p ) {}
        const json &operator ()( const json::array_t &a ) const {
            if ( p < 0 || p >= a.size() )
                throw fostlib::exceptions::out_of_range< json::array_t::size_type, uint64_t >( 0, a.size(), p );
            else
                return *a[ json::array_t::size_type( p ) ];
        }
        template< typename t >
        const json &operator ()( const t & ) const {
            throw fostlib::exceptions::not_implemented( L"json & array_dereference::operator ()( const " + coerce< string >( typeid( t ).name() ) + L" & ) const" );
        }
    };
}
const json &fostlib::json::operator []( array_t::size_type p ) const {
    return boost::apply_visitor( ::array_dereference( p ), m_element );
}
namespace {
    const json c_empty;
    struct object_dereference : public boost::static_visitor< const json & > {
        string k;
        object_dereference( string k ) : k( k ) {}

        const json &operator ()( const json::object_t &o ) const {
            json::object_t::const_iterator p( o.find( k ) );
            if ( p == o.end() )
                return c_empty;
            else
                return *(p->second);
        }
        template< typename t >
        const json &operator ()( const t &v ) const {
            throw fostlib::exceptions::not_implemented(
                L"json & object_dereference::operator ()( const " +
                    coerce< string >( typeid( t ).name() ) +
                L" & ) const",
                json::unparse( json( v ), true ) );
        }
    };
}
const json &fostlib::json::operator []( const string &w ) const {
    return boost::apply_visitor( ::object_dereference( w ), m_element );
}
namespace {
    struct path_walker : public boost::static_visitor< const json & > {
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
    if ( const array_t *a = boost::get< array_t >( &m_element ) )
        return json::const_iterator( *this, a->begin() );
    else if ( const object_t *o = boost::get< object_t >( &m_element ) )
        return json::const_iterator( *this, o->begin() );
    else
        return const_iterator();
}
json::const_iterator fostlib::json::end() const {
    if ( const array_t *a = boost::get< array_t >( &m_element ) )
        return json::const_iterator( *this, a->end() );
    else if ( const object_t *o = boost::get< object_t >( &m_element ) )
        return json::const_iterator( *this, o->end() );
    else
        return const_iterator();
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
: m_parent( NULL ) {
}
fostlib::json::const_iterator::const_iterator( const json &parent, array_t::const_iterator i )
: m_iterator( i ), m_parent( &parent ) {
}
fostlib::json::const_iterator::const_iterator( const json &parent, object_t::const_iterator i )
: m_iterator( i ), m_parent( &parent ) {
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
    struct iter_deref : boost::static_visitor< const json & > {
        const json &operator () ( t_null ) const {
            throw exceptions::null( L"Cannot dereference a null iterator" );
        }
        const json &operator () ( const json::array_t::const_iterator &i ) const {
            return **i;
        }
        const json &operator () ( const json::object_t::const_iterator &i ) const {
            return *(i->second);
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
