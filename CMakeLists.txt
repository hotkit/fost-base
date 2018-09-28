project(fost-base)

set(CMAKE_INSTALL_PREFIX ../dist
    CACHE PATH "Install location")
set(BUILD_SHARED_LIBS ON
    CACHE BOOL "Build shared libraries")

set(SMOKE_TEST_RUNNER_CPP
    ${CMAKE_CURRENT_SOURCE_DIR}/Cpp/fost-test/boost-build-unit-test.cpp
    CACHE INTERNAL "Unit test runner main.cpp")

function(smoke_test name)
    add_executable(${name}-check EXCLUDE_FROM_ALL
        ${SMOKE_TEST_RUNNER_CPP})
    if(APPLE)
        target_link_libraries(${name}-check
            -Wl,-all_load ${name} -Wl,-noall_load
            fost-test fost-cli)
    else()
        target_link_libraries(${name}-check
            -Wl,--whole-archive ${name} -Wl,--no-whole-archive
            fost-test fost-cli)
    endif()
    add_custom_command(TARGET ${name}-check
        POST_BUILD COMMAND ${name}-check -b false -v false)
    add_dependencies(check ${name}-check)
    add_test(NAME ${name}-ctest COMMAND ${name}-check)
endfunction()

add_custom_target(examples)
set_property(TARGET examples PROPERTY EXCLUDE_FROM_ALL TRUE)

add_custom_target(stress)
set_property(TARGET stress PROPERTY EXCLUDE_FROM_ALL TRUE)
function(stress_test name)
    add_executable(${name}-check EXCLUDE_FROM_ALL
        ${SMOKE_TEST_RUNNER_CPP})
    if(APPLE)
        target_link_libraries(${name}-check
            -Wl,-all_load ${name} -Wl,-noall_load
            fost-test fost-cli)
    else()
        target_link_libraries(${name}-check
            -Wl,--whole-archive ${name} -Wl,--no-whole-archive
            fost-test fost-cli)
    endif()
    add_custom_command(TARGET ${name}-check
        POST_BUILD COMMAND ${name}-check -b false -v false)
    add_dependencies(stress ${name}-check)
    add_test(NAME ${name}-ctest COMMAND ${name}-check)
endfunction()

add_subdirectory(Cpp)
add_subdirectory(Examples)
