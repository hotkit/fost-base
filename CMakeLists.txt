project(fost-base)

function(smoke_test name)
    add_executable(${name}-test ../fost-test/boost-build-unit-test.cpp)
    target_link_libraries(${name}-test
        -Wl,--whole-archive ${name} -Wl,--no-whole-archive
        fost-test fost-cli)
    add_custom_command(TARGET ${name}-test
        POST_BUILD COMMAND ${name}-test)
endfunction()

add_compile_options(-fdiagnostics-color)
if("${toolset}" STREQUAL "gcc")
    add_compile_options(-Wno-deprecated-declarations)
endif()

add_subdirectory(Cpp/fost-core) # Put first so others can depend on it

add_subdirectory(Cpp/fost-cli)
add_subdirectory(Cpp/fost-crypto)
add_subdirectory(Cpp/fost-test)
