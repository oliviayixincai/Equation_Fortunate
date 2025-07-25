export module game;
import <string>;
import <vector>;
import <fstream>;
import floor;
import prng;
import character;
import observer;
import item;
import position;

export class Game: public Observer {
    int floorNum;
    Floor *currentFloor; // Use class Forward declaration
    PlayerCharacter *player;
    bool over;
    bool enemiesFrozen;
    std::string message = "";
    std::ifstream *floorFile; // Optional floor file for command line
    std::string playerName; // Player's chosen name

public:
    Game(std::ifstream *floorFile); // Constructor with floor file
    ~Game();
    
    void notify(Position pos1, Position pos2, int who) override;
    // Core game flow
    void displayMessage();
    void start(); // Main game loop for a floor
    void processCommand(const std::string &cmd); // Legacy method
    void nextFloor();
    void quit();
    bool isOver() const;
    void gameOver();
    void restart();
    void nextLevel();
    // Turn-based system
    void turn();
    void processEnemyTurns();
    std::string getRaceName();
    // Player management
    void createPlayer(char raceChar);
    PlayerCharacter* getPlayer() const;
    
    // Floor management
    Floor *getFloor() {return currentFloor;}
    void loadFloorFromFile(const std::string& filename);
    
    // Game state
    bool areEnemiesFrozen() const;
    void toggleEnemyFreeze();
    int getFloorNumber() const;
    
    // Utility methods for command processing
    int directionToInt(const std::string& dir);
    bool isValidDirection(const std::string& dir);

    void updateMessage(const std::string& msg) {message = msg;}
    // Helper methods for enhanced gameplay
    void displayGame();
    void printEnemies();
};

