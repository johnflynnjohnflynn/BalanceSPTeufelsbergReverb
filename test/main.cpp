
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define gsl_CONFIG_CONTRACT_VIOLATION_THROWS 1

#include "Judio/Utility.h"

//--------//--------//--------//--------//--------//--------//--------//--------

TEST_CASE("nextPowerOf2()", "Utility") {
    int x = 55;
    REQUIRE(jdo::nextPowerOf2(x) == 64);
    x = -234;
    REQUIRE(jdo::nextPowerOf2(x) == 1);
    x = 16300;
    REQUIRE(jdo::nextPowerOf2(x) == 16384);
}
