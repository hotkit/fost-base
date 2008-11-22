/*
	$Revision: 64 $
	$Date: 16/06/06 22:22 $
	Copyright (C) 1999-2008, Felspar. Contact "http://fost.3.felspar.com".
*/


#ifndef FOST_DB_HPP
#define FOST_DB_HPP
#pragma once


#include "FOST.timedate.hpp"
#include "FOST.com.hpp"


#ifdef F3UTIL_EXPORTS
#define FOST_DB_HPP_ARCHIVE L"$Archive: /FOST.3/F3Util/FOST.db.hpp $"
#define FOST_DB_HPP_REVISION L"$Revision: 64 $"
#define FOST_DB_HPP_DATE L"$Date: 16/06/06 22:22 $"
#endif


namespace FSLib {


	namespace Exceptions {


		class F3UTIL_DECLSPEC DataDriver : public Exception {
		public:
            DataDriver( const fostlib::string &error, const fostlib::string &driver );
            DataDriver( const fostlib::string &error, const fostlib::string &driver1, const fostlib::string &driver2 );

		protected:
			const wchar_t * const message() const throw ();
		};


		class F3UTIL_DECLSPEC TransactionFault : public Exception {
		public:
			TransactionFault( const fostlib::string &error );

		protected:
			const wchar_t * const message() const throw ();
		};


		class F3UTIL_DECLSPEC StringDecode : public Exception {
		public:
			const fostlib::accessors< fostlib::string > string;

			StringDecode( const fostlib::string & );

		protected:
			const wchar_t * const message() const throw ();
		};


		class F3UTIL_DECLSPEC FieldCastFault : public Exception {
		public:
			FieldCastFault( const fostlib::string &error );

		protected:
			const wchar_t * const message() const throw();
		};


	}

	namespace Mangling {


		typedef enum { e_sql, e_fost } t_translation;

		F3UTIL_DECLSPEC fostlib::string mangle( const fostlib::string &, t_translation trans );
		F3UTIL_DECLSPEC fostlib::string fromSQL( const fostlib::string &, t_translation trans );

		template< typename T > inline
		fostlib::string toSQL( const T &v, t_translation trans ) {
			return toSQL( v.asString(), trans );
		}
		template< typename N > inline
		fostlib::string nullableToSQL( const fostlib::nullable< N > &v, t_translation trans ) {
			if ( v.isnull() ) {
				return toSQL< FSLib::t_null >( FSLib::Null, trans );
			} else {
				return toSQL< N >( v.value(), trans );
			}
		}

		template<> F3UTIL_DECLSPEC fostlib::string toSQL< FSLib::t_null >( const FSLib::t_null &, t_translation trans );
		template<> F3UTIL_DECLSPEC fostlib::string toSQL< bool >( const bool &, t_translation trans );
		template<> F3UTIL_DECLSPEC fostlib::string toSQL< long >( const long &, t_translation trans );
		template<> F3UTIL_DECLSPEC fostlib::string toSQL< int64_t >( const int64_t &, t_translation );
		template<> F3UTIL_DECLSPEC fostlib::string toSQL< float >( const float &, t_translation );
		template<> F3UTIL_DECLSPEC fostlib::string toSQL< double >( const double &, t_translation );
		template<> F3UTIL_DECLSPEC fostlib::string toSQL< fostlib::string >( const fostlib::string &, t_translation trans );

		template< typename T > inline
		fostlib::string toSQL( const Period< T > &p, t_translation trans, unsigned long c ) {
			switch ( c ) {
			case 0: return toSQL( p.period(), trans );
			case 1: return toSQL( p.value(), trans );
			default: throw FSLib::Exceptions::OutOfRange< unsigned long >( L"Column value out of range for FSLib::Period< T >", 0, 2, c );
			}
		}


	}


	class DBConnection;
    class Recordset;


    class F3UTIL_DECLSPEC DBInterface {
    protected:
        DBInterface( const fostlib::string &driver_name, Mangling::t_translation t );
    public:
        class Read;
        class Recordset;
        class Write;

        static const DBInterface &connection( const fostlib::string &read, const fostlib::nullable< fostlib::string > &write );

        virtual void create_database( DBConnection &dbc, const fostlib::string &name ) const = 0;
        virtual void drop_database( DBConnection &dbc, const fostlib::string &name ) const = 0;

        fostlib::accessors< const Mangling::t_translation > translation;
        virtual boost::shared_ptr< FSLib::DBInterface::Read > reader( FSLib::DBConnection &dbc ) const = 0;

        class F3UTIL_DECLSPEC Read : boost::noncopyable {
        protected:
            Read( DBConnection &dbc );
        public:
            virtual ~Read();

            virtual boost::shared_ptr< Write > writer() = 0;
            virtual boost::shared_ptr< Recordset > recordset( const fostlib::string & ) const = 0;

        protected:
            DBConnection &m_connection;
        private:
            bool m_inTransaction;
            friend class FSLib::DBInterface::Write;
        };
        class F3UTIL_DECLSPEC Write : boost::noncopyable {
        protected:
            Write( Read &reader );
        public:
            virtual ~Write();

            virtual void create_table( const fostlib::string &table, const std::list< std::pair< fostlib::string, fostlib::string > > &key, const std::list< std::pair< fostlib::string, fostlib::string > > &columns ) = 0;
            virtual void drop_table( const fostlib::string &table ) = 0;

            virtual void execute( const fostlib::string & ) = 0;
            virtual void commit() = 0;
            virtual void rollback() = 0;

        protected:
            DBConnection &m_connection;
            Read &m_reader;
        };

        class F3UTIL_DECLSPEC Recordset : boost::noncopyable {
        protected:
            Recordset( const DBInterface &dbi, const fostlib::string &cmd );
        public:
            fostlib::accessors< const fostlib::string > command;
            fostlib::accessors< const Mangling::t_translation > translation;

