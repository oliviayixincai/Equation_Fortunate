module Floor;
import <iostream>;
import <fstream>;
import <sstream>;
import <algorithm>;
import <stdexcept>;

Floor::Floor(PlayerCharacter *player): player(player) {}

Floor::~Floor() {
    // Clean up entities (Note: we don't own the player)
    for (auto enemy : enemies) {
        delete enemy;
    }
    for (auto item : items) {
        delete item;
    }
}

void Floor::usePotion(Potion &pot) {
    if(pot.heal != 0) {
        player->heal(pot.heal);
        return;
    } else {
        player = new Decorator{player, pot.info};
    }
}

istream &operator>>(istream &in, Floor &floor) {
    for (auto enemy : enemies) {
        delete enemy;
    }
    for (auto item : items) {
        delete item;
    }
    player->remove();
    std::string line;
    for (int i = 0; i < FLOOR_HEIGHT; ++i) {
    std::getline(in, line);
        for (int j = 0; j < FLOOR_WIDTH; ++j) {
            floor.map[i][j] = line[j];
        }
    }
    for (int i = 0; i < FLOOR_HEIGHT; ++i) {
        for (int j = 0; j < FLOOR_WIDTH; ++j) {
            char cell = floor.map[i][j];
            if (cell == '@') {
                player->set({i, j});
            } else if ('0' <= cell && cell <= '9') {
                addItem(cell, {i, j});
                floor.map[i][j]= cell > '5' ? 'G' : 'P';
            } else if ('A' <= cell && cell <= 'Z') {
                addEnemy(cell, {i, j});
            } else {
                continue;
            }
        }
    }
    return in;

}

void Floor::addPlayer(PlayerCharacter* p) {
    delete player;
    player = p; 
}

void award(int value = 0) {
    if (value == 0) {
        player->gainGold(prng(1,2));
    } else {
        player->gainGold(value);
    }
}

void Floor::addEnemy(char race, Position pos) { // H W E O M L
    switch (race)
    {
    case 'H':
        Enemy *enemy = new Human{this};
        enemy->set(pos);
        enemies.push_back(enemy);
        break;
    case 'W':
        Enemy *enemy = new Dwarf{this};
        enemy->set(pos);
        enemies.push_back(enemy);
        break;
    case 'E':
        Enemy *enemy = new Elf{this};
        enemy->set(pos);
        enemies.push_back(enemy);
        break;
    case 'O':
        Enemy *enemy = new Orc{this};
        enemy->set(pos);
        enemies.push_back(enemy);
        break;
    case 'M':
        Enemy *enemy = new Merchant{this};
        enemy->set(pos);
        enemies.push_back(enemy);
        break;
    case 'L':
        Enemy *enemy = new Halfling{this};
        enemy->set(pos);
        enemies.push_back(enemy);
        break;
    case 'D':
        Enemy *enemy = new Dragon{this};
        enemy->set(pos);
        enemies.push_back(enemy);
        break;
    default:
        break;
    }
}

void Floor::addItem(char type, Position pos) {
    switch (type)
    {
    case '0':
        Item *item = new Potion({0 , 0}, 10, this);
        item->set(pos);
        items.push_back(item);
        break;
    case '1':
        Item *item = new Potion({5 , 0}, 0, this);
        item->set(pos);
        items.push_back(item);
        break;
    case '2':
        Item *item = new Potion({0 , 5}, 0, this);
        item->set(pos);
        items.push_back(item);
    case '3':
        Item *item = new Potion({0 , 0}, -10, this);
        item->set(pos);
        items.push_back(item);
        break;
    case '4':
        Item *item = new Potion({-5 , 0}, 0, this);
        item->set(pos);
        items.push_back(item);
        break;
    case '5':
        Item *item = new Potion({0 , -5}, 0, this);
        item->set(pos);
        items.push_back(item);
        break;
    case '6':
        Item *item = new Gold{1, this};
        item->set(pos);
        items.push_back(item);
        break;
    case '7':
        Item *item = new Gold{2, this};
        item->set(pos);
        items.push_back(item);
        break;
    case '8':
        Item *item = new Gold{4, this};
        item->set(pos);
        items.push_back(item);
        break;
    case '9':
        Item *item = new DragonHoard{this};
        item->set(pos);
        items.push_back(item);
        break;
    default:
        break;
    }
}

void Floor::removeEnemy(Position p) {
    for (auto &it = enemies.begin(); it != enemies.end(); ++it) {
        if ((*it)->getPosition == p) {
            map[p.y][p.x] = '.'; 
            delete *it;           
            enemies.erase(it);    
            return;
        }
    }
}

void Floor::removeItem(Position p) {
    Position pos = item->getPosition(); // Fix: use standard method name
    for (auto &it = items.begin(); it != items.end(); ++it) {
        if (it->getPosition == p) {
            map[p.y][p.x] = '.';
            delete *it;
            items.erase(it);
            return;
        }
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

void Floor::updatePlayer(Position pos1, Position pos2) {
    char char1 = atPosition(pos1);
    char char2 = atPosition(pos2);
    map[pos1.y][pos1.x] = occupied;
    map[pos2.y][pos2.x] = char1;
    occupied = char2;
}

// ----------------------

bool Floor::isValidPosition(const Position& pos) const {
    return pos.x >= 0 && pos.x < FLOOR_WIDTH && pos.y >= 0 && pos.y < FLOOR_HEIGHT;
}

Item *Floor::getItemAt(const Position& pos) const {
    for (auto &n : items) {
        if (n->getPosition() == pos) {
            return *n;
        }
    }
    return ITEM_NOTHING;
}

Enemy *Floor::getEnemyAt(const Position &pos) const {
    for (auto &n : enemies) {
        if (n->getPosition() == pos) {
            return *n;
        }
    }
    return ENEMY_NOTHING;
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

void Floor::printFloor() {
    for (int i = 0; i < FLOOR_HEIGHT; ++i) {
        for (int j = 0; j < FLOOR_WIDTH; ++j) {
            cout << floor.map[i][j];
        }
        cout << endl;
    }
    cout << endl;
}