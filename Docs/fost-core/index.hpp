/** \defgroup fost_core fost-core
    \ingroup fost_base

    \brief fost-core is the bottom most layer of the Fost 4 libraries. It contains a number of foundation sub-libraries.
*/


/** \class fostlib::dynlib fost/dynlib.hpp fost/dynlib.hpp
    \ingroup fost_core

    \brief Used to load shared objects or DLLs at runtime.

    There are separate implementations of this class for Windows and for Linux/Mac OSX due to platform differences. On all platforms the dynlib instance assures that the library is loaded into memory and is properly initialised.

    In all implementations the destructor does not actually attempt to unload the library. It uses fostlib::atexit() to unload the library when the process terminates. Without this the management of the lifetimes of objects whose code is loaded in the library must be carefully managed so that they are all deleted before the library is unloaded.

    <b>Note that fost/core must be included before this header.</b> I.e.:
    <pre class="language-cpp">
    \#include <fost/core>
    \#include <fost/dynlib.hpp>
    </pre>

    \section windows Windows notes

    The Windows implementation makes use of LoadLibraryW so the name does not need to be complete, nor does it need an extension.

    The library is never unloaded as Windows will take care of this properly as the process terminates.

    \section unix Linux and Mac notes

    The library name must include the full file name, but not a path.
*/
/** \fn fostlib::dynlib::dynlib (const string &path)
    \param path The shared object/DLL to load.

    Loads and initialises a dynamic library.
*/
