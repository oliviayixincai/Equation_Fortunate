import game;
import <iostream>;
import <string>;
import <fstream>;
import <cstdint>;
import prng;
import character;
import charpack1;
import observer;
import floor;
import decorator;
import item;
using namespace std;

PRNG prng;

int main(int argc, char* argv[]) {
    try {
        std::string floorFile;
        // Handle optional floor file command line argument
        if (argc > 1) {
            floorFile = argv[1];      
        } else {
            floorFile = "MyFloors.txt";

        }
            ifstream ifs {floorFile}; 
            cout << "file opened " << floorFile << endl;
            Game game{&ifs};
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
