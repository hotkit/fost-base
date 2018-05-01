add_library(fost-crypto
        crypto.cpp
        digester.cpp
        ed25519.cpp
        hashes.cpp
        jwt.cpp
        password.cpp
        signature.cpp
        tweetnacl.cpp
    )
target_link_libraries(fost-crypto crypto++ fost-core)

add_library(fost-crypto-smoke EXCLUDE_FROM_ALL
        crypto-tests.cpp
        digests-tests.cpp
        ed25519.tests.cpp
        jwt.tests.cpp
        password.tests.cpp
    )
target_link_libraries(fost-crypto-smoke fost-crypto)
smoke_test(fost-crypto-smoke)
