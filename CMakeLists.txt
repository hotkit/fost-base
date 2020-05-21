cmake_minimum_required(VERSION 3.10)
project(fost-base)

set(CMAKE_INSTALL_PREFIX ../dist
    CACHE PATH "Install location")
set(BUILD_SHARED_LIBS ON
    CACHE BOOL "Build shared libraries")

include(./smoke_test.cmake)
include(./stress_test.cmake)
if(TARGET stress)
    set_property(TARGET stress PROPERTY EXCLUDE_FROM_ALL TRUE)
endif()

add_subdirectory(Cpp)
