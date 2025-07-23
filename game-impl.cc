module game;
import <iostream>;
import <string>;
import <vector>;
import <sstream>;
import character; // Using teammate's module
import Floor;
import observer;
// TODO: Need charpack1 module for race classes - teammate to implement

// Direction mapping for movement commands
static const std::vector<std::string> DIRECTIONS = {"nw", "no", "ne", "we", "", "ea", "sw", "so", "se"};

Game::Game() : floorNum(1), currentFloor(nullptr), player(nullptr), over(false), enemiesFrozen(false), observer(nullptr), floorFile(""), playerName("") {
    // TODO: Initialize PRNG with default seed (need teammate to implement PRNG)
    
    // Create text display observer
    observer = new TextDisplay();
    
    std::cout << "ChamberCrawler3000 initialized!" << std::endl;
}

Game::Game(const std::string& floorFile) : floorNum(1), currentFloor(nullptr), player(nullptr), over(false), enemiesFrozen(false), observer(nullptr), floorFile(floorFile), playerName("") {
    // TODO: Initialize PRNG with default seed (need teammate to implement PRNG)
    
    // Create text display observer
    observer = new TextDisplay();
    
    std::cout << "ChamberCrawler3000 initialized with floor file: " << floorFile << std::endl;
}

Game::~Game() {
    delete currentFloor;
    delete player;
    delete observer;
}

void Game::displayMessage() {
    if (observer) {
        observer->displayMessage("Welcome to ChamberCrawler3000!");
        if (!playerName.empty()) {
            observer->displayMessage("Hello, " + playerName + "!");
        }
        observer->displayMessage("Select your race: (s)hade, (d)row, (v)ampire, (g)oblin, (t)roll, or (q)uit:");
    } else {
        std::cout << "Welcome to ChamberCrawler3000!" << std::endl;
        if (!playerName.empty()) {
            std::cout << "Hello, " << playerName << "!" << std::endl;
        }
        std::cout << "Select your race: (s)hade, (d)row, (v)ampire, (g)oblin, (t)roll, or (q)uit:" << std::endl;
    }
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
    displayGame();
    
    bool floorComplete = false;
    while (!over && !floorComplete) {
        std::string cmd;
        std::cout << "> ";
        if (!std::getline(std::cin, cmd)) {
            over = true;
            break;
        }
        
        // Process player turn
        bool playerTookAction = processPlayerTurn(cmd);
        
        if (!playerTookAction) {
            continue; // Invalid command, don't advance turn
        }
        
        // Check if player reached stairs or died
        floorComplete = checkFloorComplete();
        if (over || floorComplete) break;
        
        // Process enemy turns (if player took a valid action)
        processEnemyTurns();
        
        // Check game state after enemy turns
        if (checkGameOver()) {
            over = true;
            break;
        }
        
        // Update display after turn completion
        displayGame();
    }
    
    if (floorComplete && !over) {
        nextFloor();
    }
}

