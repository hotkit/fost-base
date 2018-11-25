add_executable(hash hash.cpp)
target_link_libraries(hash fost-cli fost-crypto)
install(TARGETS hash EXPORT hash RUNTIME DESTINATION bin)
