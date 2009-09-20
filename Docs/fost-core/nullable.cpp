/** \defgroup fost_core_nullable nullable
    \ingroup fost_core

    \brief Handles optional values.

    Much as our lives as developers would be simpler if we could always rely on all information being present, this is sadly not the case with the software systems we actually have to build.
*/


/** \enum fostlib::t_null
    \ingroup fost_core_nullable

    \brief The type of the fostlib::null instance.

    This enumration is the type of the empty instance which might represent some part of bottom, _|_
*/
/** \var fostlib::null
    \ingroup fost_core_nullable

    \brief The null value.
*/


/** \class fostlib::nullable
    \ingroup fost_core_nullable
    \param T The underlying value type that is to be made optional.

    \brief Allows for optional values to be stored and handled in a type safe manner.

    The underlying type must be a value type and must support default construction and assignment.
*/
