#include <cstdlib>
#include <iostream>
#include <time.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif


void cancel_emscripten() {
    #ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
    #endif 
}


/**
 * The main loop handler called repeatedly by the browser's event loop.
 * It must have a void* parameter
 */
void one_frame(void * _secretNumber){
    // Convert back the void pointer into an integer
    const auto secretNumber = static_cast<int*>(_secretNumber);

    int userGuess;

    if (std::cin >> userGuess) {
        std::cout << "MaLa: you have entered: " << userGuess << std::endl;
    };
/*
    if (std::cin >> userGuess) {
        if (userGuess < *secretNumber) {
            std::cout << "Too low! Try a higher number.\n" << std::endl;
        } else if (userGuess > *secretNumber) {
            std::cout << "Too high! Try a lower number.\n" << std::endl;
        } else if (userGuess == *secretNumber) {
            std::cout << "Congratulations! You guessed the correct number.\n" << std::endl;
            cancel_emscripten();
        }
    };
*/
}


int main(){
    int secretNumber;
    const int MAX_NUMBER = 100;

    srand(time(NULL));
    secretNumber = rand() % MAX_NUMBER + 1;

    std::cout << "Welcome to the Number Guessing Game!" << std::endl;
    std::cout << "Please enter a guess (1-100): " << std::endl;

    #ifdef EMSCRIPTEN
        emscripten_set_main_loop_arg(one_frame, &secretNumber, 0, 1);
    #else
        while (1) {
            one_frame(&secretNumber);
        }
    #endif

    return 0;
}

