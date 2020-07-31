#include <cstdio>

using namespace std;

struct ClockOfTheLongNow {

    ClockOfTheLongNow(const long year)
        : year { year } {

    }

    long get_year() const {
        return year;
    }

    private:
        const long year;
};

struct Avout {
    const char* name;
    const ClockOfTheLongNow apert;

    Avout(const char* name, long year_of_apert)
        : name { name }, apert { year_of_apert } {
            // CAN DO THINGS HERE
    }

    void announce() {
        printf("My name is %s and my next appointment is %d.\n", name, apert.get_year());
    }
};

struct ValueHolder {
    int val;
};

struct College {
    char name[256];
    bool isSenior(const College& collegePerson) {
        return collegePerson.getFirstLetter() != 'S';
    }

    char getFirstLetter() const {
        return name[0];
    }
};

void print_names(College *colleges, size_t size) {
    for(size_t i = 0; i < size; i++) {
        printf("%s College\n", colleges[i].name);
    }
}


/* SUMMARY */
char read_from(const char charList[], int place) {
    size_t numElements = sizeof(charList) / sizeof(char);
    if (place >= numElements) {
        printf("Big off dog trying to slide out of my DM's\n");
        return '\0';
    }
    else {
        return charList[place];
    }
}

bool write_to(char charList[], int place, char letter) {
    size_t numElements = sizeof(charList) / sizeof(char);
    if (place >= numElements) {
        printf("Big oof dof trying to slide into my DM's\n");
        return false;
    }
    else {
        charList[place] = letter;
        return true;
    }
}

struct Element {
    Element* next {};
    Element* previous {};
    const char* prefix {}; // Made const to prevent compiler yelling about string literals
    short operating_number {};

    Element(const char prefix[2], short operating_number)
        : prefix { prefix }, operating_number { operating_number } {
    }

    void insert_after(Element* next_element) {
        next_element->next = next;
        next_element-> previous = this;
        next = next_element;
    }

    void insert_before(Element* new_previous) {
        previous->next = new_previous;
        new_previous->previous = previous;
        new_previous->next = this;
        previous = new_previous;
    }

    // This works because const char*'s can change what they point to, just not the values that are there.
    void setPrefix (const char prefix[]) {
        this->prefix = prefix;
    }
};

int main() {
    int my_int { 5 };
    int* my_ptr { &my_int };

    printf("Pointer is %p\n", my_ptr);
    printf("Pointer value is %d\n", *my_ptr);

    *my_ptr = 50;

    printf("int now is %d\n", my_int);

    ValueHolder a {27};
    ValueHolder* holderPtr = &a;

    printf("ValueHolder value: %d\n", holderPtr->val);
    holderPtr->val = 43;
    printf("True ValueHodler value: %d\n", a.val);


    College oxford[] = { "Magdalen", "Nuffield", "Kellog" };

    print_names(oxford, (sizeof(oxford) / sizeof(College)));

    Avout raz { "Erasmus", 3010 };
    Avout jad { "Jad", 4000 };
    raz.announce();
    jad.announce();

    /* SUMMARY */
    char lower[] { 'a', 'b', 'c', '?', 'e' };
    char upper[] { 'A', 'B', 'C', '?', 'E' };

    write_to(lower, 5, 'D');
    write_to(lower, 3, 'd');
    write_to(upper, 3, 'D');

    printf("Secret letter is: %c\n", read_from(lower, 17));
    printf("Secret letter is: %c\n", read_from(lower, 3));
    printf("Secret letter is: %c\n", read_from(upper, 3));

    Element first { "KT", 1 };
    Element second { "BC", 2 };
    first.insert_after(&second);

    first.setPrefix("ER");

    Element third { "HS", 3 };
    second.insert_before(&third);

    for(Element *elem = &first; elem; elem = elem->next) {
        printf ("HALP WHAT IS THIS?! %d, %s\n", elem->operating_number, elem->prefix);
    }

    for(Element *elem = &second; elem; elem = elem->previous) {
        printf("OH NO GOING BACKWARDS! %d, %s\n", elem->operating_number, elem->prefix);
    }

    auto original = 100;
    auto& original_ref = original;
    printf("Original: %d\n", original);
    printf("Reference: %d\n", original_ref);

    auto new_value = 200;
    original_ref = new_value;
    printf("Original: %d\n", original);
    printf("New Value: %d\n", original);
    printf("Reference: %d\n", original_ref);
    return 0;
}