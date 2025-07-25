module game;
import <iostream>;
import <string>;
import <vector>;
import <sstream>;
import <algorithm>;
import <stdexcept>;
import character; // Using teammate's module
import floor;
import charpack1;
import prng;

PRNG prng;
static const std::vector<std::string> DIRECTIONS = {"", "nw", "no", "ne", "we", "ea", "sw", "so", "se"};

Game::Game(std::ifstream *floorFile): floorNum(1), currentFloor(nullptr), player(nullptr), over(false), enemiesFrozen(false), playerName(""), floorFile{floorFile} {
    std::cout << "ChamberCrawler3000 initialized with floor file" << std::endl; 
}

Game::~Game() {
    delete currentFloor;
    delete player;
}

void Game::notify(Position pos1, Position pos2, int who) {
    if (!pos2) {
        gameOver();
    }
    currentFloor->notify(pos1, pos2, who);
}

void Game::createPlayer(char raceChar) {
    PlayerCharacter *p = nullptr;
    switch (raceChar)
    {
    case 's':
        p = new PlayerCharacter{this};
        break;
    case 'd':
        p = new Drow{this};
        break;
    case 'v':
        p = new Vampire{this};
        break;
    case 't':
        p = new Troll{this};
        break;
    case 'g':
        p = new Goblin{this};
        break;
    default:
        p = new PlayerCharacter{this};
        break;
    }
    player = p;
    updateMessage(" the " + getRaceName() + " has entered the dungeon!");
}

std::string Game::getRaceName() {
    std::string race;
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
    std::cout << "Race: " << getRaceName() << " Gold: " << player->getGold() << "        " << "Floor " << floorNum << std::endl;
    std::cout << "HP: " << player->getHP() << std::endl;
    std::cout << "ATK: " << player->getAtk() << std::endl;
    std::cout << "Def: " << player->getDef() << std::endl;
    std::cout << "Actions: " << message << std::endl;
}

void Game::start() {
    char race;
    std::string garbage;
    std::cout << "Welcome, please select your race: (s)hade, (d)row, (v)ampire, (g)oblin, (t)roll, or (q)uit" << std::endl;
    std::cin >> race;
    getline(std::cin, garbage);
    if (race == 'q') {
        over = true;
        return;
    }
    createPlayer(race);
    floorNum = 1;
    currentFloor = new Floor{this};
    currentFloor->addPlayer(player);
    *floorFile >> *currentFloor;
    displayGame();
}

void Game::gameOver() {
    char cmd;
    std::string garbage;
    if (floorNum > 5) {
        std::cout << "congratulations, you beat the game! your score is: "<< player->getGold() <<"press r to restart, q to quit" << std::endl;
    }
    std::cout << "Game Over, your score is: "<< player->getGold() <<"press r to restart, q to quit" << std::endl;
    std::cin >> cmd;
    getline(std::cin, garbage);
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
    floorFile->clear();
    floorFile->seekg(0, std::ios::beg);
    start();
}

void Game::quit() {
    over = true;
    std::cout << "Thanks for playing ChamberCrawler3000!";
}

void Game::nextLevel() {
    floorNum++;
    try {
        *floorFile >> *currentFloor;
    } catch (int e) {
        gameOver();
    }
}

void Game::turn() {
    std::string cmd;
    getline(std::cin, cmd);

    std::cout << "You entered: " << cmd << std::endl;
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
        toggleEnemyFreeze(); // Don't advance turn for freeze toggle
    }
    
    // Add help command
    if (cmd == "h" || cmd == "help") {
        printEnemies();
    }
    
    // Add floor information command
    if (cmd == "info") {
        return; // Don't advance turn for info
    }
    
    Position pos = player->getPosition();
    // Add save game stub (for future implementation)

    // Movement commands
    for (size_t i = 0; i < DIRECTIONS.size(); ++i) {
        if (cmd == DIRECTIONS.at(i)) {
            char cell = currentFloor->atPosition(pos + i);
            if (cell == '.' || cell == '+' || cell == '#') {
            player->move(i);
            } else if (cell == 'G') {
            player->useItem(*(currentFloor->getItemAt(pos + i)));
            player->move(i);
            } else if (cell == '\\') {
            nextLevel();
            if (floorNum < 6) {
            displayGame();
            }
            return;
            } else {
                updateMessage("something is on the way!");
            }
        }
    }
    std::cout << "You entered: " << cmd << std::endl;

    // Use potion: u direction
    if (cmd.size() > 2 && cmd.substr(0, 2) == "u ") {
        std::string dir = cmd.substr(2);

            int direction = isValidDirection(dir) ? directionToInt(dir) : 0;
            if (currentFloor->atPosition(pos + direction) == 'P') {
            player->useItem(*currentFloor->getItemAt(player->getPosition() + direction));
            } else {
                updateMessage("drank air and dust");
            }
    }
    // Attack: a direction
    if (cmd.size() > 2 && cmd.substr(0, 2) == "a ") {
        std::string dir = cmd.substr(2);
        int direction = isValidDirection(dir) ? directionToInt(dir) : 0;
        char cell = currentFloor->atPosition(pos + direction);
        if (cell == 'H' || cell == 'O' || cell == 'D' || cell == 'E' || cell == 'L' || cell == 'M' || cell == 'W') {
        player->attack(*currentFloor->getEnemyAt(player->getPosition() + direction));
        } else {
            updateMessage("hit nothing");
        }
    }
    std::cout << "You entered: " << cmd << std::endl;
    processEnemyTurns();
    displayGame();
}




void Game::displayGame() {
    currentFloor->printFloor();
    displayMessage();
}

bool Game::isOver() const {
    return over;
}

void Game::processEnemyTurns() {
    std::vector<Enemy *> &enemies = currentFloor->getEnemy();
    for (auto enemy : enemies) {
        if (enemy->near(player->getPosition())) {
            enemy->attack(*player);
        } else if (!enemiesFrozen) {
            Position pos = enemy->getPosition();
            std::vector<int> v;
            for (int i = 1; i <= 8; i++) {
            if(currentFloor->atPosition(pos+i) == '.') {
            v.push_back(i);
            }
            }
            if (v.size() != 0) {
            int index = prng(v.size() - 1);
            enemy->move(v.at(index));
            }
        }
    }
    std::sort(enemies.begin(), enemies.end(), [](Enemy* a, Enemy* b) {
    return *a < *b;
    });
}

PlayerCharacter* Game::getPlayer() const {
    return player;
}

bool Game::areEnemiesFrozen() const {
    return enemiesFrozen;
}

void Game::toggleEnemyFreeze() {
    enemiesFrozen = !enemiesFrozen;
    if (enemiesFrozen) {
        updateMessage("they are frozen!");
    } else {
        updateMessage("unfrozen! watch out!");
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

void Game::printEnemies() {
    std::vector<Enemy *> &enemies = currentFloor->getEnemy();
    for (auto enemy : enemies) {
        std::cout << "Enemy at " << enemy->getPosition().x << enemy->getPosition().y << " HP: " << enemy->getHP() 
                  << " ATK: " << enemy->getAtk() << " DEF: " << enemy->getDef() 
                  << std::endl;
    }
}
