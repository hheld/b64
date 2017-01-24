extern "C"
{
#include <b64.h>
}

#include <string>

#define BOOST_TEST_MODULE Encode

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(Encode_Padding1)
{
    std::string s("This is a test");

    char *c_b64 = b64_encode(s.c_str());
    std::string s_b64(c_b64);

    BOOST_CHECK(s_b64=="VGhpcyBpcyBhIHRlc3Q=");

    free(c_b64);
}


BOOST_AUTO_TEST_CASE(Encode_Padding2)
{
    std::string s("This is a test!!");

    char *c_b64 = b64_encode(s.c_str());
    std::string s_b64(c_b64);

    BOOST_CHECK(s_b64=="VGhpcyBpcyBhIHRlc3QhIQ==");

    free(c_b64);
}

BOOST_AUTO_TEST_CASE(Encode_Padding0)
{
    std::string s("This is a test!");

    char *c_b64 = b64_encode(s.c_str());
    std::string s_b64(c_b64);

    BOOST_CHECK(s_b64=="VGhpcyBpcyBhIHRlc3Qh");

    free(c_b64);
}


BOOST_AUTO_TEST_CASE(Encode_WithWindowsLineBreak)
{
    std::string s("This is a\r\ntest!");

    char *c_b64 = b64_encode(s.c_str());
    std::string s_b64(c_b64);

    BOOST_CHECK(s_b64=="VGhpcyBpcyBhDQp0ZXN0IQ==");

    free(c_b64);
}
