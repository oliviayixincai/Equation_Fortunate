import game;
import <iostream>;
import <string>;
#include "PRNG.h"

PRNG prng;

int main(int argc, char* argv[]) {
    try {
        Game game;
        uint32_t seed = getpid();
        prng.seed( seed );
        // Handle optional floor file command line argument
        if (argc > 1) {
            std::string floorFile = argv[1];
            Game gameWithFile(floorFile);
            while (!gameWithFile.isOver()) {
                gameWithFile.start();
            }
        } else {
            // Standard game without floor file
            while (!game.isOver()) {
                game.start();
            }
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
