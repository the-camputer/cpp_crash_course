/*
    Strings: <string> header provides types for human readable strings that also provide major safety and
    feature improvements over C-style or null-terminated strings.

    std::basic_string is a class template that forms the baseline string concept and can be specialized upon.

    Four provided implementations of std::basic_string:
        - std::string -> for char, is used for small character sets like ASCII/UTF-8
        - std::wstring -> for wchar_t, large enough to contain largest character for the implementation's locale
        - std::u16string -> for char16_t, used for character sets like UTF-16
        - std::u32string -> for char32_t, used for character sets like UTF-32 
*/
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <string>

TEST_CASE("std::string") {
    SECTION("supports constructing") {
        SECTION("empty strings") {
            std::string cheese;
            REQUIRE(cheese.empty());
        }
        SECTION("repeated characters") {
            std::string roadside_assistance(3, 'A');
            REQUIRE(roadside_assistance == "AAA");
        }
        SECTION("from char* and substrings") {
            auto word = "gobbledygook";
            REQUIRE(std::string(word) == "gobbledygook");
            REQUIRE(std::string(word, 6) == "gobble");
        }
        SECTION("copy/move construction") {
            std::string word("catawampus");
            REQUIRE(std::string(word) == "catawampus"); // Note: copy constructors generally expensive due to dynamic memory, unless less than 24 bytes (small string optimization)
            REQUIRE(std::string(std::move(word)) == "catawampus");
        }
        SECTION("a string with") {
            SECTION("std::string(char*) stops at embedded nulls") {
                std::string str("idioglossia\0ellohay!");
                REQUIRE(str.length() == 11);
            }
            SECTION("operator\"\"s incoporates embedded nulls") {
                using namespace std::string_literals;
                auto litty_string_city = "idioglossia\0ellohay!"s;
                REQUIRE(litty_string_city.length() == 20);
            }
        }
    }

    SECTION("c_str method returns a C-style, null-terminated string") {
        std::string word("wow look a string ain't that neat");
        auto c_str_word = word.c_str();
        REQUIRE(c_str_word[0] == word[0]);
        REQUIRE(c_str_word[10] == word[10]);
        REQUIRE(c_str_word[word.length()] == '\0');
    }

    SECTION("supports appending with") {
        std::string example("butt");
        SECTION("push_back") {
            example.push_back('e');
            REQUIRE(example == "butte");
        }
        SECTION("operator+=") {
            example += "erfinger";
            REQUIRE(example == "butterfinger");
        }
        SECTION("append char") {
            example.append(1, 's');
            REQUIRE(example == "butts");
        }
        SECTION("append char*") {
            example.append("stockings", 5);
            REQUIRE(example == "buttstock");
        }
    }
    SECTION("supports removal with") {
        std::string word("therein");
        SECTION("pop_back") {
            word.pop_back();
            word.pop_back();
            REQUIRE(word == "there");
        }
        SECTION("clear") {
            word.clear();
            REQUIRE(word.empty());
        }
        SECTION("erase using half-open range") {
            word.erase(word.begin(), word.begin()+3);
            REQUIRE(word == "rein");
        }
        SECTION("erase using an index and length") {
            word.erase(5, 2);
            REQUIRE(word == "there");
        }
    }
    SECTION("string substr with") {
        std::string word("hobbits");
        SECTION("no arguments copies the string") {
            REQUIRE(word.substr() == "hobbits");
        }
        SECTION("position takes the remainder") {
            REQUIRE(word.substr(3) == "bits");
        }
        SECTION("position/index takes a substring") {
            REQUIRE(word.substr(3, 3) == "bit");
        }
    }

    using namespace std::literals::string_literals;
    SECTION("find") {
        std::string word("pizzazz");
        SECTION("locates substrings from strings") {
            REQUIRE(word.find("zz"s) == 2); // pi(z)zazz
        }
        SECTION("accepts a position argument") {
            REQUIRE(word.find("zz"s, 3) == 5); //pizza(z)z
        }
        SECTION("locates substring from char*") {
            REQUIRE(word.find("zaz") == 3); // piz(z)azz
        }
        SECTION("returns npos when not found") {
            REQUIRE(word.find("x") == std::string::npos);
        }
    }
}

TEST_CASE("STL String Conversion") {
    using namespace std::literals::string_literals;
    SECTION("to_string") {
        REQUIRE("542345"s == std::to_string(542345));
    }
    SECTION("to_wstring") {
        REQUIRE(L"12360359.584086"s == std::to_wstring(12360359.584086));
    }
    SECTION("stoi") {
        REQUIRE(std::stoi("8675309"s) == 8675309);
    }
    SECTION("stoi") {
        REQUIRE_THROWS_AS(std::stoi("1099511627776"s), std::out_of_range);
    }
    SECTION("stoul with all valid characters") {
        size_t last_character{};
        const auto result = std::stoul("0xD3C34C3D"s, &last_character, 16);
        REQUIRE(result == 0xD3C34C3D);
        REQUIRE(last_character == 10);
    }
    SECTION("stoul") {
        size_t last_character{};
        const auto result = std::stoul("42six"s, &last_character);
        REQUIRE(result == 42);
        REQUIRE(last_character == 2);
    }
    SECTION("stod") {
        REQUIRE(std::stod("2.7182818"s) == Approx(2.7182818));
    }
} 