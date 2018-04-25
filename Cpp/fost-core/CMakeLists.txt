find_library(uuid-lib uuid)

add_library(fost-core STATIC
        atexit.cpp
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
target_link_libraries(fost-core $(uuid-lib) boost f5-cord f5-threading)
