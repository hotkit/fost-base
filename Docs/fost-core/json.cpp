/** \defgroup fost_core_json JSON
    \ingroup fost_core

    \brief A JSON parser and printer together with suitable data types to handle
   JSON objects, arrays and atoms and locations within JSON structures.

    JSON is at the heart of much of the Fost infrastructure. It provides a
   flexible mechanism for run time structured data that is in turn compatible
   with the way that many other languages also store data.

    The \ref fost_core_settings "settings database" uses JSON as it's internal
   representation.
*/
/** \class fostlib::json fost/json.hpp fost/core
    \ingroup fost_core_json

    \brief fostlib::json allows for the storage and manipulation of JSON objects.

    JSON objects can contain any of the following types:
    - fostlib::variant -- Used for atomic types
        - fostlib::null
        - bool
        - int64_t
        - double
        - fostlib::string
    - fostlib::json::object_t -- A "dictionary" of key value pairs keyed by
   fostlib::string
    - fostlib::json::array_t -- An array indexed by std::size_t

    The fostlib::json datastructure is a <a
   href="http://en.wikipedia.org/wiki/Persistent_data_structure">persistent data
   structure</a>. That is, the structure itself is immutable. In order to change
   the structure a new structure must be created. This can be done efficiently
   through the use of the mutating helper functions in fostlib::jcursor.

    \sa
    - fostlib::jcursor
    - fostlib::variant
    - http://en.wikipedia.org/wiki/Persistent_data_structure
*/
/** \class fostlib::jcursor fost/json.hpp fost/core
    \ingroup fost_core_json

    \brief fostlib::jcursor allows a location within a \ref fost_core_json
   "JSON" structure to be specified. It also allows JSON objects to be mutated.

    Positions within a JSON datastructure correspond to the numeric or string
   indices into the object and array structures within a JSON structure.
*/
