#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <boost/logic/tribool.hpp>

/*
    tribool: can be either true, false, or indeterminate. Good for checking intermediate state of system.
*/
using boost::logic::indeterminate;
using tri = boost::logic::tribool;
template <typename T>
tri greater_than(T a, T b) {
    tri result = indeterminate;
    if (a > b) result = true;
    else if (a < b) result = false;

    return result;
}

TEST_CASE("Boost tribool converts to bool") {
    REQUIRE(greater_than(12, 6));
    REQUIRE_FALSE(greater_than(6, 12));
    REQUIRE(indeterminate(greater_than(6, 6)));
}