module game;
import <iostream>;
import <string>;
import <vector>;
import <sstream>;
import character; // Using teammate's module
import Floor;
import observer;
import charpack1
// TODO: Import PRNG module when available
// extern PRNG prng; // Global PRNG instance from teammate

// Temporary stub classes REMOVED - now using teammate's real implementations

// Direction mapping for movement commands (aligned with teammate's Position::operator+ numbering)
// 0=center, 1=nw, 2=no, 3=ne, 4=we, 5=ea, 6=sw, 7=so, 8=se
static const std::vector<std::string> DIRECTIONS = {"", "nw", "no", "ne", "we", "ea", "sw", "so", "se"};

Game::Game(const std::string& floorFile) : floorNum(1), currentFloor(nullptr), player(nullptr), over(false), enemiesFrozen(false), observer(nullptr), floorFile(floorFile), playerName("") {
    std::cout << "ChamberCrawler3000 initialized with floor file: " << floorFile << std::endl;
}

Game::~Game() {
    delete currentFloor;
    delete player;
}

string Game::getRaceName() {
    string race;
    switch (player->getRace())
    {
    case 's':
        race = "Shade";
        break;
    case 'd':
        race = "Drow";
        break;
    case 'v':
        race = "Vampire";
        break;
    case 'g':
        race = "Goblin";
        break;
    case 't':
        race = "Troll";
        break;
    default:
        break;
    }
    return race;
}

void Game::displayMessage() {
    cout << "Race: " << getRaceName() << " Gold: " << player->getGold() << "        " << "Floor " << floorNum << endl;
    cout << "HP: " << player->getHP() << endl;
    cout << "ATK: " << player->getAtk() << endl;
    cout << "Def: " << player->getDef() << endl;
    cout << "Actions " << message << endl;
}

void Game::start() {
    char race;
    cout << "Welcome, please select your race: (s)hade, (d)row, (v)ampire, (g)oblin, (t)roll, or (q)uit" << endl;
    cin >> race;
    if (race == 'q') {
        over = true;
        return;
    }
    createPlayer(char raceChar);
    currentFloor = new Floor;
    floorFile >> *currentFloor;
}

void Game::gameOver() {
    char cmd;
    if (floorNum > 5) {
        cout << "congratulations, you beat the game! your score is: "<< player->getGold() <<"press r to restart, q to quit" <<endl;
    }
    cout << "Game Over, your score is: "<< player->getGold() <<"press r to restart, q to quit" <<endl;
    cin >> cmd;
    if (cmd == 'r') {
        restart();
    } else {
        quit();
    }

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
    
    start();
}

void Game::quit() {
    over = true;
    std::string message = "👋 Thanks for playing ChamberCrawler3000!";
}

void Game::nextLevel {
    floorNum++;
    try {floorFile >> *currentFloor;
    } catch {
        displayGame();
    }
}

void Game::turn() {
    string cmd;
    getline(cin, cmd);

    // Game commands (only when player exists)
    if (cmd == "q") {
        over = true;
        std::cout << "Farewell, " + playerName + "! Thanks for playing!" << std::endl;
        return;
    }

    if (cmd == "r") {
        restart();
        return; // Don't advance turn for restart
    }

    if (cmd == "f") {
        toggleEnemyFreeze();
        return; // Don't advance turn for freeze toggle
    }
    
    // Add help command
    if (cmd == "h" || cmd == "help") {
        displayHelp();
        return; // Don't advance turn for help
    }
    
    // Add floor information command
    if (cmd == "info") {
        displayFloorInfo();
        return; // Don't advance turn for info
    }
    
    // Add save game stub (for future implementation)

    // Movement commands
    for (size_t i = 0; i < DIRECTIONS.size(); ++i) {
        if (!DIRECTIONS[i].empty() && cmd == DIRECTIONS[i]) {
            try {player->move(i);
            } catch string{"new level"} {

            }
        }
    }

    // Use potion: u direction
    if (cmd.size() > 2 && cmd.substr(0, 2) == "u ") {
        std::string dir = cmd.substr(2);

            int direction = isValidDirection(dir) ? directionToInt(dir) : 0;
            player->useItem(*currentFloor->getItemAt(player->getPosition + direction));
    }
    // Attack: a direction
    if (cmd.size() > 2 && cmd.substr(0, 2) == "a ") {
        std::string dir = cmd.substr(2);
        int direction = isValidDirection(dir) ? directionToInt(dir) : 0;
        player->attack(*currentFloor->getEnemyAt(player->getPosition + direction));
    }

    processEnemyTurns();
    std::sort(enemies.begin(), enemies.end(), [](Enemy* a, Enemy* b) {
    return *a < *b;
    });

    displayGame();
}


