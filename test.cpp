#define BOOST_TEST_MODULE ip_filter_test_module

#include <boost/test/unit_test.hpp>
#include "lib.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(ip_filter_test_suite)


BOOST_AUTO_TEST_CASE(ip_filter_test_output)

{
    BOOST_CHECK(output_hash_is_equal());
}


BOOST_AUTO_TEST_SUITE_END()



