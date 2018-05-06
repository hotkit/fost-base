add_executable(hello EXCLUDE_FROM_ALL main.cpp)
target_link_libraries(hello fost-cli)
# install(TARGETS hello EXPORT hello RUNTIME DESTINATION bin)
add_dependencies(examples hello)
