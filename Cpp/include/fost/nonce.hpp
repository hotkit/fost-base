/**
    Copyright 2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <fost/string>


namespace fostlib {


    /// Produce base64url nonces of various byte sizes
    base64_string nonce8b64u();
    base64_string nonce24b64u();
    base64_string nonce32b64u();


}
