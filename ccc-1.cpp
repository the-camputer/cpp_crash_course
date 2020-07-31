#include <cstdio>

int absolute_value(int);

int main() {
    short myNegNum = -010;
    long zero = 0;
    unsigned long long myPosNum = 1'000'000;

    printf("The absolute value of %hd is %d\n", myNegNum, absolute_value(myNegNum));
    printf("The absolute value of %ld is %d\n", zero, absolute_value(zero));
    printf("The absolute value of %llu is %d\n", myPosNum, absolute_value(myPosNum));

    return 0;
}

int absolute_value(int x) {
    if (x < 0) {
        return -1 * x;
    } else {
        return x;
    }
}