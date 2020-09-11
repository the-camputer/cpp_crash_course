#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <boost/logic/tribool.hpp>
#include <optional>
#include <utility>

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

/*
    Optional: class template that contains a value that may/may not be present. Useful as the return type of a function
    that might fail instead of erroring out.
*/
struct TheMatrix {
    TheMatrix(int x) : iteration { x } {}
    const int iteration;
};

enum Pill { Red, Blue };

std::optional<TheMatrix> take(Pill pill) {
    if ( pill == Pill::Blue ) return TheMatrix { 6 };
    else return std::nullopt;
}

TEST_CASE("std::optional contains types") {
    if (auto matrix_opt = take(Pill::Blue)) {
        REQUIRE(matrix_opt->iteration == 6);
        auto &matrix = matrix_opt.value();
        REQUIRE(matrix.iteration == 6);
    } else {
        FAIL("The operational evaluated to false");
    }
}

TEST_CASE("std::optional can be empty") {
    auto matrix_opt = take(Pill::Red);
    if (matrix_opt) FAIL("The Matrix is not empty");
    REQUIRE_FALSE(matrix_opt.has_value());
}

/*
    Pair: 
*/