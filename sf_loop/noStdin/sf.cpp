#include <iostream>
#include <sstream>

#include <emscripten.h>
#include <emscripten/bind.h>


// from uci.cpp
void uci_loop(std::string token) {
    do {
//        if (!getline(std::cin, token))  token = "quit"; // Wait for an input or an end-of-file (EOF) indication

        if (token == "quit" || token == "stop") return;
        else if (token == "ponderhit")  std::cout << "MaLa: Ponderhit" << std::endl;
        else if (token == "uci")  std::cout << "MaLa: uciok" << std::endl;
        else if (token == "go")  std::cout << "MaLa: GO" << std::endl;
        else if (token == "--help" || token == "help" || token == "--license" || token == "license")
            std::cout << "\nStockfish is a powerful chess engine for playing and analyzing." << std::endl;
//        else  std::cout << "MaLa: Unknown command" << std::endl;
    } while (token != "quit");
}


int main(int argc, char* argv[]) {
    std::string cmd;
    std::cout << "To WebAssembly by MaLa" << std::endl;

    for (int i = 1; i < argc; ++i) { cmd += std::string(argv[i]) + " "; } // from uci.cpp

    uci_loop(cmd);

    std::cout << "MaLa: C++ program ended" << std::endl;

    return 0;
}


EMSCRIPTEN_BINDINGS(sf) {
    emscripten::function("uci_loop", &uci_loop);
}


/*
extern "C" avoids name mangling although EMSCRIPTEN_BINDINGS should do it so.
extern "C" is a low-level declaration an uses far fewer resources than embind. Use it in conjunction with -s EXPORTED_FUNCTIONS compilation flag.
*/
