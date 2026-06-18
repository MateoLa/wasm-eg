#include <iostream>
#include <sstream>

#include <emscripten.h>
#include <emscripten/bind.h>


std::string engine = "I am the Engine runing.";


// from uci.cpp
void one_step(std::string token) {
    if (token == "quit" || token == "stop") return;
    else if (token == "engine")  std::cout << "MaLa: " << engine << std::endl;
    else if (token == "uci")  std::cout << "MaLa: uciok" << std::endl;
    else if (token == "go")  std::cout << "MaLa: GO" << std::endl;
    else if (token == "--help" || token == "help" || token == "--license" || token == "license")
        std::cout << "\nStockfish is a powerful chess engine for playing and analyzing." << std::endl;
    else  std::cout << "MaLa: Unknown command" << std::endl;
}


int main(int argc, char* argv[]) {
    std::string cmd;
    std::cout << "To WebAssembly by MaLa" << std::endl;

    return 0;
}


EMSCRIPTEN_BINDINGS(sf) {
    emscripten::function("one_step", &one_step);
}


/*
extern "C" avoids name mangling although EMSCRIPTEN_BINDINGS should do it so.
extern "C" is a low-level declaration an uses far fewer resources than embind. Use it in conjunction with -s EXPORTED_FUNCTIONS compilation flag.
*/
