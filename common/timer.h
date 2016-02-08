#pragma once

#include <chrono>

class Timer {
public:
    Timer() {
        reset();
    }

    ~Timer() {}

    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }

    double elapsed() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start).count();
    }

private:
    std::chrono::high_resolution_clock::time_point start;
};
