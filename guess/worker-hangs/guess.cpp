#include <iostream>
#include <cstdlib>
#include <time.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif


int main(){
    int secretNumber, userGuess;
    const int MAX_NUMBER = 100;

    // Seed the random number generator using current time
    // This ensures a different random number each time the program runs  
    srand(time(NULL));
    secretNumber = rand() % MAX_NUMBER + 1;

    std::cout << "Welcome to the Number Guessing Game!" << std::endl;
    std::cout << "Please enter a guess (1-100): " << std::endl;

    while (userGuess != secretNumber) {
        if (std::cin >> userGuess) {
            if (userGuess < secretNumber) {
                std::cout << "Too low! Try a higher number.\n" << std::endl;
            } else if (userGuess > secretNumber) {
                std::cout << "Too high! Try a lower number.\n" << std::endl;
            }
            std::cout << "MaLa: you have entered: " << userGuess << std::endl;
        };
    }

    std::cout << "Congratulations! You guessed the correct number.\n" << std::endl;
    return 0;
}
