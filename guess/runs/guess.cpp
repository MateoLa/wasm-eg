#include <cstdlib>
#include <iostream>
#include <time.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

/**
 * The main loop handler called repeatedly by the browser's event loop.
 * It must have a void* parameter
 */
void one_frame(void * _secretNumber){
    // Convert the void pointer back into an integer
    const auto secretNumber = static_cast<int*>(_secretNumber);

    int userGuess;
    std::cin >> userGuess;

    if (userGuess > 100 || userGuess < 1) {
        std::cerr << "Invalid input\n" << std::endl;
        delete secretNumber;
        emscripten_cancel_main_loop();
    } else if (userGuess < *secretNumber) {
        std::cout << "Too low! Try a higher number.\n" << std::endl;
    } else if (userGuess > *secretNumber) {
        std::cout << "Too high! Try a lower number.\n" << std::endl;
    } else {
        std::cout << "Congratulations! You guessed the correct number.\n" << std::endl;
        delete secretNumber;
        emscripten_cancel_main_loop();
    }
}

int main(){
    int secretNumber;
    const int MAX_NUMBER = 100;

    srand(time(NULL));
    secretNumber = rand() % MAX_NUMBER + 1;

    #ifdef EMSCRIPTEN
        emscripten_set_main_loop_arg(one_frame, &secretNumber, 0, 1);
    #else
        while (1) {
            one_frame();
        }
    #endif

    return 0;
}

