#include <iostream>
#include <vector>

#include <emscripten.h>
#include <emscripten/bind.h>

void create_memory_error() {
    std::vector<int*> pointers;
    // An infinite loop condition is simulated here (e.g., a logic error in a real program)
    // In a real WASM scenario, this might be triggered if a condition is never met.
    // The following will rapidly consume all memory.
    while (true) {
        // Allocate memory dynamically in the loop
        int* ptr = new int[1000];
        if (ptr == nullptr) {
            // Handle allocation failure if possible
            break;
        }
        pointers.push_back(ptr);
        // In a real application, the infinite loop might also include 
        // array indexing that eventually goes out of bounds of the initially 
        // defined static or dynamically growing memory. 
        // In Wasm, this will eventually trigger a "memory access out of bounds" trap 
        // as it runs out of the browser's maximum allocated Wasm memory.
    }
}

EMSCRIPTEN_BINDINGS(loop) {
    emscripten::function("create_memory_error", &create_memory_error);
}

