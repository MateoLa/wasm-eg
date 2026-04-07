#include <cstdlib>
#include <vector>
#include <iostream>

#include <emscripten.h>
#include <emscripten/bind.h>

void mem_alloc_error() {
    std::vector<int> output;
    int nr;

    while (true) {
        nr = rand();
        output.push_back(nr);
    }
}

void out_of_bounds_error() {
    volatile int * ptr = (int*)100000000;
    *ptr = 42;
}

EMSCRIPTEN_BINDINGS(loop) {
    emscripten::function("mem_alloc_error", &mem_alloc_error);
    emscripten::function("out_of_bounds_error", &out_of_bounds_error);
}

