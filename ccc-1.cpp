#include <cstdio>

// Function declarations can be made before instantiation. Allows main() to be created before functions.
int absolute_value(int);

int main() {
    short myNegNum = -010; // uniary '-' character flips sign of operand
    long zero = 0;
    unsigned long long myPosNum = 1'000'000; // apostrophes can be used to make big numbers more readable

    // Examples of formatters
    printf("The absolute value of %hd is %d\n", myNegNum, absolute_value(myNegNum)); // hd = short
    printf("The absolute value of %ld is %d\n", zero, absolute_value(zero)); // ld = long
    printf("The absolute value of %llu is %d\n", myPosNum, absolute_value(myPosNum)); // llu = unsigned long long

    return 0;
}

int absolute_value(int x) {
    if (x < 0) {
        return -1 * x;
    } else {
        return x;
    }
}