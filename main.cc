import game;
import <iostream>;
import <string>;
#include "PRNG.h"

PRNG prng;

int main(int argc, char* argv[]) {
    try {
        uint32_t seed = getpid();
        prng.seed( seed );
        // Handle optional floor file command line argument
        if (argc > 1) {
            std::string floorFile = argv[1];
            ifstream ifs = {floorFile};         
        } else {
            FloorGen();
            ifstream ifs = {"MyFloors.txt"}; 
        }
            Game game{ifs};
            game.start();
    while (!game.isOver()) {
        game.turn();
    }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
