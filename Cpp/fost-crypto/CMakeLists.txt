add_library(fost-crypto
        crypto.cpp
        digester.cpp
        ed25519.cpp
        hashes.cpp
        jwt.cpp
        nonce.cpp
        password.cpp
        rsa.cpp
        signature.cpp
        tweetnacl.cpp
    )
target_link_libraries(fost-crypto crypto++ fost-core)
set_target_properties(fost-crypto PROPERTIES DEBUG_POSTFIX "-d")
install(TARGETS fost-crypto LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

if(TARGET check)
    add_library(fost-crypto-smoke STATIC EXCLUDE_FROM_ALL
            crypto-tests.cpp
            digests-tests.cpp
            ed25519.tests.cpp
            hashes.tests.cpp
            jwt.tests.cpp
            password.tests.cpp
            rsa.tests.cpp
        )
    target_link_libraries(fost-crypto-smoke fost-crypto)
    smoke_test(fost-crypto-smoke)
endif()
