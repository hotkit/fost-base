add_library(fost-datetime
        timer.cpp
    )
target_link_libraries(fost-datetime fost-core)
set_target_properties(fost-datetime PROPERTIES DEBUG_POSTFIX "-d")
install(TARGETS fost-datetime LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

if(TARGET check)
    add_library(fost-datetime-smoke STATIC EXCLUDE_FROM_ALL
            period-tests.cpp
            time_period.tests.cpp
        )
    target_link_libraries(fost-datetime-smoke fost-datetime)
    smoke_test(fost-datetime-smoke)
endif()
