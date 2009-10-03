/** \defgroup fost_base fost-base

    \brief This package contains all of the basic Fost 4 functionality for building DLLs/shared-objects, for command line applications and for unit testing.
*/


/** \defgroup fost_cli fost-cli
    \ingroup fost_base

    \brief Contains functionality for command line interface programs.
*/


/** \defgroup fost_crypto fost-crypto
    \ingroup fost_base

    \brief Various cryptographic building blocks.
*/


/** \defgroup fost_datetime fost-datetime
    \ingroup fost_base

    \brief Date and time handling functionality.
*/


/** \defgroup fost_test fost-test
    \ingroup fost_base

    \brief Testing library.
*/


/** \namespace fostlib

    \brief The fostlib namespace is the main namespace used for the Fost libraries.

    This namespace is used for all of the library code within all of the Fost libraries. Application code, for example in the examples, is generally in the global namespace, although larger examples may define their own namespaces.
*/


/** \namespace fostlib::detail

    \brief Used for implementation classes and functions that are needed in public headers so the compiler can generate code. No members of this namespace should be used outside of the implementation of Fost itself.
*/
