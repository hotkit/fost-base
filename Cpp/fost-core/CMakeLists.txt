find_package(Threads REQUIRED)

add_library(fost-core
        atexit.cpp
        base16.cpp
        base32.cpp
        coerce.cpp
        counter.cpp
        date.cpp
        dynlib.cpp
        exception.cpp
        file.cpp
        jcursor.cpp
        json.cpp
        json-coerce.cpp
        json-order.cpp
        json-parse.cpp
        json-unparse.cpp
        log.cpp
        log-global.cpp
        log-impl.cpp
        log-scoped.cpp
        meter.cpp
        module.cpp
        pool.cpp
        progress.cpp
        setting.cpp
        string.cpp
        string-ascii.cpp
        string-tagged.cpp
        string-utilities.cpp
        thread.cpp
        time.cpp
        timediff.cpp
        timestamp.cpp
        timestamp-rfc1123.cpp
        timezone.cpp
        unicode.cpp
        variant.cpp
        variant-coerce.cpp
        yaml.cpp
    )
target_include_directories(fost-core PUBLIC ../include)
target_compile_options(fost-core PUBLIC -fdiagnostics-color)
if("${toolset}" STREQUAL "gcc")
    target_compile_options(fost-core PUBLIC -Wno-deprecated-declarations)
endif()
target_link_libraries(fost-core ${CMAKE_THREAD_LIBS_INIT} dl
    boost_date_time boost_filesystem boost_system
    boost f5-cord f5-threading)
set_target_properties(fost-core PROPERTIES DEBUG_POSTFIX "-d")
install(TARGETS fost-core LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

add_library(fost-core-smoke STATIC EXCLUDE_FROM_ALL
        base16.tests.cpp
        base32.tests.cpp
        boost-filesystem-path-tests.cpp
        char-encoding-tests.cpp
        coercion-int-tests.cpp
        coercion-tests.cpp
        date-tests.cpp
        exception-tests.cpp
        file-tests.cpp
        hex-tests.cpp
        jcursor-tests.cpp
        json-array-basic-tests.cpp
        json-atom-basic-tests.cpp
        json-coerce-tests.cpp
        json-iterator-tests.cpp
        json-jcursor-tests.cpp
        json-object-basic-tests.cpp
        json-order.tests.cpp
        json-parse-tests.cpp
        json-to-tests.cpp
        log-tests.cpp
        memoize-tests.cpp
        module.tests.cpp
        nullable.tests.cpp
        progress-tests.cpp
        push_back-tests.cpp
        string-tagged-tests.cpp
        string-tests.cpp
        string-utilities.tests.cpp
        timediff-tests.cpp
        timestamp-rfc1123-tests.cpp
        timestamp-tests.cpp
        unicode.tests.cpp
        variant-basic-tests.cpp
        yaml-tests.cpp
    )
target_link_libraries(fost-core-smoke fost-core)
smoke_test(fost-core-smoke)
