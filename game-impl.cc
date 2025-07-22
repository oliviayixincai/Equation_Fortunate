module game;
import <iostream>;
import <string>;
import <vector>;
import <sstream>;
module Character;
module Floor;
module observer;
import charpack1; // For race classes

// Direction mapping for movement commands
static const std::vector<std::string> DIRECTIONS = {"nw", "no", "ne", "we", "", "ea", "sw", "so", "se"};

Game::Game() : floorNum(1), currentFloor(nullptr), player(nullptr), over(false), enemiesFrozen(false), observer(nullptr), floorFile(""), playerName("") {
    // TODO: Initialize PRNG with default seed
    std::cout << "ChamberCrawler3000 initialized!" << std::endl;
}

Game::Game(const std::string& floorFile) : floorNum(1), currentFloor(nullptr), player(nullptr), over(false), enemiesFrozen(false), observer(nullptr), floorFile(floorFile), playerName("") {
    // TODO: Initialize PRNG with default seed
    std::cout << "ChamberCrawler3000 initialized with floor file: " << floorFile << std::endl;
}

Game::~Game() {
    delete currentFloor;
    delete player;
    delete observer;
}

void Game::displayMessage() {
    std::cout << "Welcome to ChamberCrawler3000!" << std::endl;
    if (!playerName.empty()) {
        std::cout << "Hello, " << playerName << "!" << std::endl;
    }
    std::cout << "Select your race: (s)hade, (d)row, (v)ampire, (g)oblin, (t)roll, or (q)uit:" << std::endl;
}

void Game::start() {
    // First, get player name if we don't have one
    if (playerName.empty()) {
        promptForPlayerName();
        if (over) return; // Player quit during name entry
    }
    
    // If no player yet, we're in race selection mode
    if (!player) {
        displayMessage();
        std::string cmd;
        while (!player && !over) {
            std::cout << "> ";
            if (!std::getline(std::cin, cmd)) {
                over = true;
                break;
            }
            processCommand(cmd);
        }
        if (over) return;
        
        // Initialize first floor after player creation
        initializeFloor();
    }
    
    // Main game loop for current floor
    if (currentFloor) currentFloor->printFloor();
    
    bool floorComplete = false;
    while (!over && !floorComplete) {
        std::string cmd;
        std::cout << "> ";
        if (!std::getline(std::cin, cmd)) {
            over = true;
            break;
        }
        
        processCommand(cmd);
        
        // TODO: Move enemies if not frozen
        if (!enemiesFrozen && player) {
            // TODO: Implement enemy movement logic
        }
        
        // TODO: Check if player reached stairs
        if (player && currentFloor) {
            // TODO: Check if player->getPosition() == currentFloor->getStairPosition()
            // If so, set floorComplete = true
        }
        
        // TODO: Check if player died
        if (player && player->getHP() <= 0) {
            std::cout << "Oh no, " << playerName << "! You have died! Game Over." << std::endl;
            // TODO: Offer play again or quit option
            over = true;
        }
        
        // Redraw floor after each command
        if (currentFloor && !over) {
            currentFloor->printFloor();
        }
    }
    
    
    if (floorComplete && !over) {
        nextFloor();
    }
}

void Game::promptForPlayerName() {
    std::cout << "🎮 Welcome, brave adventurer! What is your name?" << std::endl;
    std::cout << "Enter your name (or 'q' to quit): ";
    
    std::string name;
    if (!std::getline(std::cin, name)) {
        over = true;
        return;
    }
    
    if (name == "q" || name == "Q") {
        over = true;
        std::cout << "Goodbye!" << std::endl;
        return;
    }
    
    if (name.empty()) {
        std::cout << "Please enter a valid name!" << std::endl;
        promptForPlayerName(); // Try again
        return;
    }
    
    // Clean up the name (remove extra whitespace, limit length)
    if (name.length() > 20) {
        name = name.substr(0, 20);
        std::cout << "Name truncated to: " << name << std::endl;
    }
    
    setPlayerName(name);
    std::cout << "🌟 Nice to meet you, " << playerName << "! Let's begin your adventure!" << std::endl;
}

void Game::setPlayerName(const std::string& name) {
    playerName = name;
}

std::string Game::getPlayerName() const {
    return playerName;
}

void Game::processCommand(const std::string &cmd) {
    if (cmd.empty()) return;

    // Race selection (only when no player exists)
    if (!player) {
        if (cmd == "s" || cmd == "d" || cmd == "v" || cmd == "g" || cmd == "t") {
            createPlayer(cmd[0]);
            return;
        } else if (cmd == "q") {
            over = true;
            return;
        } else {
            std::cout << "Please select a valid race: (s)hade, (d)row, (v)ampire, (g)oblin, (t)roll, or (q)uit" << std::endl;
            return;
        }
    }

    // Game commands (only when player exists)
    if (cmd == "q") {
        over = true;
        std::cout << "Farewell, " << playerName << "! Thanks for playing!" << std::endl;
        return;
    }

    if (cmd == "r") {
        restart();
        return;
    }

    if (cmd == "f") {
        toggleEnemyFreeze();
        return;
    }

    // Movement commands
    for (size_t i = 0; i < DIRECTIONS.size(); ++i) {
        if (!DIRECTIONS[i].empty() && cmd == DIRECTIONS[i]) {
            // TODO: Move player in direction i
            std::cout << playerName << " moves " << DIRECTIONS[i] << " (TODO: implement movement)" << std::endl;
            return;
        }
    }

    // Use potion: u direction
    if (cmd.size() > 2 && cmd.substr(0, 2) == "u ") {
        std::string dir = cmd.substr(2);
        if (isValidDirection(dir)) {
            // TODO: Use potion in direction dir
            std::cout << playerName << " uses a potion in direction: " << dir << " (TODO: implement potion use)" << std::endl;
        } else {
            std::cout << "Invalid direction for potion use!" << std::endl;
        }
        return;
    }

    // Attack: a direction
    if (cmd.size() > 2 && cmd.substr(0, 2) == "a ") {
        std::string dir = cmd.substr(2);
        if (isValidDirection(dir)) {
            // TODO: Attack enemy in direction dir
            std::cout << playerName << " attacks in direction: " << dir << " (TODO: implement attack)" << std::endl;
        } else {
            std::cout << "Invalid direction for attack!" << std::endl;
        }
        return;
    }

    std::cout << "Invalid command! Type 'q' to quit, 'r' to restart." << std::endl;
}

