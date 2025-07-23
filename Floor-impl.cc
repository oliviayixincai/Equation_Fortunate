module Floor;
import <iostream>;
import <fstream>;
import <sstream>;
import <algorithm>;

Floor::Floor(Observer* obs) : observer(obs), player(nullptr) {
    // Initialize grids for fast entity lookups
    characterGrid.resize(FLOOR_HEIGHT, std::vector<Character*>(FLOOR_WIDTH, nullptr));
    itemGrid.resize(FLOOR_HEIGHT, std::vector<Item*>(FLOOR_WIDTH, nullptr));
    
    initializeEmpty();
}

Floor::~Floor() {
    // Clean up entities (Note: we don't own the player)
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    for (Item* item : items) {
        delete item;
    }
}

void Floor::initializeEmpty() {
    // Initialize with empty floor
    for (int row = 0; row < FLOOR_HEIGHT; ++row) {
        for (int col = 0; col < FLOOR_WIDTH; ++col) {
            map[row][col] = '.'; // Default to floor tile
        }
        map[row][FLOOR_WIDTH] = '\0'; // Null terminate each row
    }
    
    // Clear entity vectors and grids
    enemies.clear();
    items.clear();
    for (auto& row : characterGrid) {
        std::fill(row.begin(), row.end(), nullptr);
    }
    for (auto& row : itemGrid) {
        std::fill(row.begin(), row.end(), nullptr);
    }
    
    // Set default stair position (bottom right corner)
    stairPosition = Position(FLOOR_WIDTH - 2, FLOOR_HEIGHT - 2);
}

bool Floor::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open floor file: " << filename << std::endl;
        return false;
    }
    
    initializeEmpty(); // Clear existing data
    
    std::string line;
    int row = 0;
    
    while (std::getline(file, line) && row < FLOOR_HEIGHT) {
        // Ensure line is the right length
        if (line.length() > FLOOR_WIDTH) {
            line = line.substr(0, FLOOR_WIDTH);
        } else {
            line.resize(FLOOR_WIDTH, ' '); // Pad with spaces
        }
        
        for (int col = 0; col < FLOOR_WIDTH; ++col) {
            char cell = line[col];
            map[row][col] = cell;
            
            // Parse special characters and create entities
            // TODO: This will be expanded when teammate implements Item and Enemy creation
            switch (cell) {
                case '\\': // Stair
                    stairPosition = Position(col, row);
                    map[row][col] = '.'; // Stairs are on floor tiles
                    break;
                case '@': // Player spawn (handled by Game class)
                    map[row][col] = '.'; // Player is on floor tile
                    break;
                // TODO: Add enemy and item parsing when teammate implements constructors
                // case 'H': case 'W': case 'E': case 'O': case 'M': case 'D': case 'L':
                //     // Enemies - will create when Enemy constructors are ready
                // case 'P': case 'G':
                //     // Items - will create when Item constructors are ready
                default:
                    // Keep the character as is (walls, doors, passages, etc.)
                    break;
            }
        }
        map[row][FLOOR_WIDTH] = '\0'; // Null terminate
        ++row;
    }
    
    file.close();
    
    std::cout << "Floor loaded from file: " << filename << std::endl;
    notifyObserver(); // Notify observer of the change
    return true;
}

void Floor::addPlayer(PlayerCharacter* p) {
    if (player && player != p) {
        // Remove old player from grid
        // TODO: Need PlayerCharacter::getPosition() from teammate
        // Position oldPos = player->getPosition();
        // if (isValidPosition(oldPos)) {
        //     characterGrid[oldPos.y][oldPos.x] = nullptr;
        // }
    }
    
    player = p;
    
    // TODO: Add player to character grid when getPosition() is available
    // Position pos = player->getPosition();
    // if (isValidPosition(pos)) {
    //     characterGrid[pos.y][pos.x] = player;
    // }
    
    notifyObserver();
}

void Floor::addEnemy(Enemy* enemy) {
    if (!enemy) return;
    
    enemies.push_back(enemy);
    
    // TODO: Add to character grid when Enemy::getPosition() is available
    // Position pos = enemy->getPosition();
    // if (isValidPosition(pos)) {
    //     characterGrid[pos.y][pos.x] = enemy;
    // }
    
    notifyObserver();
}

void Floor::addItem(Item* item) {
    if (!item) return;
    
    items.push_back(item);
    
    // TODO: Add to item grid when Item::getPosition() is available
    // Position pos = item->getPosition();
    // if (isValidPosition(pos)) {
    //     itemGrid[pos.y][pos.x] = item;
    // }
    
    notifyObserver();
}

void Floor::removeEnemy(Enemy* enemy) {
    auto it = std::find(enemies.begin(), enemies.end(), enemy);
    if (it != enemies.end()) {
        // TODO: Remove from character grid when getPosition() is available
        // Position pos = enemy->getPosition();
        // if (isValidPosition(pos)) {
        //     characterGrid[pos.y][pos.x] = nullptr;
        // }
        
        enemies.erase(it);
        delete enemy;
        notifyObserver();
    }
}

void Floor::removeItem(Item* item) {
    auto it = std::find(items.begin(), items.end(), item);
    if (it != items.end()) {
        // TODO: Remove from item grid when getPosition() is available
        // Position pos = item->getPosition();
        // if (isValidPosition(pos)) {
        //     itemGrid[pos.y][pos.x] = nullptr;
        // }
        
        items.erase(it);
        delete item;
        notifyObserver();
    }
}