bool Game::processPlayerTurn(const std::string& cmd) {
    if (cmd.empty()) return false;

    // Race selection (only when no player exists)
    if (!player) {
        if (cmd == "s" || cmd == "d" || cmd == "v" || cmd == "g" || cmd == "t") {
            createPlayer(cmd[0]);
            return false; // Don't advance turn for race selection
        } else if (cmd == "q") {
            over = true;
            return false;
        } else {
            std::cout << "Please select a valid race: (s)hade, (d)row, (v)ampire, (g)oblin, (t)roll, or (q)uit" << std::endl;
            return false;
        }
    }

    // Game commands (only when player exists)
    if (cmd == "q") {
        over = true;
        if (observer) {
            observer->displayMessage("Farewell, " + playerName + "! Thanks for playing!");
        } else {
            std::cout << "Farewell, " << playerName << "! Thanks for playing!" << std::endl;
        }
        return false;
    }

    if (cmd == "r") {
        restart();
        return false; // Don't advance turn for restart
    }

    if (cmd == "f") {
        toggleEnemyFreeze();
        return false; // Don't advance turn for freeze toggle
    }

    // Movement commands
    for (size_t i = 0; i < DIRECTIONS.size(); ++i) {
        if (!DIRECTIONS[i].empty() && cmd == DIRECTIONS[i]) {
            return attemptPlayerMove(static_cast<int>(i));
        }
    }

    // Use potion: u direction
    if (cmd.size() > 2 && cmd.substr(0, 2) == "u ") {
        std::string dir = cmd.substr(2);
        if (isValidDirection(dir)) {
            int direction = directionToInt(dir);
            return attemptUsePotion(direction);
        } else {
            if (observer) {
                observer->displayMessage("Invalid direction for potion use!");
            } else {
                std::cout << "Invalid direction for potion use!" << std::endl;
            }
            return false;
        }
    }

    // Attack: a direction
    if (cmd.size() > 2 && cmd.substr(0, 2) == "a ") {
        std::string dir = cmd.substr(2);
        if (isValidDirection(dir)) {
            int direction = directionToInt(dir);
            return attemptPlayerAttack(direction);
        } else {
            if (observer) {
                observer->displayMessage("Invalid direction for attack!");
            } else {
                std::cout << "Invalid direction for attack!" << std::endl;
            }
            return false;
        }
    }

    // Invalid command
    if (observer) {
        observer->displayMessage("Invalid command! Type 'q' to quit, 'r' to restart.");
    } else {
        std::cout << "Invalid command! Type 'q' to quit, 'r' to restart." << std::endl;
    }
    return false;
}

bool Game::attemptPlayerMove(int direction) {
    if (!player || !currentFloor) return false;
    
    // TODO: Get player's current position (need teammate to implement PlayerCharacter::getPosition())
    // Position currentPos = player->getPosition();
    // Position newPos = calculateNewPosition(currentPos, direction);
    
    // TODO: Check if move is valid (need teammate to implement Position operations)
    // if (!currentFloor->isValidPosition(newPos) || !currentFloor->isFloorTile(newPos)) {
    //     if (observer) {
    //         observer->displayMessage("You can't move there!");
    //     }
    //     return false;
    // }
    
    // TODO: Check for character collision (need Character system)
    // Character* occupant = currentFloor->getCharacterAt(newPos);
    // if (occupant) {
    //     if (observer) {
    //         observer->displayMessage("There's someone in the way!");
    //     }
    //     return false;
    // }
    
    // TODO: Actually move the player (need teammate's PlayerCharacter::move() and Position::set())
    // bool moveSuccess = currentFloor->moveCharacter(player, newPos);
    // if (moveSuccess) {
    //     // Check if player stepped on an item
    //     Item* item = currentFloor->getItemAt(newPos);
    //     if (item) {
    //         // TODO: Auto-pickup gold, show potion info (need Item system)
    //         // if (Gold* gold = dynamic_cast<Gold*>(item)) {
    //         //     player->useItem(*gold); // Auto-pickup
    //         //     currentFloor->removeItem(gold);
    //         // }
    //     }
    //     
    //     if (observer) {
    //         observer->displayMessage(playerName + " moves " + DIRECTIONS[direction]);
    //     }
    //     return true;
    // }
    
    // Placeholder implementation
    if (observer) {
        observer->displayMessage(playerName + " moves " + DIRECTIONS[direction] + " (TODO: need Position/Character system)");
    }
    return true; // Temporary - assume move succeeds for turn advancement
}

bool Game::attemptPlayerAttack(int direction) {
    if (!player || !currentFloor) return false;
    
    // TODO: Get adjacent position in attack direction (need Position system)
    // Position playerPos = player->getPosition();
    // Position attackPos = calculateNewPosition(playerPos, direction);
    
    // TODO: Check if there's an enemy to attack (need Character system)
    // Character* target = currentFloor->getCharacterAt(attackPos);
    // if (!target || target == player) {
    //     if (observer) {
    //         observer->displayMessage("There's nothing to attack there!");
    //     }
    //     return false;
    // }
    
    // TODO: Execute attack (need teammate's PlayerCharacter::attack())
    // player->attack(*target);
    
    // TODO: Check if enemy was killed and handle death (need Character::getHP())
    // if (target->getHP() <= 0) {
    //     if (observer) {
    //         observer->displayMessage("Enemy defeated!");
    //     }
    //     currentFloor->removeEnemy(static_cast<Enemy*>(target));
    // }
    
    // Placeholder implementation
    if (observer) {
        observer->displayMessage(playerName + " attacks in direction: " + DIRECTIONS[direction] + " (TODO: need Character system)");
    }
    return true; // Count as valid action for turn advancement
}

