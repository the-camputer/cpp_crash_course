#include <cstdio>

enum class Operation {
    ADD, SUBSTRACT, MULTIPLY, DIVIDE
};


// Struct: A class where everything is public by default
struct Calculator {

    Calculator() {
        op = Operation::ADD;
    }

    Calculator(Operation operation) {
        op = operation;
    }

    void setOperation(Operation operation) {
        op = operation;
    }

    int calculate(int a, int b) {
        switch(op) {
            case Operation::ADD: {
                return a + b;
                break;
            }
            case Operation::SUBSTRACT: {
                return a - b;
                break;
            }
            case Operation::MULTIPLY: {
                return a * b;
                break;
            }
            case Operation::DIVIDE: {
                return a / b;
                break;
            }
            default: {
                printf("Unkown Operation");
                return 0;
                break;
            }
        }
    }

    private:
        Operation op;
};

// Class: A struct where everthing is private by default
class Earth {
    public:
        Earth() {
            printf("Creating the greatest computer in existence...\n");
        }

        // Example Destructor
        ~Earth() {
            printf("Bye, and thanks for all the fish.\n");
        }
};

int main() {
    Earth newEarth {};

    Calculator calc(Operation::ADD);
    printf("Result of %d + %d is: %d\n", 10, 10, calc.calculate(10, 10));

    calc.setOperation(Operation::SUBSTRACT);
    printf("Result of %d - %d is: %d\n", 10, 10, calc.calculate(10, 10));

    calc.setOperation(Operation::MULTIPLY);
    printf("Result of %d * %d is: %d\n", 10, 10, calc.calculate(10, 10));

    calc.setOperation(Operation::DIVIDE);
    printf("Result of %d / %d is: %d\n", 10, 10, calc.calculate(10, 10));
    return 0;
}