bool Floor::isValidPosition(const Position& pos) const {
    return pos.x >= 0 && pos.x < FLOOR_WIDTH && pos.y >= 0 && pos.y < FLOOR_HEIGHT;
}

bool Floor::isFloorTile(const Position& pos) const {
    if (!isValidPosition(pos)) return false;
    char cell = map[pos.y][pos.x];
    return cell == '.';
}

bool Floor::isWall(const Position& pos) const {
    if (!isValidPosition(pos)) return true; // Out of bounds counts as wall
    char cell = map[pos.y][pos.x];
    return cell == '|' || cell == '-';
}

bool Floor::isDoor(const Position& pos) const {
    if (!isValidPosition(pos)) return false;
    char cell = map[pos.y][pos.x];
    return cell == '+';
}

bool Floor::isPassage(const Position& pos) const {
    if (!isValidPosition(pos)) return false;
    char cell = map[pos.y][pos.x];
    return cell == '#';
}

Character* Floor::getCharacterAt(const Position& pos) const {
    if (!isValidPosition(pos)) return nullptr;
    return characterGrid[pos.y][pos.x];
}

Item* Floor::getItemAt(const Position& pos) const {
    if (!isValidPosition(pos)) return nullptr;
    return itemGrid[pos.y][pos.x];
}

std::vector<Enemy*> Floor::getAdjacentEnemies(const Position& pos) const {
    std::vector<Enemy*> adjacent;
    
    // Check all 8 adjacent positions
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue; // Skip center position
            
            Position adjPos(pos.x + dx, pos.y + dy);
            Character* character = getCharacterAt(adjPos);
            
            // TODO: Need to check if character is an Enemy (need dynamic_cast or Enemy identification)
            // For now, check if it's not the player
            if (character && character != player) {
                // Assume it's an enemy for now
                Enemy* enemy = static_cast<Enemy*>(character);
                adjacent.push_back(enemy);
            }
        }
    }
    
    return adjacent;
}

std::vector<Item*> Floor::getAdjacentItems(const Position& pos) const {
    std::vector<Item*> adjacent;
    
    // Check all 8 adjacent positions
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue; // Skip center position
            
            Position adjPos(pos.x + dx, pos.y + dy);
            Item* item = getItemAt(adjPos);
            if (item) {
                adjacent.push_back(item);
            }
        }
    }
    
    return adjacent;
}

bool Floor::moveCharacter(Character* character, const Position& newPos) {
    if (!character || !isValidPosition(newPos)) return false;
    
    // Check if the new position is walkable
    if (!isFloorTile(newPos) && !isDoor(newPos) && !isPassage(newPos)) {
        return false; // Can't move to walls
    }
    
    // Check if there's already a character at the new position
    if (getCharacterAt(newPos) != nullptr) {
        return false; // Position occupied
    }
    
    // TODO: Get old position and update grids when getPosition() is available
    // Position oldPos = character->getPosition();
    // if (isValidPosition(oldPos)) {
    //     characterGrid[oldPos.y][oldPos.x] = nullptr;
    // }
    // 
    // characterGrid[newPos.y][newPos.x] = character;
    // 
    // // Update character's position
    // character->set(newPos);
    
    notifyObserver();
    return true;
}

void Floor::updateEnemies(bool frozen) {
    if (frozen) return; // Enemies are frozen
    
    // TODO: Move each enemy when Enemy::move() is implemented by teammate
    for (Enemy* enemy : enemies) {
        // enemy->move();
    }
    
    notifyObserver();
}

void Floor::processPlayerAction() {
    // This is called after player takes an action
    // Handle any floor-specific logic here
    notifyObserver();
}

void Floor::notifyObserver() {
    if (observer) {
        // TODO: Need Observer::notify() method from teammate
        // observer->notify();
    }
}

char Floor::getCell(int row, int col) const {
    if (row < 0 || row >= FLOOR_HEIGHT || col < 0 || col >= FLOOR_WIDTH) {
        return ' '; // Out of bounds
    }
    return map[row][col];
}

char Floor::getDisplayChar(const Position& pos) const {
    if (!isValidPosition(pos)) return ' ';
    
    // Priority: Characters > Items > Floor
    Character* character = getCharacterAt(pos);
    if (character) {
        if (character == player) {
            return '@';
        } else {
            // TODO: Need Character::getRace() or display character from teammate
            return 'E'; // Generic enemy for now
        }
    }
    
    Item* item = getItemAt(pos);
    if (item) {
        // TODO: Need Item::getSymbol() or display character from teammate
        return '?'; // Generic item for now
    }
    
    // Check for stairs
    if (pos.x == stairPosition.x && pos.y == stairPosition.y) {
        return '\\';
    }
    
    return map[pos.y][pos.x]; // Return floor tile
}

void Floor::printDebugInfo() const {
    std::cout << "=== FLOOR DEBUG INFO ===" << std::endl;
    std::cout << "Dimensions: " << FLOOR_WIDTH << "x" << FLOOR_HEIGHT << std::endl;
    std::cout << "Enemies: " << enemies.size() << std::endl;
    std::cout << "Items: " << items.size() << std::endl;
    std::cout << "Player: " << (player ? "Present" : "Not present") << std::endl;
    std::cout << "Stair position: (" << stairPosition.x << ", " << stairPosition.y << ")" << std::endl;
    std::cout << "Observer: " << (observer ? "Set" : "Not set") << std::endl;
} 