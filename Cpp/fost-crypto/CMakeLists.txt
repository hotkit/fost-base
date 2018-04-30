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
