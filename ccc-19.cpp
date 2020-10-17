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

TEST_CASE("future will throw any errors the async task throws") {
    auto explosion = async([] { throw runtime_error { "Boom." }; });
    REQUIRE_THROWS_AS(explosion.get(), runtime_error);
}

/*
    You can also wait for async tasks to finish using wait_for(Duration) or wait_until(time_point)
    result:
        - future_status::deferred - if the async task is deffered
        - future_status::ready - completed and results are ready
        - future_status::timeout - result not yet ready
*/
TEST_CASE("Waiting for a task to complete") {
    using namespace literals::chrono_literals;
    auto sleepy = async(launch::async, [] { this_thread::sleep_for(100ms); });
    const auto not_ready_yet = sleepy.wait_for(25ms);
    REQUIRE(not_ready_yet == future_status::timeout);
    const auto ready_now = sleepy.wait_for(100ms);
    REQUIRE(ready_now == future_status::ready);
}