void Game::createPlayer(char raceChar) {
    delete player; // Clean up existing player if any
    
    std::string raceName;
    int hp, maxHp, atk, def;
    
    // Set race-specific stats based on CC3k specification
    switch (raceChar) {
        case 's': // Shade
            raceName = "Shade";
            hp = maxHp = 125;
            atk = 25;
            def = 25;
            break;
        case 'd': // Drow
            raceName = "Drow";
            hp = maxHp = 150;
            atk = 25;
            def = 15;
            break;
        case 'v': // Vampire
            raceName = "Vampire";
            hp = 50;
            maxHp = 0; // Vampire has no max HP
            atk = 25;
            def = 25;
            break;
        case 'g': // Goblin
            raceName = "Goblin";
            hp = maxHp = 110;
            atk = 15;
            def = 20;
            break;
        case 't': // Troll
            raceName = "Troll";
            hp = maxHp = 120;
            atk = 25;
            def = 15;
            break;
        default:
            std::cout << "Invalid race!" << std::endl;
            return;
    }
    
    // For now, create a basic PlayerCharacter with race stats
    // TODO: Replace with actual race classes when implemented
    player = new PlayerCharacter(this);
    
    // Set the stats manually for now (TODO: move this to race constructors)
    // This is a temporary workaround
    
    std::cout << "⚔️  " << playerName << " the " << raceName << " has entered the dungeon!" << std::endl;
    std::cout << "Stats: HP=" << hp << "/" << maxHp << ", ATK=" << atk << ", DEF=" << def << std::endl;
    std::cout << "Player created successfully!" << std::endl;
}

void Game::initializeFloor() {
    delete currentFloor;
    currentFloor = new Floor();
    
    if (!floorFile.empty()) {
        // TODO: Load floor from file
        std::cout << "Loading floor from file: " << floorFile << " (TODO: implement)" << std::endl;
    } else {
        // TODO: Generate random floor
        std::cout << "🏰 " << playerName << " enters floor " << floorNum << "... (TODO: implement floor generation)" << std::endl;
    }
    
    // TODO: Spawn player, enemies, items on floor
}

void Game::nextFloor() {
    floorNum++;
    
    if (floorNum > 5) {
        std::cout << "🎉 Congratulations, " << playerName << "! You have completed all 5 floors and won the game!" << std::endl;
        // TODO: Calculate and display final score
        over = true;
        return;
    }
    
    std::cout << "🆙 " << playerName << " advances to floor " << floorNum << "..." << std::endl;
    
    // TODO: Strip temporary potion effects from player
    if (player) {
        // TODO: player = player->remove(); // Remove all decorators
    }
    
    initializeFloor();
}

void Game::restart() {
    floorNum = 1;
    delete currentFloor;
    currentFloor = nullptr;
    delete player;
    player = nullptr;
    over = false;
    enemiesFrozen = false;
    // Keep the player name for restart
    
    std::cout << "🔄 Game restarted. Welcome back, " << playerName << "!" << std::endl;
    displayMessage();
}

void Game::quit() {
    over = true;
    std::cout << "👋 Thanks for playing ChamberCrawler3000, " << playerName << "!" << std::endl;
}

bool Game::isOver() const {
    return over;
}

void Game::gameOver() {
    over = true;
    std::cout << "💀 Game Over, " << playerName << "!" << std::endl;
    // TODO: Display final score, offer play again option
}

PlayerCharacter* Game::getPlayer() const {
    return player;
}

bool Game::areEnemiesFrozen() const {
    return enemiesFrozen;
}

void Game::toggleEnemyFreeze() {
    enemiesFrozen = !enemiesFrozen;
    std::cout << "❄️  " << playerName << " " << (enemiesFrozen ? "freezes" : "unfreezes") << " all enemies!" << std::endl;
}

int Game::getFloorNumber() const {
    return floorNum;
}

void Game::loadFloorFromFile(const std::string& filename) {
    // TODO: Implement floor file loading
    std::cout << "Loading floor from " << filename << " for " << playerName << " (TODO: implement)" << std::endl;
}

int Game::directionToInt(const std::string& dir) {
    for (size_t i = 0; i < DIRECTIONS.size(); ++i) {
        if (DIRECTIONS[i] == dir) {
            return static_cast<int>(i);
        }
    }
    return -1; // Invalid direction
}

bool Game::isValidDirection(const std::string& dir) {
    return directionToInt(dir) != -1 && !dir.empty();
}
