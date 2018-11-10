## We're not going to build this if we're on Android
if("${ANDROID_ABI}" STREQUAL "")
    add_library(fost-cli
            arguments.cpp
            inifile.cpp
            logger-stdio.cpp
            main_exec.cpp
            progress-bar.cpp
        )
    target_link_libraries(fost-cli fost-core)
    set_target_properties(fost-cli PROPERTIES DEBUG_POSTFIX "-d")
    install(TARGETS fost-cli LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

    if(TARGET check)
        add_library(fost-cli-smoke STATIC EXCLUDE_FROM_ALL
                arguments-tests.cpp
                progress-bar-tests.cpp
            )
        target_link_libraries(fost-cli-smoke fost-cli)
        smoke_test(fost-cli-smoke)
    endif()
endif()
