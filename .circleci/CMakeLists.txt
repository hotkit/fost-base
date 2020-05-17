cmake_minimum_required(VERSION 3.10)
project(fost-base-circleci)

## Opt in to tests
add_custom_target(check)

## Add folders
add_subdirectory(fost-boost)
add_subdirectory(crypto)
add_subdirectory(cord)
add_subdirectory(threading)
add_subdirectory(.. fost-base)
add_subdirectory(../Examples fost-base/Examples)
