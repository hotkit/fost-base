if(TARGET stress)
    add_library(fost-core-test STATIC EXCLUDE_FROM_ALL
            dynlib.cpp
            settings.cpp
            thread.cpp
            workerpool.cpp
        )
    target_link_libraries(fost-core-test fost-core)
    stress_test(fost-core-test)

    add_executable(fost-core-test-fileio EXCLUDE_FROM_ALL
            file-io.cpp
        )
    target_link_libraries(fost-core-test-fileio fost-cli)
    add_custom_command(TARGET fost-core-test-fileio
            POST_BUILD_COMMAND fost-core-test-fileio)
    add_dependencies(stress fost-core-test-fileio)
    add_test(NAME fost-core-test-fileio COMMAND fost-core-test-fileio)
endif()
