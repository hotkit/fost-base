add_executable(hash EXCLUDE_FROM_ALL hash.cpp)
target_link_libraries(hash fost-cli fost-crypto)
# install(TARGETS hash EXPORT hash RUNTIME DESTINATION bin)
add_dependencies(examples hash)
