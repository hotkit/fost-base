/** \defgroup fost_core_threading threading
    \ingroup fost_core

    \brief A threading library providing futures, worker threads, and other thread safe/thread based utilities.

    This module logically breaks down into three sections:
    - Thread based execution of code provided by fostlib::worker, fostlib::in_process and fostlib::workerpool.
    - Thread safe datastructures, so far fostlib::library.
    - Thread safe counter, so far fostlib::counter.

    To choose between the thread execution classes, normally application would not make use of fostlib::worker directly. fostlib::in_process is useful where a thread can be represented by an object. Both synchronous and asynchronous messages can be passed to the object. Where a pool is required then fostlib::workerpool should be used.
*/
/** \class fostlib::worker fost/thread.hpp fost/threading
    \ingroup fost_core_threading

    Generally this class is not used directly. For work that is to be done by instances of an object then the fostlib::in_process class is usually a better choice. Where a pool of workers is required then the fostlib::workerpool should be used -- see \ref fost_core_threading.
*/
/** \class fostlib::future fost/thread.hpp fost/threading
    \ingroup fost_core_threading

    The use of futures makes many multi-threaded problems much easier to reason about, but there is a still a chance of deadlocks occuring. This is possible when code that takes a future as a parameter itself gets used as a future. So long as the call graph is an acyclic tree (a DAG, or directed acyclic graph) then there is no possibility of a deadlock. When a cycle is introduced into the graph however then a dead lock might occur.
*/
