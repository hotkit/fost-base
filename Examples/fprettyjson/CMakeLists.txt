add_executable(fprettyjson fprettyjson.cpp)
target_link_libraries(fprettyjson fost-cli)
install(TARGETS fprettyjson EXPORT fprettyjson RUNTIME DESTINATION bin)
