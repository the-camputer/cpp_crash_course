/*
 * Smart Pointers: RAII-patterned object pointers available in the stdlib and boost that guarantee that the pointer's
 *  lifetime will be at least as long as the object it is pointing to.
 *  Ownership models:
 *   1. Scoped: Non-transferable (cannot change scope), exclusive ownership (cannot be copied) of a single dynamic object.
 *   2. Unique: 
 *   3. Shared
 *   4. Weak
 *   5. Intrusive
*/
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/smart_ptr/scoped_array.hpp>

/*
 * BOOST SCOPED POINTERS
*/
struct DeadMenOfDunharrow {
    DeadMenOfDunharrow(const char* m="")
        : message { m } {
        oaths_to_fufill++;
    }

    ~DeadMenOfDunharrow() {
        oaths_to_fufill--;
    }

    const char* message;
    static int oaths_to_fufill;
};
int DeadMenOfDunharrow::oaths_to_fufill{};
using ScopedOathBreakers = boost::scoped_ptr<DeadMenOfDunharrow>;

TEST_CASE("SCOPED POINTERS: ") {
    SECTION("ScopedPtr evaluates to") {
        SECTION("true when full") {
            ScopedOathBreakers aragorn { new DeadMenOfDunharrow{} };
            REQUIRE(aragorn);
        }

        SECTION("false when empty") {
            ScopedOathBreakers aragorn;
            REQUIRE_FALSE(aragorn);
        }
    }

    SECTION("ScopedPtr is an RAII wrapper") {
        REQUIRE(DeadMenOfDunharrow::oaths_to_fufill == 0);
        ScopedOathBreakers aragorn { new DeadMenOfDunharrow{} };
        REQUIRE(DeadMenOfDunharrow::oaths_to_fufill == 1);
        // Curly braces denote a new scope --> ScopedPtr should kill all instances after leaving scope
        {
            ScopedOathBreakers legolas { new DeadMenOfDunharrow{} };
            REQUIRE(DeadMenOfDunharrow::oaths_to_fufill == 2);
        }
        REQUIRE(DeadMenOfDunharrow::oaths_to_fufill == 1);
    }

    SECTION("ScopedPtr supports pointer semantics, like") {
        auto message { "The way is shut." };
        ScopedOathBreakers aragorn{ new DeadMenOfDunharrow { message } };
        SECTION("operator*") {
            REQUIRE((*aragorn).message == message);
        }
        SECTION("operator->") {
            REQUIRE(aragorn->message == message);
        }
        SECTION("get(), which returns a raw pointer") {
            REQUIRE(aragorn.get() != nullptr);
        }
    }

    SECTION("ScopedPtr supports comparison to nullptr") {
        SECTION("operator==") {
            ScopedOathBreakers legolas;
            REQUIRE(legolas == nullptr);
        }
        SECTION("operator!=") {
            ScopedOathBreakers aragorn { new DeadMenOfDunharrow{} };
            REQUIRE(aragorn != nullptr);
        }
    }

    SECTION("ScopedPtr supports object swapping") {
        auto message1 { "The way is shut." };
        auto message2 { "Until the time comes." };

        ScopedOathBreakers aragorn{
            new DeadMenOfDunharrow { message1 }
        };

        ScopedOathBreakers legolas{
            new DeadMenOfDunharrow { message2 }
        };

        aragorn.swap(legolas);

        REQUIRE(aragorn->message == message2);
        REQUIRE(legolas->message == message1);
    }

    SECTION("ScopedArray supports operator[]") {
        boost::scoped_array<int> squares {
            new int[5] { 0, 4, 9, 16 ,25 }
        };
        squares[0] = 1;
        REQUIRE(squares[0] == 1);
        REQUIRE(squares[1] == 4);
        REQUIRE(squares[2] == 9);
    }
}


