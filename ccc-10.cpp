/*
    CATCH2: Unit Testing Framework for C++

    The following #define statement need only be defined in a single file, and the #include provided in every test file
    for the tests to run. Note: it's actually recommended to define these 2 in an otherwise empty file, and then only bring
    the #include statement to your other test files.

    basic structure:
    TEST_CASE("test-name", "[tag1][tag2][...]") {
        REQUIRE(boolean expression)
    }

    BDD-style structure:
    SCENARIO("scenario-name", "[tag1][tag2][...]") {
        GIVEN("given-statement") {
            // setup
            
            WHEN("when-statement") {
                THEN("then-statement") {
                    REQUIRE(boolean expression)
                }
            }
        }
    }
*/
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <exception>

// Yes, using an unsigned int here may be more preferred, but I want to show error testing.
int Factorial(int number) {
    if (number < 0) throw std::range_error{"Given number too low!"};
    return number <= 1 ? 1 : Factorial(number-1)*number;
}

TEST_CASE("Factorials are computed", "[factorial]") {
    REQUIRE( Factorial(0) == 1);
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}

SCENARIO("BDD-style factorials", "[factorial]") {
    GIVEN("Negative Numbers") {
        WHEN("I pass in a negative number") {
            THEN("An Error is thrown") { 
                REQUIRE_THROWS( Factorial(-1) );
            }
        }
    }
}