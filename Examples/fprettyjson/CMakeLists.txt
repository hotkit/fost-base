add_executable(fprettyjson EXCLUDE_FROM_ALL fprettyjson.cpp)
target_link_libraries(fprettyjson fost-cli)
# install(TARGETS fprettyjson EXPORT fprettyjson RUNTIME DESTINATION bin)
add_dependencies(examples fprettyjson)
