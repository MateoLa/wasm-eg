#include <cstdlib>
#include <iostream>
#include <time.h>

#include <emscripten.h>

int main(){
    int secretNumber, userGuess;
    const int MAX_NUMBER = 100;

    // Seed the random number generator using current time
    // This ensures a different random number each time the program runs  
    srand(time(NULL));
    secretNumber = rand() % MAX_NUMBER + 1;

    std::cout << "Welcome to the Number Guessing Game!\n" << std::endl;

    while (userGuess != secretNumber) {
        std::cout << "Please enter a guess (1-100): \n";
        std::cin >> userGuess; // Blocks the thread in Wasm environment
        
        if (userGuess < secretNumber) {
            std::cout << "Too low! Try a higher number.\n" << std::endl;
        } else if (userGuess > secretNumber) {
            std::cout << "Too high! Try a lower number.\n" << std::endl;
        }
    }

    std::cout << "Congratulations! You guessed the correct number.\n" << std::endl;
    return 0;
}
