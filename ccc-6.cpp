#include <cstdio>
#include <utility>
#include <map>
#include <set>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

template<typename T>
T mean(T* values, size_t length) {
    T result{};
    for(size_t i{}; i < length; i++) {
        result += values[i];
    }

    return result / length;
}

int mode(const int* values, size_t length) {
    if (length <= 0) {
        printf("NO VALUES GIVEN\n");
        return 0;
    }

    std::map<int, int> val_occurance_counts;

    for(size_t i = 0; i < length; i++) {
        int val = values[i];
        if (val_occurance_counts.find(val) != val_occurance_counts.end()) {
            val_occurance_counts[val] += 1;
        } else {
            val_occurance_counts[val] = 1;
        }
    }

    int highest_count = 0;
    std::set<int> highest_count_entries;

    std::map<int, int>::iterator iter = val_occurance_counts.begin();
    while (iter != val_occurance_counts.end()) {
        if (iter->second >= highest_count) {
            highest_count = iter->second;
        }
        iter++;
    }

    iter = val_occurance_counts.begin();
    while(iter != val_occurance_counts.end()) {
        if (iter->second == highest_count) {
            highest_count_entries.insert(iter->first);
        }
        iter++;
    }

    if (highest_count_entries.size() > 1) {
        printf("TOO MANY MODES\n");
        return 0;
    } else {
        return *(highest_count_entries.begin());
    }
}

/* 
 * RAII wrapper around pointer objects so that, 
 * when the wrapper is deleted, the underlying object is also deleted.
 * This a pedagogical implementation of std::unique_ptr, part of the RAII template group in std called smart pointers
 */
template <typename T>
struct SimpleUniquePointer {
    // default used when you need both a default and non-default constructor
    // default constructor will set pointer to nullptr due to default initialization rules
    SimpleUniquePointer() = default; 
    
    SimpleUniquePointer(T* pointer) 
        : pointer { pointer } {

    }

    ~SimpleUniquePointer() {
        // Make sure pointer is not nullptr before deleting
        if (pointer) delete pointer;
    }

    // =delete: prevents compiler from automatically generating functions and operators
    // in these cases: prevents creations of copy constructors to prevent double free problems
    SimpleUniquePointer(const SimpleUniquePointer&) = delete;
    SimpleUniquePointer& operator=(const SimpleUniquePointer&) = delete;
    SimpleUniquePointer(SimpleUniquePointer&& other) noexcept
        : pointer { other.pointer } {
        other.pointer = nullptr;        
    }
    SimpleUniquePointer& operator=(const SimpleUniquePointer&& other) noexcept {
        if (pointer) delete pointer; // make sure to delete whatever is being held before reassigning
        pointer = other.pointer;
        other.pointer = nullptr;
        return *this;
    }

    T* get() {
        return pointer;
    }

    private:
        T* pointer;
};

struct Tracer {
    Tracer(const char* name)
        : name { name } {
        printf("%s constructed.\n", name);
    }

    ~Tracer() {
        printf("%s destructed.\n", name);
    }

    private:
        const char* const name;
};

void consumer(SimpleUniquePointer<Tracer> consumer_ptr) {
    printf("(cons) consumer_ptr: 0x%p\n", consumer_ptr.get());
}

int main() {
    long longs[] {1, 2, 3};
    int ints[] { 4, 5, 6};
    double doubles[] {7, 8, 9};

    long long_mean = mean<long>(longs, 3);
    int int_mean = mean<int>(ints, 3);
    double double_mean = mean<double>(doubles, 3);

    printf("LONG MEAN: %ld\n", long_mean);
    printf("INT MEAN: %d\n", int_mean);
    printf("DOUBLE MEAN: %f\n", double_mean);

    auto ptr_a = SimpleUniquePointer<Tracer>{new Tracer("ptr_a")};
    printf("(main) ptr_a: 0x%p\n", ptr_a.get());
    consumer(std::move(ptr_a));
    printf("(main) ptr_a: 0x%p\n", ptr_a.get()); // ptr_a now in a moved-from state, inner Tracer moved from ptr_a to consumer

    int counts_too_many[] = {12, 24, 24, 54, 1, 3, 7, 7, 9, 9};
    mode(counts_too_many, 8);
    int counts_none[] = {};
    mode(counts_none, 0);
    int counts_good[] = {1, 1, 1, 2, 2, 3, 4, 5, 6, 6, 6, 6};
    int m_mode = mode(counts_good, 12);
    printf("MODE! %d\n", m_mode);
    return 0;

    // TODO: HEY PUT THIS IN GITHUB
}