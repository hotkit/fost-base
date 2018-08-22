add_library(fost-test
        testsuite.cpp
    )
target_link_libraries(fost-test fost-core)
set_target_properties(fost-test PROPERTIES DEBUG_POSTFIX "-d")
install(TARGETS fost-test LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

