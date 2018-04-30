add_library(fost-test
        test-main.cpp
        testsuite.cpp
    )
target_link_libraries(fost-test fost-core)
