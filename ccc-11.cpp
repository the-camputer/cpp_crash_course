/*
 * Smart Pointers: RAII-patterned object pointers available in the stdlib and boost that guarantee that the pointer's
 *  lifetime will be at least as long as the object it is pointing to.
 *  Ownership models:
 *   1. Scoped: Non-transferable (cannot change scope), exclusive ownership (cannot be copied) of a single dynamic object.
 *   2. Unique: Transferable, exclusive ownership of a single dynamic object
 *   3. Shared: Transferable, non-exclusive (more than one owner possible) ownership over a single dynamic object
 *   4. Weak: Tracks object and can be converted to shared pointer if object still exists. Typically used in caches
 *   5. Intrusive
*/
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/smart_ptr/scoped_array.hpp>
#include <memory>

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


/*
 * BOOST SCOPED POINTERS
*/
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


/*
 * UNIQUE POINTERS
 * note: use std::make_unique instead of new
*/
using UniqueOathBreakers = std::unique_ptr<DeadMenOfDunharrow>;
TEST_CASE("UNIQUE POINTERS: ") {
    SECTION("UniquePtr's can be used in move") {
        auto aragorn = std::make_unique<DeadMenOfDunharrow>();
        SECTION("construction") {
            auto son_of_arathorn { std::move(aragorn) };
            REQUIRE(DeadMenOfDunharrow::oaths_to_fufill == 1); // aragorn obj moved to son_of_arathorn
        }

        SECTION("assignment") {
            auto son_of_arathorn = std::make_unique<DeadMenOfDunharrow>(); // create new "son of arathorn" separate from aragorn
            REQUIRE(DeadMenOfDunharrow::oaths_to_fufill == 2);
            son_of_arathorn = std::move(aragorn); // move aragorn to son_of_arathorn, thus removing whatever assignment was originally there
            REQUIRE(DeadMenOfDunharrow::oaths_to_fufill == 1);
        }
    }

    SECTION("UniquePtr to array supports operator[]") {
        /*
         * NOTE! Don't initialize unique_ptr<T> with dynamic array T[] a la the following
         * std::unique_ptr<int> dont_do_this { new int[5] {0, 0, 0, 0, 0} }
        */
        std::unique_ptr<int[]> squares {
            new int[5] { 0, 4 ,9, 16, 25 }
        };

        squares[0] = 1;
        REQUIRE(squares[0] == 1);
        REQUIRE(squares[1] == 4);
        REQUIRE(squares[2] == 9);
    }

    /*
     * Note: Custom deleters are needed when regular delete won't release necessary resources, 
     * e.g. low level file system operations
    */
    SECTION("UniquePtr supports using custom deleters") {
        auto deleted_via_custom = false;

        auto custom_deleter = [&deleted_via_custom](int* x) {
            deleted_via_custom = true;
            delete x;
        };

        std::unique_ptr<int, decltype(custom_deleter)> kill_int { new int{5}, custom_deleter };

        kill_int.reset();

        REQUIRE(deleted_via_custom);
    }
}

/*
    SHARED POINTERS
    note: use std::make_shared instead of new
    note: supports shared_array and deleters
*/
using SharedOathBreakser = std::shared_ptr<DeadMenOfDunharrow>;
TEST_CASE("SHARED POINTERS: ") {
    SECTION("SharedPtr can be used in copy") {
        auto aragorn = std::make_shared<DeadMenOfDunharrow>();
        SECTION("construction") {
            auto son_of_arathorn { aragorn };
            REQUIRE(DeadMenOfDunharrow::oaths_to_fufill == 1);
        }
        SECTION("assignment") {
            SharedOathBreakser son_of_arathorn;
            son_of_arathorn = aragorn;
            REQUIRE(DeadMenOfDunharrow::oaths_to_fufill == 1);
        }
        SECTION("assignment, and original gets discarded") {
            auto son_of_arathorn = std::make_shared<DeadMenOfDunharrow>();
            REQUIRE(DeadMenOfDunharrow::oaths_to_fufill == 2);
            son_of_arathorn = aragorn;
            REQUIRE(DeadMenOfDunharrow::oaths_to_fufill == 1);
        }
    }
}

/*
    WEAK POINTERS
    note: constructed using a shared pointer or another weak pointer
*/
TEST_CASE("WEAK POINTERS: ") {
    SECTION("WeakPtr lock() yields") {
        auto message = "The way is shut.";
        SECTION("a shared pointer when tracked object is alive") {
            auto aragorn = std::make_shared<DeadMenOfDunharrow>(message);
            REQUIRE(DeadMenOfDunharrow::oaths_to_fufill == 1);

            std::weak_ptr<DeadMenOfDunharrow> legolas { aragorn };
            auto sh_ptr = legolas.lock();

            REQUIRE(sh_ptr->message == message);
            REQUIRE(DeadMenOfDunharrow::oaths_to_fufill == 2);
        }

        SECTION("empty when shared pointer is empty") {
            std::weak_ptr<DeadMenOfDunharrow> legolas;
            
            // block scope will cause aragorn to fall out of scope and dies
            {
                auto aragorn = std::make_shared<DeadMenOfDunharrow>(message);
                legolas = aragorn;
            }

            auto shared_ptr = legolas.lock();

            REQUIRE(shared_ptr == nullptr);
        }
    }
}