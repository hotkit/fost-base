/** \defgroup fost_core_exceptions exceptions
    \ingroup fost_core

    \brief A rich set of exception classes that allow for annotation of
   exception information as well as being derived from std::exception.

    Fost exceptions are always throw by instance and are always caught by
   reference. All of these exceptions can be annotated with extra through a
   try/catch block which adds further explanations to the
   fostlib::exceptions::exception::info() member.

    <pre>
    try {
        throw fostlib::exceptions::not_implemented("This part not done");
    } catch ( fostlib::exceptions::exception &e ) {
        e.info() << "Whoops in the example" << std::endl;
        throw;
    }
    </pre>

    On Windows the exception handling includes translation of structured
   exceptions to fostlib::exceptions::exception classes where safe.
*/


/** \class fostlib::exceptions::ctrl_break fost/exception.hpp fost/core
    \ingroup fost_core_exceptions

    This exception will in the future be used to handle CTRL-C or CTRL-BRK
   functionality. It is not currently in use.
*/


/** \class fostlib::exceptions::exception fost/exception.hpp fost/core
    \ingroup fost_core_exceptions

    \brief The parent class for the Fost exceptions. This class derives from
   std::exception.

    Note that this class is abstract and so can never be thrown. If you wish to
   annotate exceptions with extra information about what the program was doing
   then this is the type that should be caught.

    <pre class="language-cpp">
    void foo( const bar &b ) {
        try {
            <i>Do something with b</i>
        } catch ( fostlib::exceptions::exception &e ) {
            e.info() << "Attempting to foo with the bar instance: " << b <<
   std::endl; throw;
        }
    }
    </pre>

    The fostlib::exceptions::exception::info() member returns a iostream that
   can be annotated with information that will help to debug what the program
   was doing when the exception was thrown.
*/
