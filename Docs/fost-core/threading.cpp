/** \defgroup fost_core_threading threading
    \ingroup fost_core

    \brief A threading library providing futures, worker threads, and other thread safe/thread based utilities.

    This module logically breaks down into three sections:
    - Thread based execution of code provided by fostlib::worker, fostlib::in_process and fostlib::workerpool.
    - Thread safe datastructures, so far fostlib::library.
    - Thread safe counter, so far fostlib::counter.

    To choose between the thread execution classes, normally application would not make use of fostlib::worker directly. fostlib::in_process is useful where a thread can be represented by an object. Both synchronous and asynchronous messages can be passed to the object. Where a pool is required then fostlib::workerpool should be used.
*/


/** \class fostlib::future fost/thread.hpp fost/threading
    \ingroup fost_core_threading

    The use of futures makes many multi-threaded problems much easier to reason about, but there is a still a chance of deadlocks occuring. This is possible when code that takes a future as a parameter itself gets used as a future. So long as the call graph is an acyclic tree (a DAG, or directed acyclic graph) then there is no possibility of a deadlock. When a cycle is introduced into the graph however then a dead lock might occur.
*/


/** \class fostlib::worker fost/thread.hpp fost/threading
    \ingroup fost_core_threading

    Generally this class is not used directly. For work that is to be done by instances of an object then the fostlib::in_process class is usually a better choice. Where a pool of workers is required then the fostlib::workerpool should be used -- see \ref fost_core_threading.
*/


/** \class fostlib::in_process fost/thread.hpp fost/threading
    \ingroup fost_core_threading
    \param O The type of the object that is to be used in the remote thread.

    Instances of this class are used to communicate with remote objects which in turn represent remote threads. The threads are within the same process that they are created in, hence the name.

    Ownership of the remote object is always by the in_process instance. When the in_process dies it will delete the object instance.
*/
/** \fn fostlib::in_process< O >::in_process (std::function<O *(void)> c)
    \param c A lambda that returns a pointer to a new object of type O.

    The lambda is executed in the remote so can be used where the remote object has strict thread affinity.
*/


/** \class fostlib::threadsafe_store fost/thread.hpp fost/threading
    \ingroup fost_core_threading
    \param F The type of the items that the store manages access to.
    \param K The type of the key used to fetch the items.
    \param S The type of the underlying store. This is generaly a std::multimap.

    The data store works on value types. If objects are to be used and the data store needs to manage their lifetimes then the item type (F) should be a std::shared_ptr to the object instances.

    Because a std::multimap is used multiple items can appear at the same key.

    Some care needs to be taken when registering objects that are global within shared objects. Because the order of initialisation may not be well defined it is normally best to not remove them from the store when the shared object is unloaded. This is because the destructors in the shared object may run later than the destructor for the library that they are registered in. If this happens the program will crash on exit.

    \sa
    - fostlib::dynlib
*/
/** \fn fostlib::threadsafe_store< F, K, S >::add (const key_t &k, const item_t &f)
    \param k The key to add the item to.
    \param f The item that is to be stored.

    Stores a copy of the item against the key provided.
*/
