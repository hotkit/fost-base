add_executable(ftest ftest.cpp)
target_link_libraries(ftest fost-cli fost-test)
install(TARGETS ftest EXPORT ftest RUNTIME DESTINATION bin)
