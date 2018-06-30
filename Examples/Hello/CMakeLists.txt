add_executable(hello-example EXCLUDE_FROM_ALL main.cpp)
target_link_libraries(hello-example fost-cli)
# install(TARGETS hello EXPORT hello RUNTIME DESTINATION bin)
add_dependencies(examples hello-example)
