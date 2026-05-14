#include <iostream>
#include <emscripten.h>
#include <emscripten/bind.h>


// from misc.cpp
std::string engine_info() {
    std::string str = " by the Stockfish developers (see AUTHORS file)";
    str += "\nTo WebAssembly by MaLa";
    return str;
}


// from uci.cpp
void loop(int argc, std::string argv) {
    std::string token;

    do
    {
        if (argc == 1 && !getline(std::cin, token)) token = "quit";
    std::cout << "MaLa debugging: Into the loop. Not waiting." << std::endl;

        token.clear();  // Avoid a stale if getline() returns nothing or a blank line
        token = argv;

        if (token == "quit" || token == "stop") emscripten_cancel_main_loop();

        // The GUI sends 'ponderhit' to tell that the user has played the expected move.
        // So, 'ponderhit' is sent if pondering was done on the same move that the user
        // has played. The search should continue, but should also switch from pondering
        // to the normal search.
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
            std::cout << "Unknown command: '" << token << "'. Type help for more information."
                      << std::endl;
std::cout << "MaLa debugging: Into the loop. Token: " + token << std::endl;
std::cout << "MaLa debugging: Into the loop. Arg Counter: " + std::to_string(argc) << std::endl;
    } while (token != "quit" && argc == 1);  // The command-line arguments are one-shot
}


// Execute UCI::loop() only once.
extern "C" void wasm_uci_execute(int argc, std::string argv) {
    loop(argc, argv);
    std::cout << "MaLa debugging: Exiting UCI Loop" << std::endl;
}


int main(int argc, char* argv[]) {
    std::cout << engine_info() << std::endl;

    std::string my_argv;
    for (int i = 0; i < argc; ++i) my_argv += argv[i] + std::string(" ");
    wasm_uci_execute(argc, my_argv);

    return 0;
}


EMSCRIPTEN_BINDINGS(sf_comm) {
//    emscripten::function("wasm_uci_execute", &wasm_uci_execute, emscripten::allow_raw_pointers());
    emscripten::function("wasm_uci_execute", &wasm_uci_execute);
}


