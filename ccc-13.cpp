

/*
    Sequence Containers: STL containers that provide sequential member access
*/
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <array>

/*
    Arrays: RAII wrapper around statically-sized arrays.
*/
std::array<int, 10> static_array; // static duration -> initialize ints to 0's
TEST_CASE("std::array") {
    REQUIRE(static_array[0] == 0);

    SECTION("uninitialized without braced initializers") {
        std::array<int, 10> local_array; // automatic storage duration -> initialize ints to unitialized elemnts (not necessarily 0)
        REQUIRE(local_array[0] != 0);
    }

    SECTION("initialized with braced initializers") {
        std::array<int, 10> local_array { 1, 1, 2, 3 };
        REQUIRE(local_array[0] == 1);
        REQUIRE(local_array[2] == 2);
        REQUIRE(local_array[4] == 0);
    }

    SECTION("retrieval includes bounds checking depending on version") {
        std::array<int, 10> local_array;
        REQUIRE_NOTHROW(local_array[12]);
        REQUIRE_THROWS_AS(local_array.at(12), std::out_of_range);
        // std::get<12>(local_array) fails at compile time due to out of bounds
    }

    SECTION("std::array also provides front() and back() methods") {
        std::array<int, 4> kris_kross { 2, 4, 8, 16 };
        REQUIRE(kris_kross.front() == 2);
        REQUIRE(kris_kross.back() == 16);
    }

    SECTION("data() method provides pointer to first element") {
        std::array<char, 9> color{ 'o',  'c', 't', 'a', 'r', 'i', 'n', 'e' };
        const auto* color_ptr = color.data();

        REQUIRE(*color_ptr == 'o');
        REQUIRE(&color.front() == color_ptr);
        REQUIRE(&color.at(0) == color_ptr);
        REQUIRE(&color[0] == color_ptr);
    }

    /*
        Iterators: interface between container and algorithms that provides simple, pointer-like methods
        for iterating over the data
            - begin(): creates a pointer initialliy pointing to first element in structure
            - end(): creates a pointer to the block *after* the last element, creating a "half-open range"
    */
    SECTION("std::array begin/end form a half-open range") {
        std::array<int, 0> e{};
        REQUIRE(e.begin() == e.end());
    }

    SECTION("std::array iterators act as pointers") {
        std::array<int, 3> easy_as { 1, 2, 3 };

        auto iter = easy_as.begin();
        REQUIRE(*iter == 1);
        iter++;
        REQUIRE(*iter == 2);
        iter++;
        REQUIRE(*iter == 3);
        iter++;

        REQUIRE(iter == easy_as.end());
    }

    SECTION("std::array can be used in range expressions") {
        std::array<int, 5> nums { 1, 1, 2, 3, 5 };

        int sum { 0 };
        for(const auto num : nums) {
            sum += num;
        }

        REQUIRE(sum == 12);
    }
}

/*
    Vector: dynamically sized sequential containers
*/