            virtual ~Recordset();

            virtual bool eof() const = 0;
            virtual void moveNext() = 0;
            virtual std::size_t fields() const = 0;
            virtual const fostlib::string &name( std::size_t f ) const = 0;
            virtual const Json &field( std::size_t i ) const = 0;
            virtual const Json &field( const fostlib::string &name ) const = 0;
        };
    };


    class F3UTIL_DECLSPEC Recordset {
	public:
		Recordset( boost::shared_ptr< DBInterface::Recordset > rs );
		~Recordset();

        const fostlib::string &command() const {
            return m_interface->command();
        }
		bool eof() const;
		void moveNext();

		std::size_t fields() const;
        const fostlib::string &name( std::size_t f ) const;
		const Json &field( const fostlib::string &i ) const;
		const Json &field( std::size_t i ) const;

        bool isnull( const fostlib::string & ) const;
		bool isnull( std::size_t ) const;

        fostlib::accessors< const Mangling::t_translation > translation;

	private:
        boost::shared_ptr< DBInterface::Recordset > m_interface;
	};


    class F3UTIL_DECLSPEC Transaction : boost::noncopyable {
    public:
        Transaction( DBConnection &dbc );
        ~Transaction();

        void create_table( const fostlib::string &table, const std::list< std::pair< fostlib::string, fostlib::string > > &key, const std::list< std::pair< fostlib::string, fostlib::string > > &columns );
        void drop_table( const fostlib::string &table );

        Transaction &execute( const fostlib::string & );
        void commit();

    private:
        DBConnection &m_connection;
        boost::shared_ptr< DBInterface::Write > m_transaction;
    };


    class F3UTIL_DECLSPEC DBConnection : boost::noncopyable {
        friend class Transaction;
    public:
        static const Setting< bool > c_commitCount;
        static const Setting< long > c_commitCountDomain;

        DBConnection( const fostlib::string &readDSN );
        DBConnection( const fostlib::string &readDSN, const fostlib::string &writeDSN );
        ~DBConnection();

        const DBInterface &dbInterface() const;

        void create_database( const fostlib::string &name );
        void drop_database( const fostlib::string &name );

        Recordset recordset( const fostlib::string &command );

        bool inTransaction() const;
        Transaction &transaction();

        fostlib::accessors< const fostlib::string > readDSN;
        fostlib::accessors< const fostlib::nullable< fostlib::string > > writeDSN;

    private:
        bool m_readOnly;
        const DBInterface &m_interface;
        boost::shared_ptr< DBInterface::Read > m_connection;
        Transaction *m_transaction;
    };


	namespace Private {


		// Templated holding function throws exception
		template< typename T > inline
		T impl_field_cast( const Recordset &r, const fostlib::string &i, const T & ) {
			return coerce< T >( r.field( i ) );
		}
		template< typename T > inline
		T impl_field_cast( const Recordset &r, long i, const T & ) {
			return coerce< T >( r.field( i ) );
		}


		// Specialisations for string implementation cast functions
		template<> inline
		fostlib::string impl_field_cast( const Recordset &r, const fostlib::string &i, const fostlib::string & ) {
			return FSLib::Mangling::fromSQL( coerce< fostlib::string >( r.field( i ) ), r.translation() );
		}
		template<> inline
		fostlib::string impl_field_cast( const Recordset &r, long i, const fostlib::string & ) {
			return FSLib::Mangling::fromSQL( coerce< fostlib::string >( r.field( i ) ), r.translation() );
		}


	}


	template< typename T > inline 
	T field_cast( const Recordset &r, const fostlib::string & i ) {
		try {
			return FSLib::Private::impl_field_cast( r, i, T() );
		} catch ( FSLib::Exceptions::Exception &e ) {
			e.info() << L"Field: " << i << std::endl;
			throw;
		}
	}
	template< typename T > inline
	T field_cast( const Recordset &r, long i ) {
		try {
			return FSLib::Private::impl_field_cast( r, i, T() );
		} catch ( FSLib::Exceptions::Exception &e ) {
			e.info() << L"Field: " << i << std::endl;
			throw;
		}
	}


	template< typename T > inline
	fostlib::nullable<T> nullable_cast( const Recordset &rs, const fostlib::string & i ) {
		if ( !rs.isnull( i ) ) {
			return field_cast< T >( rs, i );
		} else {
			return fostlib::nullable< T >();
		}
	}
	template< typename T > inline
	fostlib::nullable<T> nullable_cast( const Recordset &rs, long i ) {
		if ( !rs.isnull( i ) ) {
			return field_cast< T >( rs, i );
		} else {
			return fostlib::nullable< T >();
		}
	}


	template< typename T > inline
	Period<T> period_cast( const Recordset &r, const fostlib::string &f ) {
		return Period< T >( period_cast< void >( r, f + L"_period" ), field_cast< T >( r, f ) );
	}
	template< typename T > inline
	Period<T> period_cast( const Recordset &r, long f ) {
		return Period< T >( period_cast< void >( r, f ), field_cast< T >( r, f + 1 ) );
	}


	template<> inline
	Period< void > period_cast( const Recordset &r, const fostlib::string &f ) {
		return Period< void >( field_cast< fostlib::string >( r, f ) );
	}
	template<> inline
	Period< void > period_cast( const Recordset &r, long f ) {
		return Period< void >( field_cast< fostlib::string >( r, f ) );
	}


	inline
	Period< void > period_cast( const Recordset & r ) {
		return Period< void >( field_cast< fostlib::string >( r, L"this_period" ) );
	}


}


#include "FOST.db.schema.hpp"
#include "FOST.db.stream.hpp"


#endif // FOST_DB_HPP
