module game;
import <iostream>;
import <string>;
import <vector>;
import <sstream>;
import Character;
import Floor;
import observer;
import charpack1; // For race classes

// Direction mapping for movement commands
static const std::vector<std::string> DIRECTIONS = {"nw", "no", "ne", "we", "", "ea", "sw", "so", "se"};

Game::Game() : floorNum(1), currentFloor(nullptr), player(nullptr), over(false), enemiesFrozen(false), observer(nullptr), floorFile("") {
    // TODO: Initialize PRNG with default seed
    std::cout << "ChamberCrawler3000 initialized!" << std::endl;
}

Game::Game(const std::string& floorFile) : floorNum(1), currentFloor(nullptr), player(nullptr), over(false), enemiesFrozen(false), observer(nullptr), floorFile(floorFile) {
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
    std::cout << "Select your race: (s)hade, (d)row, (v)ampire, (g)oblin, (t)roll, or (q)uit:" << std::endl;
}

void Game::start() {
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
            std::cout << "You have died! Game Over." << std::endl;
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
        std::cout << "Thanks for playing!" << std::endl;
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
            std::cout << "Moving " << DIRECTIONS[i] << " (TODO: implement movement)" << std::endl;
            return;
        }
    }

    // Use potion: u direction
    if (cmd.size() > 2 && cmd.substr(0, 2) == "u ") {
        std::string dir = cmd.substr(2);
        if (isValidDirection(dir)) {
            // TODO: Use potion in direction dir
            std::cout << "Using potion in direction: " << dir << " (TODO: implement potion use)" << std::endl;
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
            std::cout << "Attacking in direction: " << dir << " (TODO: implement attack)" << std::endl;
        } else {
            std::cout << "Invalid direction for attack!" << std::endl;
        }
        return;
    }

    std::cout << "Invalid command! Type 'q' to quit, 'r' to restart." << std::endl;
}

void Game::createPlayer(char raceChar) {
    delete player; // Clean up existing player if any
    
    switch (raceChar) {
        case 's':
            // TODO: player = new Shade(this);
            std::cout << "Created Shade player (TODO: implement)" << std::endl;
            break;
        case 'd':
            // TODO: player = new Drow(this);
            std::cout << "Created Drow player (TODO: implement)" << std::endl;
            break;
        case 'v':
            // TODO: player = new Vampire(this);
            std::cout << "Created Vampire player (TODO: implement)" << std::endl;
            break;
        case 'g':
            // TODO: player = new Goblin(this);
            std::cout << "Created Goblin player (TODO: implement)" << std::endl;
            break;
        case 't':
            // TODO: player = new Troll(this);
            std::cout << "Created Troll player (TODO: implement)" << std::endl;
            break;
        default:
            std::cout << "Invalid race!" << std::endl;
            return;
    }
    
    // TODO: Create a basic PlayerCharacter for now
    // player = new PlayerCharacter(this);
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
        std::cout << "Generating random floor " << floorNum << " (TODO: implement)" << std::endl;
    }
    
    // TODO: Spawn player, enemies, items on floor
}

void Game::nextFloor() {
    floorNum++;
    
    if (floorNum > 5) {
        std::cout << "Congratulations! You have completed all 5 floors and won the game!" << std::endl;
        // TODO: Calculate and display final score
        over = true;
        return;
    }
    
    std::cout << "Advancing to floor " << floorNum << "..." << std::endl;
    
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
    
    std::cout << "Game restarted. Please select your race:" << std::endl;
    displayMessage();
}

void Game::quit() {
    over = true;
    std::cout << "Thanks for playing ChamberCrawler3000!" << std::endl;
}

bool Game::isOver() const {
    return over;
}

void Game::gameOver() {
    over = true;
    std::cout << "Game Over!" << std::endl;
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
    std::cout << "Enemies " << (enemiesFrozen ? "frozen" : "unfrozen") << std::endl;
}

int Game::getFloorNumber() const {
    return floorNum;
}

void Game::loadFloorFromFile(const std::string& filename) {
    // TODO: Implement floor file loading
    std::cout << "Loading floor from " << filename << " (TODO: implement)" << std::endl;
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
