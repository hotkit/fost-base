add_library(fost-datetime
        timer.cpp
    )
target_link_libraries(fost-datetime fost-core)

add_library(fost-datetime-smoke EXCLUDE_FROM_ALL
        period-tests.cpp
        time_period.tests.cpp
    )
target_link_libraries(fost-datetime-smoke fost-datetime)
smoke_test(fost-datetime-smoke)
