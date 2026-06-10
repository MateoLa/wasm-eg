#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <time.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#else
    #include <unistd.h>
#endif

using namespace std;

/**
 * The main loop handler called repeatedly by the browser's event loop.
 * It must have a void* parameter
 */
void one_frame(void * _secretNumber){
    // Convert back the void pointer into an integer
    const auto secretNumber = static_cast<int*>(_secretNumber);
    static bool first_line = true;
    std::string userGuess;

    errno = 0;
    while (errno != EAGAIN) {
        if (first_line) {
            char c;
            if (std::cin.get(c)) {
                std::cout.put(c);
                if (c == '\n') {
                    first_line = false;
                }
            }
        } else {
            if (getline(std::cin, userGuess)) {
                std::cout << userGuess << "\n";
            }
        }

        if (std::cin.fail() && !std::cin.eof()) {
            if (errno != EAGAIN) {
                cerr << "error " << strerror(errno) << "\n";
                exit(EXIT_FAILURE);
            }
            std::cin.clear();
        }

        if (std::cin.eof()) {
            std::cout << "eof\n";
            exit(EXIT_SUCCESS);
        }
    }
}


int main(){
    int secretNumber;
    const int MAX_NUMBER = 100;

    srand(time(NULL));
    secretNumber = rand() % MAX_NUMBER + 1;

    std::cout << "Welcome to the Number Guessing Game!" << std::endl;
    std::cout << "Please enter a guess (1-100): " << std::endl;

    #ifdef EMSCRIPTEN
        emscripten_set_main_loop_arg(one_frame, &secretNumber, 10, 1);
    #else
        while (1) {
            one_frame(&secretNumber);
        }
    #endif

    return 0;
}

