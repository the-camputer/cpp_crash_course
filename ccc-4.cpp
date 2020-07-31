#include <cstdio>
#include <string>
#include <cstring>
#include <ctime>
#include <stdexcept>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif


/*******************************************/
/*             RULE OF FIVE                */
/******************************************/
/*
    The Rule of Five says that there are 5 methods that govern move and copy behavior:
    1) The destructor
    2) The copy constructor
    3) The move constructor
    4) The copy assignment operator
    5) The move assignment operator

    In order to avoid headaches caused by the compiler automatically generating these methods
    on your behalf, you should explicitly define these yourself. A little time up front to 
    save a lot of headaches later on.
*/
struct SimpleString {
    SimpleString(size_t max_size) 
        : max_size{ max_size },
          length {} {
        if (max_size == 0) {
            throw std::runtime_error { "Max size must be at least 1." };
        }

        buffer = new char[max_size];
        buffer[0] = 0;
    }

    // Copy Constructor
    SimpleString(const SimpleString& other)
    : max_size{ other.max_size },
      length{ other.length },
      buffer{ new char[other.max_size] } {
        strcpy(buffer, other.buffer);
    }

    // Move Constructor; because other is an explicit rvalue, we can cannibalize it
    // Because it's designed not to throw an exception, it's marked noexcept
    // **** MOVING IS WAY LESS EXPENSIVE THAN COPYING BECAUSE RESOURCES ARE SAVED ***
    SimpleString(SimpleString&& other) noexcept
    : max_size{ other.max_size },
      length{ other.length},
      buffer { other.buffer } {
        other.length = 0;
        other.buffer = nullptr;
        other.max_size = 0;
    }

    // Copy Assignment Operator
    SimpleString& operator=(const SimpleString& other) {
        if (this == &other) return *this;

        const auto new_buffer = new char[other.max_size];
        delete[] buffer;
        buffer = new_buffer;
        length = other.length;
        max_size = other.max_size;
        strcpy(buffer, other.buffer);
        return *this;
    }

    // Move Assignment Operator
    SimpleString& operator=(SimpleString&& other) noexcept{
        if (this == &other) return *this;

        delete[] buffer;
        buffer = other.buffer;
        length = other.length;
        max_size = other.max_size;
        other.buffer = nullptr;
        other.length = 0;
        other.max_size = 0;

        return *this;
    }

    ~SimpleString() {
        delete[] buffer;
    }

    void print(const char* tag) const {
        printf("%s: %s\n", tag, buffer);
    }

    bool append_line(const char* appendee) {
        const auto appendee_len = strlen(appendee);
        if (appendee_len + length + 2 > max_size) {
            return false;
        } else {
            // Buffer + length == next spot in buffer to place string
            // max_size - length makes sure that insertion doesn't exceed buffer length
            std::strncpy(buffer + length, appendee, max_size - length);
            length += appendee_len; // update length
            buffer[length++] = '\n'; // add newline
            buffer[length] = 0; // null terminate
            return true;
        }
    }

    private:
        size_t max_size;
        char* buffer;
        size_t length;
};

struct SimpleStringOwner {
    SimpleStringOwner(const char* str) 
    : string { 10 } {
        if (!string.append_line(str)) {
            throw std::runtime_error { "Not enough memory!" };
        } else {
            string.print("Constructed: ");
        }
    }

    // Constructor utilizes moving to wrap reference without wasting resources
    SimpleStringOwner(SimpleString&& x) : string{ std::move(x) } {}

    ~SimpleStringOwner() {
        string.print("About to destroy: ");
    }

    private:
        SimpleString string;
};

struct TimerClass {

    TimerClass(const char* name) {
        timestamp = time(NULL);
        this->name = new std::string{name};
        printf("Timestamp for %s is %ld\n", (*this->name).c_str(), timestamp);
    }

    TimerClass(const TimerClass& other) {
        timestamp = other.timestamp;
        name = new std::string{*other.name};
    }

    TimerClass(TimerClass&& other) {
        timestamp = other.timestamp;
        name = other.name;
        other.name = nullptr;
        other.timestamp = 0;
    }

    TimerClass& operator=(TimerClass& other) {
        if (this == &other) return *this;

        timestamp = other.timestamp;
        delete name;
        name = other.name;
        return *this;
    }

    TimerClass& operator=(TimerClass&& other) noexcept {
        if (this == &other) return *this;

        this->timestamp = other.timestamp;
        delete name;
        name = other.name;
        other.name = nullptr;
        return *this;
    }

    ~TimerClass() {
        long int deathTime = time(NULL);
        long int age = deathTime - timestamp;

        if (name != NULL) {
            printf("Age of %s is %ld\n", (*name).c_str(), age);
        } else if (timestamp != 0) {
            printf("Age of anonymous is %ld\n", age);
        }
    }

    void setName(const char* newName) {
        if (name != NULL) {
            printf("Changing name of %s to %s\n", (*name).c_str(), newName);
            delete name;
        }
        name = new std::string{newName};
    }

    std::string getName() {
        return *name;
    }

    int getTimestamp() {
        return timestamp;
    }

    void setTimestamp(const int timestamp) {
        this->timestamp = timestamp;
    }
    
    private:
    int timestamp;
    std::string *name;
};

void fn_c() {
    SimpleStringOwner c{ "cccccccccccccc" };
}

void fn_b() {
    SimpleStringOwner b{ "bbbbbbbb" };
    fn_c();
}

// Shit be expensive to pass by value since it calls the copy constructor.
// use const where you can to pass by reference instead.
void foo(SimpleString x) {
    x.append_line("This change is lost");
}

void ref_type(int& x) {
    printf("lvalue reference: %d\n", x);
}

void ref_type(int&& x) {
    printf("rvalue reference: %d\n", x);
}

int main() {
    

    TimerClass t{"t"};
    TimerClass t2 = t;
    t2.setName("t2");
    TimerClass moveable{"moveable"};
    TimerClass t3 {{std::move(moveable)}};
    t3.setName("t3");

    SimpleString string {115};
    string.append_line("Starbuck. Whaddya hear?");
    string.append_line("Nothin' but the rain.");
    string.print("A: ");
    string.append_line("Grab ya gun and bring the cat in.");
    string.append_line("Aye-aye sir, coming home.");
    string.print("B: ");
    if (!string.append_line("Galactica!")) {
        printf("String was not big enough to append another message.\n");
    }

    SimpleStringOwner x { "x" };
    printf("x is alive!\n");

    try {
        SimpleStringOwner a{ "aaaaaa" };
        fn_b();
        SimpleStringOwner d{ "dddddd" };
    } catch(const std::runtime_error& e) {
        printf("Exception: %s\n", e.what());
    }

    SimpleString a{ 50 };
    a.append_line("We appologize for the");
    SimpleString a_copy{a};
    a.append_line("inconvenience.");
    a_copy.append_line("incontinence.");
    a.print("a");
    a_copy.print("a_copy");

    SimpleString empty_boi{ 20 };
    foo(empty_boi);
    a.print("Still empty");

    auto b = 1;
    ref_type(b); // lvalue because named
    ref_type(std::move(b)); //std::move doesn't actually move, just cast lvalue to rvalue
    ref_type(2); // rvalue because value is unnamed
    ref_type(b + 2); //rvalue because equation result is unnamed
    sleep(5);
    return 0;
}