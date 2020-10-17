/*
    Asynchonous Programming: Executing multiple tasks concurrentally using multiple threads
        - uses std::async from <future> header
        - Called with one of two launch policies:
            - std::launch::async - runtime creates new thread and executes code immediatey
            - std::lauch::deferred - runtime waits until task result is needed before executing (lazy evaluation)
        - returns an std::future object
        - signature: std::future<function return type> std::async([policy], func, Args&&... args)
        - requires -pthread compiler option to compile
*/
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <future>
#include <string>

using namespace std;

TEST_CASE("async valid() ensures that result is obtainable from future") {
    using namespace literals::string_literals;
    auto the_future = async([] { return "female"s; });
    REQUIRE(the_future.valid());
    auto result = the_future.get();
    REQUIRE(result == "female");
    REQUIRE_FALSE(the_future.valid());
}

TEST_CASE("async valid() defaults to false") {
    future<bool> fake_future;
    REQUIRE_FALSE(fake_future.valid());
}