bool Game::attemptUsePotion(int direction) {
    if (!player || !currentFloor) return false;
    
    // TODO: Get adjacent position for potion use (need Position system)
    // Position playerPos = player->getPosition();
    // Position potionPos = calculateNewPosition(playerPos, direction);
    
    // TODO: Check if there's a potion there (need Item system)
    // Item* item = currentFloor->getItemAt(potionPos);
    // Potion* potion = dynamic_cast<Potion*>(item);
    // if (!potion) {
    //     if (observer) {
    //         observer->displayMessage("There's no potion there!");
    //     }
    //     return false;
    // }
    
    // TODO: Use the potion (need PlayerCharacter::useItem() and Decorator pattern)
    // player->useItem(*potion);
    // currentFloor->removeItem(potion);
    
    // Placeholder implementation
    if (observer) {
        observer->displayMessage(playerName + " uses a potion in direction: " + DIRECTIONS[direction] + " (TODO: need Item/Decorator system)");
    }
    return true; // Count as valid action for turn advancement
}

void Game::processEnemyTurns() {
    if (!currentFloor || enemiesFrozen) return;
    
    // Get all enemies and process them in order
    std::vector<Enemy*>& enemies = currentFloor->getEnemies();
    
    for (Enemy* enemy : enemies) {
        if (!enemy || !player) continue;
        
        processEnemyTurn(enemy);
    }
    
    // Update floor after all enemies have acted
    currentFloor->processPlayerAction();
}

void Game::processEnemyTurn(Enemy* enemy) {
    if (!enemy || !player || !currentFloor) return;
    
    // TODO: Get enemy and player positions (need Position system)
    // Position enemyPos = enemy->getPosition();
    // Position playerPos = player->getPosition();
    
    // TODO: Check if player is within 1-tile radius (need Position::near() or distance calculation)
    // if (enemyPos.near(playerPos)) {
    //     // Player is adjacent - attack!
    //     enemy->attack(*player);
    //     
    //     if (observer) {
    //         observer->displayMessage("Enemy attacks " + playerName + "!");
    //     }
    //     return; // Enemy attacked, doesn't move
    // }
    
    // TODO: Move enemy randomly within its chamber (need Enemy::move())
    // enemy->move();
    
    // Placeholder - just indicate enemy turn processing
    // (This won't spam because we don't have actual enemies yet)
}

bool Game::checkFloorComplete() {
    if (!player || !currentFloor) return false;
    
    // TODO: Check if player position matches stair position (need Position comparison)
    // Position playerPos = player->getPosition();
    // Position stairPos = currentFloor->getStairPosition();
    // return playerPos == stairPos;
    
    return false; // Placeholder - can't complete floor yet
}

bool Game::checkGameOver() {
    if (!player) return true;
    
    // TODO: Check if player HP <= 0 (need teammate's PlayerCharacter::getHP())
    // if (player->getHP() <= 0) {
    //     gameOver();
    //     return true;
    // }
    
    return false; // Placeholder
}

