#include <stdexcept>
#include <limits>
#include <cstdio>
#include <new>
#include <cstddef>

/*
    Operator Overloading: process of defining custom behavior for operators.
    Typically used on user-defined classes.
*/
struct CheckedInteger {

    const unsigned int value; // const means that CheckedInteger is immutable. This prevents changes during + operator

    CheckedInteger(unsigned int value) : value { value } {}

    /* 
        Overloading + operator:
            Allows for the addition of a regular unsigned int to a CheckedInteger and produce a resulting CheckedInteger
            with the correct value.
    */
    CheckedInteger operator+(unsigned int other) const {
        CheckedInteger result { value + other};

        if (result.value < value) throw std::overflow_error { "OVERFLOW!!!" };

        return result;
    }
};

/*
    overloading 'new' operator: Done when you need to have finer grain control over memory management in the free-store.
    For example, overloading new and delete operators can help prevent memory fragmentation by allocating/deallocating whole
    blocks of memory at once, i.e. buckets.
*/

// Object representing a contiguous unit of 4096 bytes of memory
struct Bucket {
    const static size_t data_size { 4096 };
    std::byte data[data_size];
};

/*
    Implements a heap by using a set series of Buckets instead of free memory.
*/
struct Heap {
    // Attempts to allocate a new bucket of memory
    void* allocate(size_t bytes) {
        // Enforce that number of bytes requested must be less than a single bucket
        if (bytes > Bucket::data_size) throw std::bad_alloc{};

        // Find the next available bucket and return its contents, i.e. a byte array used for holding data
        for(size_t i{}; i < n_heap_buckets; i++) {
            if (!bucket_used[i]) {
                bucket_used[i] = true;
                return buckets[i].data;
            }
        }

        throw std::bad_alloc{};
    }

    // "Frees" memory by resetting flag for corresponding bucket to say it is usable again.
    // Note: void pointers have no associated type: they deal in raw memory.
    void free(void* p) {
        for (size_t i{}; i < n_heap_buckets; i++) {
            if (buckets[i].data == p) {
                bucket_used[i] = false;
                return;
            }
        }
    }

    static const size_t n_heap_buckets { 10 };
    Bucket buckets[n_heap_buckets];
    bool bucket_used[n_heap_buckets];
};

// Declare heap and operator overloads in namespace scope so that way
// compiler picks them up and uses them program-wide.
// NOTE! THIS IS TERRIBLE TO DO IN REAL LIFE AS STL LIBRARIES WILL BE AFFECTED BY THIS.
Heap heap;
void* operator new(size_t n_bytes) {
    return heap.allocate(n_bytes);
}
void operator delete(void* p) {
    return heap.free(p);
}


/*
    User-defined type Conversions:
    C++ provides the ability for user-defined types to also include user-defined conversion functions
*/
struct ReadOnlyInt {
    ReadOnlyInt(int val) : value { val } {}

    // overloads explicit conversion operation. Leaving off the 'explicit' allows for implicit conversions.
    explicit operator int() const {
        printf("EXPLICIT CONVERTIN!\n");
        return value;
    }

    private:
    const int value;
};

/*
    Precedence and Evaluation Order:
    Precendence order is about compile-time associativity and defining which operators bind to which operands
    e.g. *a++ - since postfix incrementation is higher precendence order than dereferencing, a is bound 
    to the postfix incrementation and the result is then bound to the dereference: *(a++)

    Evaluation order deals with in what order should expressions be evaluated
    e.g. stop() + drop() + roll(), C++ does not enfore evaluation order, therefore there is no guarantee 
    that these are called in left-to-right order.

*/

int main() {
    CheckedInteger a { 100 };
    auto b = a + 200;
    printf("a + 200 = %u\n", b.value);
    try {
        auto c = a + std::numeric_limits<unsigned int>::max(); // numeric_limits in <limits> header able to get max or min of type
    } catch (const std::overflow_error& e) {
        printf("(a + max) EXCEPTION: %s\n", e.what());
    }

    printf("Buckets! %p\n", heap.buckets);
    auto breakfast = new unsigned int { 0xC0FFEE };
    auto dinner = new unsigned int { 0xDEADBEEF };
    printf("Breakfast: %p 0x%x\n", breakfast, *breakfast); // Note how breakfast address is same as buckets. Starts at beginning of heap
    printf("Dinner: %p 0x%x\n", dinner, *dinner); // Note how address is so 0x1000 bytes (4096) of breakfast, even though the actual data contents is tiny
    delete breakfast;
    delete dinner;

    try {
        // This should get run 10 times, i.e. the number of buckets in the heap
        while(true) {
            auto thing = new char;
            printf("Allocated a char. %p\n", thing);
        }
    } catch(const std::bad_alloc&) {
        printf("std::bad_alloc caught\n");
    }

    // Using User-defined conversion
    ReadOnlyInt ro_int { 42 };
    // auto more_stuff = ro_int * 10; HEY! This won't work because there's no implicit conversion defined for ReadOnlyInt
    auto even_more_stuff = static_cast<int>(ro_int) * 12; // Notice explicit conversion is used here.

    return 0;
}