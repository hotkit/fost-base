project(fost-base)

set(CMAKE_INSTALL_PREFIX ../dist
    CACHE PATH "Install location")
set(BUILD_SHARED_LIBS ON
    CACHE BOOL "Build shared libraries")

function(smoke_test name)
    add_executable(${name}-check EXCLUDE_FROM_ALL
        ../fost-test/boost-build-unit-test.cpp)
    target_link_libraries(${name}-check
        -Wl,--whole-archive ${name} -Wl,--no-whole-archive
        fost-test fost-cli)
    add_custom_command(TARGET ${name}-check
        POST_BUILD COMMAND ${name}-check)
    add_dependencies(check ${name}-check)
    add_test(NAME ${name}-ctest COMMAND ${name}-check)
endfunction()

add_compile_options(-fdiagnostics-color)
if("${toolset}" STREQUAL "gcc")
    add_compile_options(-Wno-deprecated-declarations)
endif()

add_subdirectory(Cpp/fost-core) # Put first so others can depend on it

add_subdirectory(Cpp/fost-cli)
add_subdirectory(Cpp/fost-crypto)
add_subdirectory(Cpp/fost-datetime)
add_subdirectory(Cpp/fost-ftest)
add_subdirectory(Cpp/fost-sinks)
add_subdirectory(Cpp/fost-test)

