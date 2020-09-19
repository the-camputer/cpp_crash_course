

/*
    Sequence Containers: STL containers that provide sequential member access
*/
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <array>
#include <vector>
#include <stack>
#include <queue>

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
    Vector: dynamically sized sequential containers. Is the workhorse of sequential data structures.
    Contains almost all of the same operations as arrays with greater flexibility.
*/
TEST_CASE("std::vector") {
    SECTION("vector support empty initialization") {
        std::vector<const char*> vec;
        REQUIRE(vec.empty());
    }

    SECTION("vector supports braced initialization") {
        std::vector<int> fib { 1, 1, 2, 3, 5 };
        REQUIRE(fib.size() == 5);
    }

    SECTION("vector has fill constructor for repetetive numbers") {
        std::vector<int> five_nine { 5, 9 }; // curly braces
        std::vector<int> five_nines( 5, 9 ); // parentheses
    
        REQUIRE(five_nine[0] == 5);
        REQUIRE(five_nines[4] == 9);
    }

    SECTION("vector supports construction from iterators") {
        std::array<int, 5> arr_nums { 1, 1, 2, 3, 5 };
        std::vector<int> vec_nums(arr_nums.begin(), arr_nums.end());

        REQUIRE(vec_nums.size() == arr_nums.size());
        REQUIRE(vec_nums[4] == arr_nums[4]);
    }

    SECTION("std::vector assign replaces existing elements") {
        std::vector<int> message{ 13, 80, 110, 114, 102, 110, 101 };
        REQUIRE(message.size() == 7);
        message.assign({ 67, 97, 101, 115, 97, 114 });
        REQUIRE(message[5] == 114);
        REQUIRE(message.size() == 6);
    }

    SECTION("vector insert places new elements") {
        std::vector<int> zeroes(3, 0);
        auto second_element = zeroes.begin() + 1;
        zeroes.insert(second_element, 10);
        REQUIRE(zeroes[1] == 10);
        REQUIRE(zeroes.size() == 4);
    }

    SECTION("vector push_back places new elements at end") {
        std::vector<int> zeroes(3, 0);
        zeroes.push_back(10);

        REQUIRE(zeroes.size() == 4);
        REQUIRE(zeroes[3] == 10);
    }
}

/*
    Container Adapters: wrappers around other STL containers that provides additional functionality or conforms
    the containers to specific definitions, i.e. stack, queue, and heap
*/
TEST_CASE("std::stack") {
    /*
        stack templates have two parameters: the type, and the type of underlying structure. The second is optional,
        and defaults to deque unless you specify vector
    */
    SECTION("stack supports push/pop/top operations") {
        std::vector<int> vec { 1, 3 };
        std::stack<int, decltype(vec)> easy_as(vec);
        REQUIRE(easy_as.top() == 3);
        easy_as.pop();
        easy_as.push(2);
        REQUIRE(easy_as.top() == 2);
        easy_as.pop();
        REQUIRE(easy_as.top() == 1);
        easy_as.pop();
        REQUIRE(easy_as.empty());
    }
}

TEST_CASE("std::queue supports push/pop/front/back") {
    std::deque<int> deq { 1, 2 };
    std::queue<int> easy_as(deq);

    REQUIRE(easy_as.front() == 1);
    REQUIRE(easy_as.back() == 2);

    easy_as.pop();
    easy_as.push(3);

    REQUIRE(easy_as.front() == 2);
    REQUIRE(easy_as.back() == 3);

    easy_as.pop();

    REQUIRE(easy_as.front() == 3);

    easy_as.pop();

    REQUIRE(easy_as.empty());
}