void Game::processEnemyTurns() {
    if (!currentFloor || enemiesFrozen) return;
    for (auto enemy : enemies) {
        if (enemy->getPosition().near(player->getPosition())) {
            enemy->attack(*player);
        } else if (!enemiesFrozen) {
            enemy->move;
        }
    }
}

void Game::displayGame() {
    currentFloor->printFloor;
    displayMessage();
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

void Game::createPlayer(char raceChar) {
    delete player; // Clean up existing player if any
    // Create race-specific player using teammate's charpack1 classes
    switch (raceChar) {
        case 's': player = new Shade(this); break;
        case 'd': player = new Drow(this); break;
        case 'v': player = new Vampire(this); break;
        case 'g': player = new Goblin(this); break;
        case 't': player = new Troll(this); break;
        default: player = new Shade(this); break; // Default to Shade
    }
    
    std::string message =  " the " + getRaceName() + " has entered the dungeon!";
}

bool Game::isOver() const {
    return over;
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

int Game::directionToInt(const std::string& dir) {
    for (int i = 0; i < DIRECTIONS.size(); ++i) {
        if (DIRECTIONS[i] == dir) {
            return i;
        }
    }
    return 0;
}

bool Game::isValidDirection(const std::string& dir) {
    return 1 <= directionToInt(dir) && directionToInt(dir) <= 8;
}

void Game::displayHelp() {
    if (observer) {
        observer->displayMessage("🎮 ChamberCrawler3000 Help:");
        observer->displayMessage("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        observer->displayMessage("📍 Movement: nw, no, ne, we, ea, sw, so, se");
        observer->displayMessage("⚔️  Attack: 'a <direction>' (e.g., 'a nw' to attack northwest)");
        observer->displayMessage("🧪 Use Potion: 'u <direction>' (e.g., 'u so' to use potion to the south)");
        observer->displayMessage("❄️  Freeze Enemies: 'f' (cheat mode)");
        observer->displayMessage("🔄 Restart Game: 'r'");
        observer->displayMessage("ℹ️  Floor Info: 'info'");
        observer->displayMessage("💾 Save Game: 'save' (coming soon)");
        observer->displayMessage("❓ Help: 'h' or 'help'");
        observer->displayMessage("🚪 Quit: 'q'");
        observer->displayMessage("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    } else {
        std::cout << "🎮 ChamberCrawler3000 Help:" << std::endl;
        std::cout << "Movement: nw, no, ne, we, ea, sw, so, se" << std::endl;
        std::cout << "Attack: 'a <direction>', Use Potion: 'u <direction>'" << std::endl;
        std::cout << "Special: 'f' freeze, 'r' restart, 'q' quit, 'info' floor info" << std::endl;
    }
}

void Game::displayFloorInfo() {
    if (!currentFloor) {
        if (observer) {
            observer->displayMessage("❌ No floor loaded!");
        }
        return;
    }
    
    // Count entities
    int enemyCount = currentFloor->getEnemies().size();
    int itemCount = currentFloor->getItems().size();
    
    if (observer) {
        observer->displayMessage("🏰 Floor " + std::to_string(floorNum) + " Information:");
        observer->displayMessage("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        observer->displayMessage("📏 Dimensions: " + std::to_string(currentFloor->getWidth()) + " x " + std::to_string(currentFloor->getHeight()));
        observer->displayMessage("👹 Enemies: " + std::to_string(enemyCount));
        observer->displayMessage("💎 Items: " + std::to_string(itemCount));
        observer->displayMessage("🪜 Stairs: " + std::to_string(currentFloor->getStairPosition().x) + ", " + std::to_string(currentFloor->getStairPosition().y));
        observer->displayMessage("❄️  Enemies Frozen: " + (enemiesFrozen ? "Yes" : "No"));
        if (!floorFile.empty()) {
            observer->displayMessage("📁 Loaded from: " + floorFile);
        } else {
            observer->displayMessage("🎲 Generated randomly");
        }
        observer->displayMessage("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    }
}

