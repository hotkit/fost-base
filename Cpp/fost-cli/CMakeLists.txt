add_library(fost-cli
        arguments.cpp
        inifile.cpp
        logger-stdio.cpp
        main_exec.cpp
        progress-bar.cpp
    )
target_link_libraries(fost-cli fost-core)

add_library(fost-cli-smoke EXCLUDE_FROM_ALL
        arguments-tests.cpp
        progress-bar-tests.cpp
    )
target_link_libraries(fost-cli-smoke fost-cli)
smoke_test(fost-cli-smoke)
