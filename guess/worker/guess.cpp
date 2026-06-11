#include <iostream>
#include <optional>
#include <cstdlib>
#include <time.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/bind.h>
#endif


// Define a global variable to hold the secret.
std::optional<int> secretNumber = std::nullopt;


// Only one iteration of the original loop.
void one_frame(int userGuess){
    if (secretNumber.empty()) { return {}; }

    if (userGuess < *secretNumber) {
        std::cout << "Too low! Try a higher number.\n" << std::endl;
    } else if (userGuess > *secretNumber) {
        std::cout << "Too high! Try a lower number.\n" << std::endl;
    } else if (userGuess == *secretNumber) {
        std::cout << "Congratulations! You guessed the correct number.\n" << std::endl;
        secretNumber.reset();
    };
}


int main(){
    const int MAX_NUMBER = 100;

    srand(time(NULL));
    secretNumber = rand() % MAX_NUMBER + 1;

    std::cout << "Welcome to the Number Guessing Game!" << std::endl;
    std::cout << "Please enter a guess (1-100): " << std::endl;

    return 0;
}


EMSCRIPTEN_BINDINGS(guess) {
    emscripten::function("one_frame", &one_frame);
}