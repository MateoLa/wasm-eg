#include <stdio.h>
#include <emscripten/emscripten.h>

int main() {
    printf("Greetings extranger!!\n");
    return 0;
}

int add(int a, int b) { return a + b; }


#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

EXTERN EMSCRIPTEN_KEEPALIVE void myFooFunct(int argc, char ** argv) {
    printf("My Foo function Called\n");
}

// extern : Prevent name mangling
// EMSCRIPTEN_KEEPALIVE : Avoid optimization
