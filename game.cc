export module game;
import <string>;
import <vector>;
import <memory>;

// Forward declarations for circular dependency resolution
export class Game;

// Need to include these after Game is declared
import character; // Fixed: lowercase module name
import observer;
import item; // Add item import for new item integration
import position; // Add position import

export class Game {
    int floorNum;
    class Floor *currentFloor; // Use class Forward declaration
    PlayerCharacter *player;
    bool over;
    bool enemiesFrozen;
    Observer *observer; // For display/UI updates
    std::string floorFile; // Optional floor file for command line
    std::string playerName; // Player's chosen name

public:
    Game();
    Game(const std::string& floorFile); // Constructor with floor file
    ~Game();
    
    // Core game flow
    void displayMessage();
    void displayGame(); // Display floor and game info using observer
    void start(); // Main game loop for a floor
    void processCommand(const std::string &cmd); // Legacy method
    void nextFloor();
    void quit();
    bool isOver() const;
    void gameOver();
    void restart();
    
    // Turn-based system
    bool processPlayerTurn(const std::string& cmd);
    bool attemptPlayerMove(int direction);
    bool attemptPlayerAttack(int direction);
    bool attemptUsePotion(int direction);
    void processEnemyTurns();
    void processEnemyTurn(class Enemy* enemy);
    bool checkFloorComplete();
    bool checkGameOver();
    
    // Player management
    void createPlayer(char raceChar);
    PlayerCharacter* getPlayer() const;
    void promptForPlayerName(); // New method for name input
    void setPlayerName(const std::string& name);
    std::string getPlayerName() const;
    
    // Floor management
    void initializeFloor();
    void loadFloorFromFile(const std::string& filename);
    
    // Game state
    bool areEnemiesFrozen() const;
    void toggleEnemyFreeze();
    int getFloorNumber() const;
    
    // Utility methods for command processing
    int directionToInt(const std::string& dir);
    bool isValidDirection(const std::string& dir);
};
