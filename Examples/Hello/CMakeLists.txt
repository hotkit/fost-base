add_executable(hello-example main.cpp)
target_link_libraries(hello-example fost-cli)
install(TARGETS hello-example EXPORT hello-example RUNTIME DESTINATION bin)
