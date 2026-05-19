#include <iostream>
#include <sstream>
#include <emscripten.h>
#include <emscripten/bind.h>


// from misc.cpp
std::string engine_info() {
    std::string str = " by the Stockfish developers (see AUTHORS file)";
    str += "\nTo WebAssembly by MaLa";
    return str;
}


// from uci.cpp
// Execute UCI::loop() only once.
void one_frame(void * _token) {
    std::stringstream ss;
    ss << static_cast<char*>(_token);
    std::string token = ss.str();

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
        std::cout
            << "\nStockfish is a powerful chess engine for playing and analyzing."
                "\nIt is released as free software licensed under the GNU GPLv3 License."
                "\nStockfish is normally used with a graphical user interface (GUI) and implements"
                "\nthe Universal Chess Interface (UCI) protocol to communicate with a GUI, an API, etc."
                "\nFor any further information, visit https://github.com/official-stockfish/Stockfish#readme"
                "\nor read the corresponding README.md and Copying.txt files distributed along with this program.\n"
            << std::endl;
    else if (!token.empty() && token[0] != '#')
        std::cout << "Unknown command: '" << token << "'. Type help for more information." << std::endl;
}


extern "C" void wasm_uci_execute(std::string argv) {
    emscripten_set_main_loop_arg(one_frame, &argv, 0, 1);
    std::cout << "MaLa debugging: Exiting UCI Loop" << std::endl;
}


int main(int argc, char* argv[]) {
    std::cout << engine_info() << std::endl;

    std::string my_argv;
    for (int i = 0; i < argc; ++i) my_argv += argv[i] + std::string(" ");

    wasm_uci_execute(my_argv);

    return 0;
}


EMSCRIPTEN_BINDINGS(sf_comm) {
//    emscripten::function("wasm_uci_execute", &wasm_uci_execute, emscripten::allow_raw_pointers());
    emscripten::function("wasm_uci_execute", &wasm_uci_execute);
}


