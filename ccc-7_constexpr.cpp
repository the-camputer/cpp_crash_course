#include <cstdint>
#include <cstdio>
#include <string>
#include <cstring>

/*
    Constant Expressions:
    Expressions that can be evaluated at compile time, providing better performance and security.
    These are used mainly to remove manually calculated values and improve readability by eliminating magic numbers.
    For example, writing constexpr's to handle the conversion of RGB values to HSV, removing the need to manually calculate them.

    Note: The only functions that constexpr's are allowed to call are other constexpr's.
*/
struct Color {
    float H; // Base color
    float S; // How much light to add. higher sat --> less light (i.e. more 'pure' color)
    float V; // How much dark to add. higher value --> more dark (i.e. darker shade)
};

// Since we know HSV values are all nonnegative and no greater than 256, we can use uint8_t: unsigned 8-bit integer
// We also can't use fmax out of <math.h> as it is not constexpr
constexpr uint8_t max(uint8_t a, uint8_t b) {
    return a > b ? a : b;
}

constexpr uint8_t max(uint8_t a, uint8_t b, uint8_t c) {
    return max(max(a, b), max(a, c)); // uses transitive property to calculate max with 3 numbers given 2 2-number comparisons
}

constexpr uint8_t min(uint8_t a, uint8_t b) {
    return a < b ? a : b;
}

constexpr uint8_t min(uint8_t a, uint8_t b, uint8_t c) {
    return min(min(a, b), min(a, c));
}

// Since % is for integers and fmod in <math.h> isn't constexpr, we need to create our own. IDK how the math works.
constexpr float modulo(float dividend, float divisor) {
    const auto quotient = dividend / divisor;
    return divisor * (quotient - static_cast<uint8_t>(quotient));
}

// Calculates HSV values given easier-to-comprehend rgb values and returns a struct containing the values.
constexpr Color rgb_to_hsv(uint8_t r, uint8_t g, uint8_t b) {
    Color c{};
    const auto c_max = max(r, g, b);
    c.V = c_max / 255.0f;

    const auto c_min = min(r, g, b);
    const auto delta = c.V - c_min / 255.0f;
    c.S = c_max == 0 ? 0 : delta / c.V;

    if (c_max == c_min) {
        c.H = 0;
        return c;
    }

    if (c_max == r) {
        c.H = (g / 255.0f - b / 255.0f) / delta;
    } else if (c_max == g) {
        c.H = (b / 255.0f - r / 255.0f) / delta + 2.0f;
    } else if (c_max == b) {
        c.H = (r / 255.0f - g / 255.0f) / delta + 4.0f;
    }

    c.H *= 60.0f;
    c.H = c.H >= 0.0f ? c.H : c.H + 360.0f;
    c.H = modulo(c.H, 360.0f);

    return c;
}

char* color_to_string(const Color color) {
    // create std:string to make use of concatenation ease
    std::string concat_str = "<" + std::to_string(color.H) + ", " + std::to_string(color.S) + ", " + std::to_string(color.V) + ">";
    // create new char* with size of string plus 1 for null terminator
    char* result = new char[concat_str.size()+1];
    // copy c_str into array. Note! simply using = won't work because arrays
    std::strcpy(result, concat_str.c_str());
    // return result
    return result;
    }

int main() {
    
    auto black  = rgb_to_hsv(0,   0,     0);
    auto white  = rgb_to_hsv(255, 255, 255);
    auto red    = rgb_to_hsv(255, 0,     0);
    auto blue   = rgb_to_hsv(0,   255,   0);
    auto green  = rgb_to_hsv(0,   0,   255);
    auto purple = rgb_to_hsv(255, 0,   255);

    printf("BLACK: %s\n", color_to_string(black));
    printf("WHITE: %s\n", color_to_string(white));
    printf("RED: %s\n", color_to_string(red));
    printf("BLUE: %s\n", color_to_string(blue));
    printf("GREEN: %s\n", color_to_string(green));
    printf("PURPLE: %s\n", color_to_string(purple));

    return 0;
}