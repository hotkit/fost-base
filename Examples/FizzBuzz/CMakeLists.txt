add_executable(fizzbuzz main.cpp)
target_link_libraries(fizzbuzz fost-cli)
install(TARGETS fizzbuzz EXPORT fizzbuzz RUNTIME DESTINATION bin)
