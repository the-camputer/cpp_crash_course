#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

using namespace std;

union Info {
    float gpa;
    bool isRA;
};

struct Student {
    int id;
    bool isGrad;
    Info info;
};

int main() {
    Student jeffTheLasyUndergrad = {
        1, false, {.gpa=2.1}
    };
    Student rodgerTheDepressedGradStudent = {
        2, true, {.isRA=true}
    };
    cout << "Is Rodger an RA? " << rodgerTheDepressedGradStudent.info.isRA << endl;
    cout << "What is Jeff's GPA? " << jeffTheLasyUndergrad.info.gpa << endl;
    cout << "This theoretically is pointless and undefined: " << rodgerTheDepressedGradStudent.info.gpa << endl;

    int thing{};
    printf("Thing! %d\n", thing);
    return 0;
}