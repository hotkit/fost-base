set(STRESS_TEST_RUNNER_CPP
    ${CMAKE_CURRENT_LIST_DIR}/Cpp/fost-test/boost-build-unit-test.cpp
    CACHE INTERNAL "Unit test runner main.cpp for stress tests")

function(stress_test name)
    add_executable(${name}-check EXCLUDE_FROM_ALL
        ${STRESS_TEST_RUNNER_CPP})
    if(APPLE)
        target_link_libraries(${name}-check
            -Wl,-force_load ${name}
            fost-test fost-cli)
    else()
        target_link_libraries(${name}-check
            -Wl,--whole-archive ${name} -Wl,--no-whole-archive
            fost-test fost-cli)
    endif()
    add_custom_command(TARGET ${name}-check
        POST_BUILD COMMAND ${name}-check -b false -v false)
    if(TARGET stress)
        add_dependencies(stress ${name}-check)
    endif()
    add_test(NAME ${name}-ctest COMMAND ${name}-check)
endfunction()