void Game::displayGame() {
    if (observer && currentFloor) {
        observer->displayGameInfo(player, floorNum);
        observer->displayFloor(currentFloor);
    } else if (currentFloor) {
        // Fallback display if observer is not available
        std::cout << "Floor " << floorNum << " - " << playerName << std::endl;
        currentFloor->printDebugInfo();
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
    // Legacy method - now delegates to processPlayerTurn
    processPlayerTurn(cmd);
}

void Game::createPlayer(char raceChar) {
    delete player; // Clean up existing player if any
    
    std::string raceName;
    
    // Set race name for display
    switch (raceChar) {
        case 's': raceName = "Shade"; break;
        case 'd': raceName = "Drow"; break;
        case 'v': raceName = "Vampire"; break;
        case 'g': raceName = "Goblin"; break;
        case 't': raceName = "Troll"; break;
        default:
            std::cout << "Invalid race!" << std::endl;
            return;
    }
    
    // TODO: Create actual race-specific player (need teammate to implement race classes in charpack1)
    // For now, create a basic PlayerCharacter - teammate will implement proper constructors
    player = new PlayerCharacter(this);
    
    std::string message = "⚔️  " + playerName + " the " + raceName + " has entered the dungeon!";
    if (observer) {
        observer->displayMessage(message);
        observer->displayMessage("Player created successfully!");
    } else {
        std::cout << message << std::endl;
        std::cout << "Player created successfully!" << std::endl;
    }
}

void Game::initializeFloor() {
    delete currentFloor;
    
    // Create floor with observer
    currentFloor = new Floor(observer);
    
    if (!floorFile.empty()) {
        // Load floor from file
        if (currentFloor->loadFromFile(floorFile)) {
            std::string message = "📁 Floor loaded from file: " + floorFile;
            if (observer) {
                observer->displayMessage(message);
            } else {
                std::cout << message << std::endl;
            }
        } else {
            std::string message = "❌ Failed to load floor from file: " + floorFile + ". Using default floor.";
            if (observer) {
                observer->displayMessage(message);
            } else {
                std::cout << message << std::endl;
            }
            currentFloor->initializeEmpty();
        }
    } else {
        // TODO: Generate random floor using RandomFloorGenerator for DLC feature
        std::string message = "🏰 " + playerName + " enters floor " + std::to_string(floorNum) + "... (TODO: implement RandomFloorGenerator)";
        if (observer) {
            observer->displayMessage(message);
        } else {
            std::cout << message << std::endl;
        }
        currentFloor->initializeEmpty();
    }
    
    // TODO: Add player to floor when Position system is ready
    if (player) {
        currentFloor->addPlayer(player);
    }
}

void Game::nextFloor() {
    floorNum++;
    
    if (floorNum > 5) {
        std::string message = "🎉 Congratulations, " + playerName + "! You have completed all 5 floors and won the game!";
        if (observer) {
            observer->displayMessage(message);
        } else {
            std::cout << message << std::endl;
        }
        // TODO: Calculate and display final score (need teammate to implement player->getGold())
        over = true;
        return;
    }
    
    std::string message = "🆙 " + playerName + " advances to floor " + std::to_string(floorNum) + "...";
    if (observer) {
        observer->displayMessage(message);
    } else {
        std::cout << message << std::endl;
    }
    
    // TODO: Strip temporary potion effects from player (need teammate to implement PlayerCharacter::remove())
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
    
    std::string message = "🔄 Game restarted. Welcome back, " + playerName + "!";
    if (observer) {
        observer->displayMessage(message);
    } else {
        std::cout << message << std::endl;
    }
    displayMessage();
}

void Game::quit() {
    over = true;
    std::string message = "👋 Thanks for playing ChamberCrawler3000, " + playerName + "!";
    if (observer) {
        observer->displayMessage(message);
    } else {
        std::cout << message << std::endl;
    }
}

bool Game::isOver() const {
    return over;
}

void Game::gameOver() {
    over = true;
    std::string message = "💀 Game Over, " + playerName + "!";
    if (observer) {
        observer->displayMessage(message);
    } else {
        std::cout << message << std::endl;
    }
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
    std::string message = "❄️  " + playerName + " " + (enemiesFrozen ? "freezes" : "unfreezes") + " all enemies!";
    if (observer) {
        observer->displayMessage(message);
    } else {
        std::cout << message << std::endl;
    }
}

int Game::getFloorNumber() const {
    return floorNum;
}

void Game::loadFloorFromFile(const std::string& filename) {
    // TODO: Implement floor file loading (my responsibility in Floor module)
    std::string message = "Loading floor from " + filename + " for " + playerName + " (TODO: implement)";
    if (observer) {
        observer->displayMessage(message);
    } else {
        std::cout << message << std::endl;
    }
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
