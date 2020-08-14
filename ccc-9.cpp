#include <cstdio>
#include <cstdint>
#include <cstdarg>

/*
    function prefix modifiers:
    * static: out-of-class - function exists entirely within a single translation unit (internal linkage). 
              in-class - method isn't associated with an instance of the class.
    * virtual: method can be overriden by a child class
    * constexpr: function should be evaluated at compile time
    * [[noreturn]]: signifies that control won't flow back to controller on function completion (compiler optimization, mostly for terminating functions).
    * inline: copy-pastes function code into execution path instead of using stack and registers (compiler optimization, most do it automatically anymore)
    
    fuction suffix modifiers:
    * noexcept: function will never throw an error. Allows compiler to optimize
    * const: function will not modify an instance of its class
    * final: method cannot be overriden by child class. Can be applied to class to prevent becoming a parent
             if you're using interface inheritance, mark your implementations as final to unlock runtime optimizations via devirtualization.
    * volatile: denotes that object's value can change at any time. Used similar to const (only volatile objects can call volatile methods) 
                Not used outside of low-level systems programming.
*/

struct BostonCorbett {
  virtual void shoot() final {
    printf("What a God we have...God avenged Abraham Lincoln");
  }
};

/*
    auto can be set as a return type and the compiler will figure out what to send back
*/
template<typename T, typename U>
auto returnEnhancedT(T t, U u) -> decltype(t+u) { // the -> decltype() will determine what type to use. You don't need this most of the time
    return t+u;
}

/*
    Variadic functions: take in a variable number of arguments. ex: printf
    Elements from variadic arguments cannot be extracted directly. Access granted through types in <cstdarg.h>
    On their own they're pretty unsafe (not type safe, requires knowing how many arguments there are).
*/
int sum(size_t n, ...) { // ellipses denotes the function as variadic
    va_list args;
    va_start(args, n); // starts the iterator
    int result;
    while (--n) {
        auto next_element = va_arg(args, int); // int is the expected argument type
        result += next_element;
    }
    va_end(args); // kill the iterator
    return result;
}

/*
    Variadic Templates: allows you to create functions that take a variable number of same-type arguments
    The 'typename...' is called a template parameter pack. Used within a function, it's called a function parameter pack.
    Function parameter packs allow you to get their size (number of arguments) via 'sizeof...(args)', as well as expand
    the pack for further processing, e.g. 'other_function(args...)'. This is important, because you cannot index a parameter
    pack directly. e.g.
    template<typename T, typename... Args>
    void my_func(T x, Args... args) {
        x++;
        my_func(args...);
    }

    This requires a 'stop' condition via an overloaded function with a different template. e.g.
    template<typename T>
    void my_func(T x) {
        do literally anything but recurse here
    }
*/
int z{};
template<typename T>
constexpr T sum(T x) {
    ++z;
    return x;
}

template<typename T, typename... Args>
constexpr T sum(T x, Args... args) {
    return x + sum(args...);
}

/*
    Fold Expressions: Computes a result by applying a binary operator over all elements in an argument pack
*/
template<typename... T>
constexpr auto sum2(T... args) {
    return (... + args); // pack operator, binary operator to apply, parameter pack to apply it to.
}

struct BostonCorbettJunior : BostonCorbett {
  //void shoot() override { } // Bang! shoot is final.
};

int main() {
    BostonCorbettJunior junior;

    printf("THING! %d\n", returnEnhancedT(100, 12));
    printf("THING AGAIN! %f\n", returnEnhancedT(100.0, 12));

    printf("The answer is %d\n", sum(2, 4, 6, 8, 10, 12));
    printf("Another answer is %d\n", sum2(1, 5, 6, 8, 10, 12));
}