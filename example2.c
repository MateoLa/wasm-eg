#include <stdio.h>
#include <string.h>
#include <emscripten.h>

int add(int a, int b) { return a + b; }

extern char* EMSCRIPTEN_KEEPALIVE jsPrint(const char* s, int len);

void greet() { 
    const char* str = "Greetings from Wonderland & ML!!";
    jsPrint(str, strlen(str)); 
}

// EMSCRIPTEN_KEEPALIVE : Avoid optimization
// extern : Prevent name mangling