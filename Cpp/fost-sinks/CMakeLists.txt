add_library(fost-sinks SHARED
        panoptes.cpp
        panoptes.pathname.cpp
        sinks.cpp
    )
target_link_libraries(fost-sinks fost-core)
set_target_properties(fost-sinks PROPERTIES DEBUG_POSTFIX "-d")
install(TARGETS fost-sinks LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

add_library(fost-sinks-smoke STATIC EXCLUDE_FROM_ALL
        panoptes.pathname.tests.cpp
    )
target_link_libraries(fost-sinks-smoke fost-sinks)
smoke_test(fost-sinks-smoke)

