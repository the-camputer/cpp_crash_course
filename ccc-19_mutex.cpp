/*
    Mutex: Mechanism for preventing multiple threads from accessing shared resources simultaneously.
        - Supports two operations:
            - lock: locks down a resource so other resources can't access it
                - lock() will suspend thread until lock can be obtained, then proceed
                - try_lock() will return immediately with boolean saying if lock was obtained
            - unlock: releases lock on the resource
        - <mutex> types:
            - std::mutex - basic mutex implementation
            - std::timed_mutex - mutual exclusion, with a timeout
            - std::recursive_mutex - mutual exclusion with recursive locking on same thread
            - std::recursive_time_mutex - mixture of recursive and timed mutex
            - std::lock_guard - RAII wrapper for mutex. Non-copyable, non-movable
            - std::scoped_lock - deadlock RAII wrapper for multiple mutexes
            - std::unique_lock - movable mutex ownership wrapper
            - std::shared_lock - movable mutex ownership for shared mutexes.
        - <shared_mutex> types:
            - std::shared_mutex - allows for several threads to own the mutex simultaneously. Typicall
                                  used with multiple readers and single writer
            - std::shared_timed_mutex - shared mutex with a timeout
*/
#include <mutex>
#include <future>
#include <atomic>
#include <condition_variable>
#include <iostream>

using namespace std;

// Uses lock and unlock to handle mutex
void goat_rodeo() {
    const size_t iterations { 1'000'000 };
    int tin_cans_available {};
    mutex tin_can_mutex;

    auto eat_cans = async(launch::async, [&] {
        for(size_t i{}; i < iterations; i++) {
            tin_can_mutex.lock();
            tin_cans_available--; // these operations are now *atomic* because no one can interrupt or look halfway through
            tin_can_mutex.unlock();
        }
    });

    auto deposit_cans = async(launch::async, [&] {
        for(size_t i{}; i < iterations; i++) {
            tin_can_mutex.lock();
            tin_cans_available++; // these operations are now *atomic* because no one can interrupt or look halfway through
            tin_can_mutex.unlock();
        }
    });

    eat_cans.get();
    deposit_cans.get();
    cout << "Tin cans available: " << tin_cans_available << endl;
}

// Uses RAII wrapper around mutex to prevent accidentally forgetting to unlock
// lock_guard will release lock as soon as it falls out of scope
void raii_rodeo() {
    const size_t iterations { 1'000'000 };
    int tin_cans_available {};
    mutex tin_can_mutex;

    auto eat_cans = async(launch::async, [&] {
        for(size_t i{}; i < iterations; i++) {
            lock_guard<mutex> guard { tin_can_mutex };
            tin_cans_available--; // these operations are now *atomic* because no one can interrupt or look halfway through
        }
    });

    auto deposit_cans = async(launch::async, [&] {
        for(size_t i{}; i < iterations; i++) {
            lock_guard<mutex> guard { tin_can_mutex };
            tin_cans_available++; // these operations are now *atomic* because no one can interrupt or look halfway through
        }
    });

    eat_cans.get();
    deposit_cans.get();
    cout << "Tin cans available: " << tin_cans_available << endl;
}

/*
    atomics: means of creating thread-safe operations without the need for locks (lock-free concurrent programming).
             These are helpful because mutexes are computationally expensive to create, lock, and unlock.
             Atomics create the same safety without the overhead.
        - Shit be hella faster.
        - Using atomics correctly and creating lock-free solutions for complicated solutions is difficult to do correctly
*/
void atomic_rodeo() {
    const size_t iterations { 1'000'000 };
    std::atomic_int tin_cans_available {};

    auto eat_cans = async(launch::async, [&] {
        for(size_t i{}; i < iterations; i++) {
            tin_cans_available--;
        }
    });

    auto deposit_cans = async(launch::async, [&] {
        for(size_t i{}; i < iterations; i++) {
            tin_cans_available++;
        }
    });

    eat_cans.get();
    deposit_cans.get();
    cout << "Tin cans available: " << tin_cans_available << endl;
}

/*
    Condition Variables: Synchronization primitive that blocks one ore more threads until notified.
        Another thread can notify the condition variable, which can then notify one or more threads so they can do work.

    Typical Pattern:
        - One thread does the following:
            - Acquire some mutex shared with waiting threads
            - modify the shared state
            - notify the condition variable
            - release the mutex
        - Threads waiting on the condition variable do the following:
            - Acquire the mutex
            - wait on the condition variable
            - when notified, mutex is reacquired and work is performed
            - release the mutex

    - ALWAYS HAVE VERIFICATION WHEN WAITING ON CONDITION VARIABLE
*/
void conditional_rodeo() {
    mutex m;
    condition_variable cv;
    const size_t iterations{ 1'000'000 };
    int tin_cans_available{};

    auto eat_cans = async(launch::async, [&] {
        unique_lock<mutex> lock{ m };
        cv.wait(lock, [&] { return tin_cans_available == 1'000'000; });
        for(size_t i{}; i < iterations; i++) {
            tin_cans_available--;
        }
    });

    auto deposit_cans = async(launch::async, [&] {
        scoped_lock<mutex> lock{ m };
        for(size_t i{}; i < iterations; i++) {
            tin_cans_available++;
        }
        cv.notify_all();
    });

    eat_cans.get();
    deposit_cans.get();

    cout << "Tin cans available: " << tin_cans_available << endl;
}

int main() {
    goat_rodeo();
    goat_rodeo();
    goat_rodeo();

    raii_rodeo();
    raii_rodeo();
    raii_rodeo();

    atomic_rodeo();
    atomic_rodeo();
    atomic_rodeo();

    conditional_rodeo();
    conditional_rodeo();
    conditional_rodeo();

    return 0;
}