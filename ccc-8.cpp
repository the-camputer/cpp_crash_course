#include <cstdio>
#include <cstdint>
#include <stdexcept>

// function definition: signature of the function
// Allows usage of function before declaration (i.e. implementation)
// free functions: non-member functions (not in a class or struct)
void randomize(uint32_t&);

// namespaces: prevents naming conflicts in larger projects by qualifying symbols
namespace AVeryUniqueNamespace {
    namespace WeCanEvenNestNamespaces {
        enum Color {
            Russet,
            Chartruce,
            Periwinkle
        };
    }
}

// type aliases (explained below) can also be used with templates to perform partial application (fixing the number
// of paramters in a template in order to produce a template with fewer arguments)
template<typename To, typename From>
struct NarrowCaster {
    To cast(From value) {
        const auto converted = static_cast<To>(value);
        const auto backward = static_cast<From>(converted);
        if (value != backward) throw std::runtime_error{"Narrowed!"};
        return converted;
    }
};

template<typename From>
using short_caster = NarrowCaster<short, From>;

/*
    Structured Bindings:
    The ability to unpack objects into their constituent parts. Works for any type whose non-static members are public.
    Unpacking occurs in top-down order. For example, useful for returning a success flag as well as data instead of
    throwing an error.
*/
struct TextFile {
    bool success;
    const char* data;
    size_t n_bytes;
};

TextFile read_text_file(const char* path) {
    const static char contents[] = {"Hey look a fake file how cool."};
    return TextFile {
        true, 
        contents, 
        sizeof(contents)
    };
}

struct Tracer {
    Tracer(const char* name) : name { name } {
        printf("%s constructed\n", name);
    }

    ~Tracer() {
        printf("%s deconstructed\n", name);
    }

    private:
    /*
        Overview of const usages:
        char *name: mutable pointer to mutable character
        const char* name: mutable pointer to a constant character
        char* const name: constant pointer to a mutable character
        const char* const name: constant pointer to a constant character
    */
    const char* const name;
};

/*
    Blocks: sections of compound statements enclosed by brackets. Provides a scope 
    for varible lifecyccles

    Variables within a block are destroyed in reverse order to their declarations.
*/
int main() {
    Tracer main { "MAIN!" };
    {
        printf("Block a!\n");
        Tracer a1 { "a1" };
        Tracer a2 { "a2" }; 
    }
    {
        printf("Block b!\n");
        Tracer b1 { "b1" };
        Tracer b2 { "b2" };
    }

    size_t iterations{};
    uint32_t number { 0x4c4347 };
    while (number != 0x474343) {
        randomize(number); // Notice: function used before declaration because defintion is earlier
        iterations++;
    }

    printf("It took %zd iterations\n", iterations);

    // use the scope-resolution operator to specify what you want within a namespace
    const auto color = AVeryUniqueNamespace::WeCanEvenNestNamespaces::Color::Periwinkle;
    // You can also use a "using" directive to reduce the need to expressly qualify namespaces
    // NOTE! Do not use a "using" directice in a header file. It dumps everthing into the global namespace and becomes a pain to debug.
    using AVeryUniqueNamespace::WeCanEvenNestNamespaces::Color;
    if (color == Color::Periwinkle) {
        printf("Yup. Periwinkle is a great color\n");
    }

    // type aliases: literally just a synonym for a specific type. Makes code more readable
    using String = const char[256];

    String saying {
        "Hey look at this "
        "I made a 'string' without actually making a string"
    };

    printf("saying: %s\n", saying);

    try {
        short_caster<int> caster;
        const auto cyclic_short = caster.cast(142857);
        printf("Cyclic short: %d\n", cyclic_short);
    } catch(std::runtime_error& e) {
        printf("Exception: %s\n", e.what());
    }

    // Structured Bindings example: TextFile POD is automatically unpacked into elements in left-to-right order from top-to-bottom
    const auto [success, data, length] = read_text_file("README.txt");
    if (success) {
        printf("Read %ld bytes: %s\n", length, data);
    } else {
        printf("Failed to read anything");
    }
}

// function declaration: the actual implementation of a function. Can be separate from definition
void randomize(uint32_t& x) {
    x = 0x3FFFFFFF & (0x41C64E6D * x + 12345) % 0x80000000; // a quick and dirty pseduo-random number generator
}