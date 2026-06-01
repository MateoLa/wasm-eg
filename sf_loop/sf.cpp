#include <iostream>
#include <sstream>
#include <emscripten.h>
#include <emscripten/bind.h>


// from misc.cpp
std::string engine_info() {
    std::string str = "To WebAssembly by MaLa";
    return str;
}


// from uci.cpp
// Execute UCI::loop() only once.
void wasm_uci_execute(std::string token) {

    if (token == "quit" || token == "stop") emscripten_cancel_main_loop();
    else if (token == "ponderhit")
        std::cout << "MaLa debugging: Ponderhit Command" << std::endl;
    else if (token == "uci")
        std::cout << "MaLa debugging: uciok" << std::endl;
    else if (token == "setoption")
        std::cout << "MaLa debugging: SetOption" << std::endl;
    else if (token == "go")
        std::cout << "MaLa debugging: GO" << std::endl;
    else if (token == "--help" || token == "help" || token == "--license" || token == "license")
        std::cout << "\nStockfish is a powerful chess engine for playing and analyzing." << std::endl;
    else if (!token.empty() && token[0] != '#')
        std::cout << "Unknown command: '" << token << "'. Type help for more information." << std::endl;
}


int main(int argc, char* argv[]) {
    std::cout << engine_info() << std::endl;

    wasm_uci_execute("");

    std::cout << "MaLa: Exiting UCI Loop" << std::endl;
    return 0;
}


EMSCRIPTEN_BINDINGS(to_SF_comm) {
//    emscripten::function("wasm_uci_execute", &wasm_uci_execute, emscripten::allow_raw_pointers());
    emscripten::function("wasm_uci_execute", &wasm_uci_execute);
}


