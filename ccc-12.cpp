#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <boost/logic/tribool.hpp>
#include <optional>
#include <utility>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <chrono>
#include <thread>
#include <cmath>
#include <boost/math/constants/constants.hpp>
#include <random>

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
    Pair: Template that contains two objects of potentially differing types within the same type
*/
struct Socialite { const char* birthname; };
struct Valet { const char* surname; };
Socialite Valentino { "Scaramucci" };
Valet Jimmy { "Johnson" };
TEST_CASE("std::pair permits access to members") {
    std::pair<Socialite, Valet> immutable_couple { Valentino, Jimmy };
    REQUIRE(immutable_couple.first.birthname == Valentino.birthname);
    REQUIRE(immutable_couple.second.surname == Jimmy.surname);
}

/*
    Tuple: Generalization of Pair to include an arbitrary number of elements
*/
struct Acquaintance { const char* nickname; };
Acquaintance Reginald { "Regi" };
TEST_CASE("std::tuple permists access to members with get()") {
    using Trio = std::tuple<Socialite, Valet, Acquaintance>;
    Trio three_musketeers { Valentino, Jimmy, Reginald };
    auto& regi_ref = std::get<2>(three_musketeers);
    REQUIRE(regi_ref.nickname == Reginald.nickname);

    // can use type if all types in tuple are different
    auto& jimmy_ref = std::get<Valet>(three_musketeers);
    REQUIRE(jimmy_ref.surname == Jimmy.surname);
}

/*
    Dates and Times:
        - Boost has a date type that is based on the Gregorian Calendar
        - Boost has a posix_time section that handles clocks with microsecond resolution
        - Boost has a time_zone_base section that time zone aware time programming
        - Stdlib's Chrono library offers utilities useful for time or timing-dependant code
*/
TEST_CASE("boost::gregorian::date") {
    using boost::gregorian::date;
    SECTION("Invalid dates throw exceptions") {
        using boost::gregorian::bad_day_of_month;

        REQUIRE_THROWS_AS(date(1986, 9, 32), bad_day_of_month);
    }

    // NOTE!! This requires you to link boost_date_time in your compilation to work.
    // SECTION("boost dates can be created from numbers or strings using from_string") {
    //     date d_from_numbers { 1990, 9, 15 };
    //     auto d_from_string = boost::gregorian::from_string("1990-09-15");

    //     REQUIRE(d_from_numbers == d_from_numbers);
    // }

    SECTION("boost date supports basic calendar functions") {
        date d{ 1986, 9, 15 };

        REQUIRE(d.year() == 1986);
        REQUIRE(d.month() == 9);
        REQUIRE(d.day() == 15);
        REQUIRE(d.day_of_year() == 258);
        REQUIRE(d.day_of_week() == boost::date_time::Monday);
    }

    SECTION("boost date supports calendar arithmetic") {
        date d1{ 1986, 9, 15 };
        date d2{ 2019, 8, 1 };
        auto duration = d2 - d1;

        REQUIRE(duration.days() == 12008);
    }
}

TEST_CASE("stdlib chrono librarby") {
    SECTION("std::chrono supports several clocks") {
        /*
            Types of Chrono clicks:
                - System clock: the standard, system-wide real-time clock
                - High Resolution clock: clock with the shortest tick
                - Steady clock: gauranteed not to go backwardwards somehow.
            
            Time Point: an encapsulated representation of a moment in time. Defines the amount of time that has
            passed since the beginning of the clock's epock (01/01/1970 for POSIX, 01/01/1601 for Windows)
            using its time_since_epoch() method, which returns a Duration between the given time and the epoch start

            Duration: represents the time between two time_point objects. Exposes a count() method which returns
            the number of ticks between the two points.
        */
        auto sys_now = std::chrono::system_clock::now();
        auto hi_res_now = std::chrono::high_resolution_clock::now();
        auto steady_now = std::chrono::steady_clock::now();

        REQUIRE(sys_now.time_since_epoch().count() > 0);
        REQUIRE(hi_res_now.time_since_epoch().count() > 0);
        REQUIRE(steady_now.time_since_epoch().count() > 0);
    }

    SECTION("std::chrono also provides literals for easier readability") {
        using namespace std::literals::chrono_literals;
        auto one_s = std::chrono::seconds(1);
        auto thousand_ms = 1000ms;

        REQUIRE(one_s == thousand_ms);
    }

    SECTION("std::chrono supports duration casting") {
        using namespace std::chrono;
        auto billion_ns_as_s = duration_cast<seconds>(1'000'000'000ns);

        REQUIRE(billion_ns_as_s.count() == 1);
    }

    SECTION("std::chrono can be used to sleep threads") {
        using namespace std::literals::chrono_literals;

        auto start = std::chrono::system_clock::now();
        std::this_thread::sleep_for(100ms);
        auto end = std::chrono::system_clock::now();

        REQUIRE(end - start >= 100ms);
    }
}

/*
    Math: <cmath> header contains just a metric crap ton of useful methematical functions.
    In addition, Boost has a mathematical constants sections that makes some code easier to read
*/

TEST_CASE("Evaluation of cmath and Boost mathematical constants") {
    using namespace boost::math::double_constants;

    auto sphere_volume = four_thirds_pi * std::pow(10, 3);

    REQUIRE(sphere_volume == Approx(4188.7902047)); //Approx is a catch function 
}

/*
    Random Numbers: Boost and stdlib provide a variety of random number generaters based on needs
        - pseudorandom: provides a reproducable series of seemingly random numbers based on a seed
        - cryptographically random: closer to true random number generation
*/
TEST_CASE("Random Number Generation Engines") {
    SECTION("mt19937_64 is psedorandom") {
        std::mt19937_64 mt_engine { 91586 };

        REQUIRE(mt_engine() == 8346843996631475880);
        REQUIRE(mt_engine() == 2237671392849523263);
        REQUIRE(mt_engine() == 7333164488732543658);
    }

    SECTION("std::random_device is invocable") {
        std::random_device rd_engine{};

        REQUIRE_NOTHROW(rd_engine());
    }
}