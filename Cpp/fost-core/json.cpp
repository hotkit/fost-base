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


/*
    push_back
*/
namespace {
    struct atom_back_pusher : public boost::static_visitor< void > {
        void operator ()( const t_null ) const {
        }
        template< typename t >
        void operator ()( const t & ) const {
            throw fostlib::exceptions::not_implemented( L"void atom_back_pusher::operator ()( const " + coerce< string >( typeid( t ).name() ) + L" &j ) const" );
        }
    };
    struct json_back_pusher : public boost::static_visitor< json::element_t > {
        const json &j;
        json_back_pusher( const json &j ) : j( j ) {}

        json::element_t operator ()( json::atom_t &a ) const {
            json::array_t arr;
            if ( !a.isnull() )
                arr.push_back( boost::shared_ptr< json >( new json( a ) ) );
            arr.push_back( boost::shared_ptr< json >( new json( j ) ) );
            return arr;
        }

        json::element_t operator ()( json::array_t &a ) const {
            a.push_back( boost::shared_ptr< json >( new json( j ) ) );
            return a;
        }

        template< typename t >
        json::element_t operator ()( t & ) const {
            throw fostlib::exceptions::not_implemented( L"void json_back_pusher::operator ()( " + coerce< string >( typeid( t ).name() ) + L" &j ) const" );
        }
    };
}
json &fostlib::json::push_back( const json &j ) {
    m_element = boost::apply_visitor( ::json_back_pusher( j ), m_element );
    return *this;
}


namespace {
    struct object_assigner : public boost::static_visitor< void > {
        const std::pair< string, json > &assign;
        bool insert, replace;
        object_assigner( const std::pair< string, json > &p, bool ins, bool rep )
        : assign( p ), insert( ins ), replace( rep ) {
        }

        void operator()( json::atom_t &/*a*/ ) const {
            throw fostlib::exceptions::not_implemented( L"cannot add an object at a key to a json atom" );
        }
        void operator()( json::object_t &t ) const {
            json::object_t::iterator p( t.find( assign.first ) );
            if ( p != t.end() ) {
                if ( !replace )
                    throw exceptions::not_null( L"Cannot insert into an object where key already exists" );
                else
                    p->second = boost::shared_ptr< json >( new json( assign.second ) );
            } else if ( p == t.end() ) {
                if ( !insert )
                    throw exceptions::null( L"Cannot replace a non-existent item in an object" );
                else
                    t[ assign.first ] = boost::shared_ptr< json >( new json( assign.second ) );
            }
        }

        template< typename t >
        void operator ()( t & ) const {
            throw fostlib::exceptions::not_implemented( L"void object_inserter::operator ()( " + coerce< string >( typeid( t ).name() ) + L" &j ) const" );
        }
    };
}
json &fostlib::json::assign( const std::pair< string, json > &v, assign_operation operation ) {
    switch ( operation ) {
    case e_strict_insert:
        boost::apply_visitor( ::object_assigner( v, true, false ), m_element );
        break;
    case e_strict_replace:
        boost::apply_visitor( ::object_assigner( v, false, true ), m_element );
        break;
    case e_destructive:
        boost::apply_visitor( ::object_assigner( v, true, true ), m_element );
        break;
    }
    return *this;
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
        return false;
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
                throw fostlib::exceptions::out_of_range< json::array_t::size_type >( 0, a.size(), p );
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
                json::unparse( json( v ) ) );
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
