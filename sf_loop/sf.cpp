#include <iostream>
#include <emscripten.h>
#include <emscripten/bind.h>



// from uci.cpp
// Execute UCI::loop() only once.
void wasm_uci_execute(std::string token) {

    if (token == "quit" || token == "stop") return;
    else if (token == "ponderhit")
        std::cout << "MaLa: Ponderhit Command" << std::endl;
    else if (token == "uci")
        std::cout << "MaLa: uciok" << std::endl;
    else if (token == "setoption")
        std::cout << "MaLa: SetOption" << std::endl;
    else if (token == "go")
        std::cout << "MaLa: GO" << std::endl;
    else if (token == "--help" || token == "help" || token == "--license" || token == "license")
        std::cout << "\nStockfish is a powerful chess engine for playing and analyzing." << std::endl;
    else if (!token.empty() && token[0] != '#')
        std::cout << "Unknown command: '" << token << "'. Type help for more information." << std::endl;
}


int main(int argc, char* argv[]) {
    std::cout << "To WebAssembly by MaLa" << std::endl;
    return 0;
}


EMSCRIPTEN_BINDINGS(sf) {
//    emscripten::function("wasm_uci_execute", &wasm_uci_execute, emscripten::allow_raw_pointers());
    emscripten::function("wasm_uci_execute", &wasm_uci_execute);
}


