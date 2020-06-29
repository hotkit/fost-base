/**
    Copyright 2015-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <string>

#include <crypto++/config.h>
#include <crypto++/osrng.h>
#include <crypto++/rsa.h>

#include "fost-crypto-test.hpp"
#include <fost/crypto>
#include <fost/rsa.hpp>


namespace {
    // Following test parameter come from https://jwt.io/ when change algorithm
    // to RS256 Web site will show example JWTs with public and private key in
    // PEM format Convert private key to JWK format at
    // https://irrte.ch/jwt-js-decode/pem2jwk.html And then get parameter n
    // (modulus), e (public exponent), d (private exponent) for generate key

    // modulus
    const std::string base64url_n =
            "nzyis1ZjfNB0bBgKFMSvvkTtwlvBsaJq7S5wA-kzeVOVpVWwkWdVha4s38XM_pa_"
            "yr47av7-z3VTmvDRyAHcaT92whREFpLv9cj5lTeJSibyr_Mrm_"
            "YtjCZVWgaOYIhwrXwKLqPr_"
            "11inWsAkfIytvHWTxZYEcXLgAXFuUuaS3uF9gEiNQwzGTU1v0FqkqTBr4B8nW3HCN4"
            "7XUu0t8Y0e-lf4s4OxQawWD79J9_"
            "5d3Ry0vbV3Am1FtGJiJvOwRsIfVChDpYStTcHTCMqtvWbV6L11BWkpzGXSW4Hv43qa"
            "-GSYOD2QU68Mb59oSk2OB-BtOLpJofmbGEGgvmwyCI9Mw";
    // public exponent
    const std::string base64url_e = "AQAB";
    // private exponent
    const std::string base64url_d =
            "KIBGrbCSW2O1yOyQW9nvDUkA5EdsS58Q7US7bvM4iWpuDIBwCXur7_VuKnhn_"
            "HUhURLzj_JNozynSChqYyG-CvL-ZLy82LUE3ZIBkSdv_vFLFt-"
            "VvvRtf1EcsmoqenkZl7aN7HD7DJeXBoz5tyVQKuH17WW0fsi9StGtCcUl-"
            "H6KzV9Gif0Kj0uLQbCg3THRvKuueBTwCTdjoP0PwaNADgSWb3hJPeLMm_"
            "yII4tIMGbOw-xd9wJRl-ZN9nkNtQMxszFGdKjedB6goYLQuP0WRZx-"
            "YtykaVJdM75bDUvsQar49Pc21Fp7UVk_CN11DX_hX3TmTJAUtqYADliVKkTbCQ";
}

FSL_TEST_SUITE(rsa);


FSL_TEST_FUNCTION(sign_jwt) {
    // message JWT {header}.{payload}
    const std::string base64url_jwt =
            "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9."
            "eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOnRydW"
            "UsImlhdCI6MTUxNjIzOTAyMn0";
    // signature from JWTs
    const std::string base64url_signature =
            "POstGetfAytaZS82wHcjoTyoqhMyxXiWdR7Nn7A29DNSl0EiXLdwJ6xC6AfgZWF1bO"
            "sS_TuYI3OG85AmiExREkrS6tDfTQ2B3WXlrr-wp5AokiRbz3_oB4OxG-"
            "W9KcEEbDRcZc0nH3L7LzYptiy1PtAylQGxHTWZXtGz4ht0bAecBgmpdgXMguEIcoqP"
            "J1n3pIWk_dUZegpqx0Lka21H6XxUTxiy8OcaarA8zdnPUnV6AmNP3ecFawIFYdvJB_"
            "cm-GvpCSbr8G8y_Mllj8f4x9nBH8pQux89_"
            "6gUY618iYv7tuPWBFfEbLxtF2pZS6YC1aSfLQxeNe8djT9YjpvRZA";
    FSL_CHECK_EQ(
            fostlib::rsa::PKCS1v15_SHA256::sign(
                    base64url_jwt, base64url_n, base64url_e, base64url_d),
            base64url_signature);
}


FSL_TEST_FUNCTION(validate_jwt_signature) {
    // message JWT {header}.{payload}
    const std::string base64url_jwt =
            "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9."
            "eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOnRydW"
            "UsImlhdCI6MTUxNjIzOTAyMn0";
    // signature from JWTs
    const std::string base64url_signature =
            "POstGetfAytaZS82wHcjoTyoqhMyxXiWdR7Nn7A29DNSl0EiXLdwJ6xC6AfgZWF1bO"
            "sS_TuYI3OG85AmiExREkrS6tDfTQ2B3WXlrr-wp5AokiRbz3_oB4OxG-"
            "W9KcEEbDRcZc0nH3L7LzYptiy1PtAylQGxHTWZXtGz4ht0bAecBgmpdgXMguEIcoqP"
            "J1n3pIWk_dUZegpqx0Lka21H6XxUTxiy8OcaarA8zdnPUnV6AmNP3ecFawIFYdvJB_"
            "cm-GvpCSbr8G8y_Mllj8f4x9nBH8pQux89_"
            "6gUY618iYv7tuPWBFfEbLxtF2pZS6YC1aSfLQxeNe8djT9YjpvRZA";
    FSL_CHECK(fostlib::rsa::PKCS1v15_SHA256::validate(
            base64url_jwt, base64url_signature, base64url_n, base64url_e));
}


FSL_TEST_FUNCTION(validate_invalid_jwt_signature) {
    // message JWT {header}.{payload}
    const std::string base64url_jwt =
            "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9."
            "eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOnRydW"
            "UsImlhdCI6MTUxNjIzOTAyMn0";
    // signature from JWTs
    const std::string base64url_signature =
            "QzDfUO_ihdaDOJ9nS1b4aLmWTj_pSaWN-F_"
            "r9yvBwzqvm6NCj5MQubLRczSSlYwLTHbC9YYm8weHbVIYGdTJ8bO-"
            "xeiipZwJUzULuz2zfUx4LzgZWEJOWO9bPD0cL_Kko_"
            "Z0Io48Kl4fF9tfUVfOGnU42FLlAwWAVeIFfsB9h5dZxymWfcYBB-"
            "ZqkTRJzqJbYQpiX5UcCkB4bXWfhSZGCIEEGoWvTeCkqNYn6_"
            "XEmrH8f7GVO9m3vTFEsIyLPwxdwmJMSsbHvqpETJo4XBzniyq3S8HdThOhyWiYvTin"
            "kNi6FhI2tpza4AVWqo1rDG3GxqerA8JyvLnBkYePY3om_g";
    FSL_CHECK(!fostlib::rsa::PKCS1v15_SHA256::validate(
            base64url_jwt, base64url_signature, base64url_n, base64url_e));
}
