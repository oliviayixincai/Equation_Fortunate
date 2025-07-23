module Floor;
import <iostream>;
import <fstream>;
import <sstream>;
import <algorithm>;
import <stdexcept>;

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
        std::cerr << "   Please check if the file exists and is readable." << std::endl;
        return false;
    }
    
    initializeEmpty(); // Clear existing data
    
    std::string line;
    int row = 0;
    int playerSpawns = 0;
    int stairCount = 0;
    
    std::cout << "📁 Loading floor from: " << filename << std::endl;
    
    while (std::getline(file, line) && row < FLOOR_HEIGHT) {
        // Ensure line is the right length
        if (line.length() > FLOOR_WIDTH) {
            std::cout << "⚠️  Warning: Line " << row + 1 << " too long, truncating..." << std::endl;
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
                    stairCount++;
                    std::cout << "🪜 Found stairs at (" << col << ", " << row << ")" << std::endl;
                    break;
                case '@': // Player spawn (handled by Game class)
                    map[row][col] = '.'; // Player is on floor tile
                    playerSpawns++;
                    std::cout << "👤 Found player spawn at (" << col << ", " << row << ")" << std::endl;
                    break;
                case 'H': case 'W': case 'E': case 'O': case 'M': case 'D': case 'L':
                    std::cout << "👹 Found enemy '" << cell << "' at (" << col << ", " << row << ") [TODO: spawn when Character system ready]" << std::endl;
                    break;
                case 'P':
                    std::cout << "🧪 Found potion at (" << col << ", " << row << ") [TODO: spawn when Item system ready]" << std::endl;
                    break;
                case 'G':
                    std::cout << "💰 Found gold at (" << col << ", " << row << ") [TODO: spawn when Item system ready]" << std::endl;
                    break;
                // TODO: Add enemy and item parsing when teammate implements constructors
                default:
                    // Keep the character as is (walls, doors, passages, etc.)
                    break;
            }
        }
        map[row][FLOOR_WIDTH] = '\0'; // Null terminate
        ++row;
    }
    
    file.close();
    
    // Validate floor data
    bool valid = true;
    if (stairCount == 0) {
        std::cerr << "Error: No stairs found in floor file!" << std::endl;
        valid = false;
    }
    if (stairCount > 1) {
        std::cerr << "Warning: Multiple stairs found (" << stairCount << "), using last one." << std::endl;
    }
    if (playerSpawns == 0) {
        std::cerr << "Error: No player spawn (@) found in floor file!" << std::endl;
        valid = false;
    }
    if (playerSpawns > 1) {
        std::cerr << "Warning: Multiple player spawns found (" << playerSpawns << "), using Game's logic." << std::endl;
    }
    
    if (valid) {
        std::cout << "Floor loaded successfully from: " << filename << std::endl;
        std::cout << "Floor stats: " << row << " rows, " << stairCount << " stairs, " << playerSpawns << " spawn points" << std::endl;
    } else {
        std::cerr << "Floor loaded with warnings. Game may not work correctly." << std::endl;
    }
    
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
    Position pos = enemy->getPosition(); // Fix: use standard method name
    for (auto &it : enemies) {
        if (&it == enemy) {
            map[pos.y][pos.x] = '.';
            enemies.erase(it);
            return;
        }
    }
}

void Floor::removeItem(Item* item) {
    Position pos = item->getPosition(); // Fix: use standard method name
    for (auto &it : item) {
        if (&it == enemy) {
            map[pos.y][pos.x] = '.';
            enemies.erase(it);
            return;
        }
}

// ---------------------

char Floor::atPosition(Position pos) const {
    if (!isValidPosition(pos)) {
        return ' ';
    } else {
        char cell = map[pos.y][pos.x];
        return cell;
    }
}

void Floor::update(Position pos1, Position pos2) {
    char char1 = atPosition(pos1);
    char char2 = atPosition(pos2);
    map[pos1.y][pos1.x] = char2;
    map[pos2.y][pos2.x] = char1;
}

// ----------------------

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

Item &Floor::getItemAt(const Position& pos) const {
    for (auto &n : items) {
        if (n->getPos == pos) {
            return n;
        }
    }
    return ITEM_NOTHING;
}

Character &Floor::getEnemyAt(const Position &pos) const {
    for (auto &n : enemies) {
        if (n->getPosition() == pos) {
            return *n;
        }
    }
    return ENEMY_NOTHING;
}

bool Floor::moveCharacter(Character* character, const Position& newPos) {
    if (!character || !isValidPosition(newPos)) return false;
    
    // Check if the new position is walkable
    if (!isFloorTile(newPos) && !isDoor(newPos) && !isPassage(newPos)) {
        return false; // Can't move to walls
    }
    
    // Check if there's already a character at the new position
    if (atPosition(newPos) != nullptr) {
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
    
    // Check for player first (always on top)
    if (player && player->getPosition() == pos) {
        return '@';
    }
    
    // Check for enemies using teammate's system
    Character* enemy = getEnemyAt(pos);
    if (enemy) {
        return enemy->getRace(); // Use teammate's getRace() method directly
    }
    
    // Check for items using teammate's system
    Item* item = getItemAt(pos);
    if (item) {
        // For now, use simple logic - teammate can enhance this
        if (item->getValue() > 0) {
            return 'G'; // Gold
        } else {
            return 'P'; // Potion
        }
    }
    
    // Check for stairs
    if (pos == stairPosition) {
        return '\\';
    }
    
    // Return the actual map character (leverages teammate's map loading)
    return map[pos.y][pos.x];
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