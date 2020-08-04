#include <stdexcept>
#include <limits>
#include <cstdio>
#include <new>
#include <cstddef>


/*
    Operator Overloading: process of defining custom behavior for operators.
    Typically used on user-defined classes.
*/

struct CheckedInteger {

    const unsigned int value; // const means that CheckedInteger is immutable. This prevents changes during + operator

    CheckedInteger(unsigned int value) : value { value } {}

    /* 
        Overloading + operator:
            Allows for the addition of a regular unsigned int to a CheckedInteger and produce a resulting CheckedInteger
            with the correct value.
    */
    CheckedInteger operator+(unsigned int other) const {
        CheckedInteger result { value + other};

        if (result.value < value) throw std::overflow_error { "OVERFLOW!!!" };

        return result;
    }
};

int main() {
    CheckedInteger a { 100 };
    auto b = a + 200;
    printf("a + 200 = %u\n", b.value);
    try {
        auto c = a + std::numeric_limits<unsigned int>::max(); // numeric_limits in <limits> header able to get max or min of type
    } catch (const std::overflow_error& e) {
        printf("(a + max) EXCEPTION: %s\n", e.what());
    }
}