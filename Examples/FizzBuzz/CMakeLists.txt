add_executable(fizzbuzz EXCLUDE_FROM_ALL main.cpp)
target_link_libraries(fizzbuzz fost-cli)
# install(TARGETS fizzbuzz EXPORT fizzbuzz RUNTIME DESTINATION bin)
add_dependencies(examples fizzbuzz)
