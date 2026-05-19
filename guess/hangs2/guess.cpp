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
    // Convert the void pointer back into an integer
    const auto secretNumber = static_cast<int*>(_secretNumber);

    int userGuess = 0;  // initialized to avoid garbage values.

    if (!(std::cin >> userGuess)) {
        std::cout << "MaLa cancelling game" << std::endl;
        cancel_emscripten();
    }

    if (userGuess == 0) { std::cout << "MaLa: no input yet" << std::endl; }

    std::cout << "MaLa - Your input is: " << userGuess << std::endl;
    std::cout << "The secret is: " << *secretNumber << std::endl;


/*    if (std::cin >> userGuess) {
        // Valid integer was successfully read
        std::cout << "You entered: " << userGuess << std::endl;
    } else {
        // Input failed (stream is empty, non-numeric, or null)
        std::cin.clear(); // Clear the failbit so we can try reading again
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

*/

    if (userGuess > 100 || userGuess < 1) {
        std::cerr << "Invalid input. Try again" << std::endl;
//        cancel_emscripten();
    } else if (userGuess < *secretNumber) {
        std::cout << "Too low! Try a higher number.\n" << std::endl;
    } else if (userGuess > *secretNumber) {
        std::cout << "Too high! Try a lower number.\n" << std::endl;
    } else if (userGuess == *secretNumber) {
        std::cout << "Congratulations! You guessed the correct number.\n" << std::endl;
        cancel_emscripten();
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
            one_frame(&secretNumber);
        }
    #endif

    return 0;
}

