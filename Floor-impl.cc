module floor;
import <iostream>;
import <fstream>;
import <sstream>;
import <algorithm>;
import <stdexcept>;
import prng;

PRNG prng;

Floor::Floor(Observer *theGame): player{nullptr}, theGame{theGame}{}

void Floor::notify(Position pos1, Position pos2, int who) {
    switch (who)
    {
    case 0:
    if (!pos2) {
        theGame->notify(pos1, pos2, who);
    } else {
        updatePlayer(pos1, pos2);
    }
        break;
    case 1:
    if (!pos2) {
        char cell = atPosition(pos1);
        if (cell == 'M') {
        player->gainGold(4);
        } else if (cell == 'H') {
        player->gainGold(4);
        } else if (cell == 'D') {
        getItemAt(static_cast<Dragon*>(getEnemyAt(pos1))->treasure)->unlock();
        } else {
        player->gainGold(prng(1, 2));
        }
        removeEnemy(pos1);
    } else {
        update(pos1, pos2);
    }
    case 2:
    if (!pos2) {
        Item *item = getItemAt(pos1);
        if (!(item->getPosition())) {
            break;
        } else if (item->getValue() == 0) {
            Potion *pot = static_cast<Potion*>(item);
            usePotion(*pot);
            removeItem(pos1);
        } else {
            player->gainGold(item->getValue());
        }
    }
    default:
        break;
    }
}

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
    if(pot.getHeal() != 0) {
        player->heal(pot.getHeal());
        return;
    } else {
        player = new Decorator{player, pot.getInfo()};
    }
}

std::istream &operator>>(std::istream &in, Floor &floor) {
    for (auto enemy : floor.enemies) {
        delete enemy;
    }
    for (auto item : floor.items) {
        delete item;
    }
    floor.player->remove();
    std::string line;
    for (int i = 0; i < floor.FLOOR_HEIGHT; ++i) {
    if(!std::getline(in, line)) {
        throw 1;
    }; 
        for (int j = 0; j < floor.FLOOR_WIDTH; ++j) {
            floor.map[i][j] = line[j];
        }
    }
    for (int i = 0; i < floor.FLOOR_HEIGHT; ++i) {
        for (int j = 0; j < floor.FLOOR_WIDTH; ++j) {
            char cell = floor.map[i][j];
            if (cell == '@') {
                floor.getPlayer()->set({i, j});
            } else if ('0' <= cell && cell <= '9') {
                floor.addItem(cell, {i, j});
                floor.map[i][j]= cell > '5' ? 'G' : 'P';
            } else if ('A' <= cell && cell <= 'Z') {
                floor.addEnemy(cell, {i, j});
            } else {
                continue;
            }
        }
    }
    return in;

}

void Floor::addPlayer(PlayerCharacter* p) {
    player = p; 
}

void Floor::addEnemy(char race, Position pos) {
    Enemy *enemy = nullptr;

    switch (race) {
    case 'H':
        enemy = new Human{this};
        break;
    case 'W':
        enemy = new Dwarf{this};
        break;
    case 'E':
        enemy = new Elf{this};
        break;
    case 'O':
        enemy = new Orc{this};
        break;
    case 'M':
        enemy = new Merchant{this};
        break;
    case 'L':
        enemy = new Halfling{this};
        break;
    case 'D':
    Position t;
    for (int i = 1; i <= 8; i++) {
        
        if(atPosition(pos + i) == 'G' || atPosition(pos + i) == '9') {
            t = pos + i;
            break;
        }
    }
        enemy = new Dragon{this, t};
        break;
    default:
        return; // Invalid race, do nothing
    }
    enemies.push_back(enemy);
    enemy->set(pos);
}

void Floor::addItem(char type, Position pos) {
    Item *item = nullptr;
    switch (type)
    {
    case '0':
        item = new Potion({0 , 0}, 10, this);
        break;
    case '1':
        item = new Potion({5 , 0}, 0, this);
        break;
    case '2':
        item = new Potion({0 , 5}, 0, this);
        break;
    case '3':
        item = new Potion({0 , 0}, -10, this);
        break;
    case '4':
        item = new Potion({-5 , 0}, 0, this);
        break;
    case '5':
        item = new Potion({0 , -5}, 0, this);
        break;
    case '6':
        item = new Gold{1, this};
        break;
    case '7':
        item = new Gold{2, this};
        break;
    case '8':
        item = new Gold{4, this};
        break;
    case '9':
        item = new DragonHoard{this};
        break;
    default:
        return;
    }
    item->set(pos);
    items.push_back(item);
}

void Floor::removeEnemy(Position p) {
    for (auto it = enemies.begin(); it != enemies.end(); ++it) {
        if ((*it)->getPosition() == p) {
            map[p.y][p.x] = '.'; 
            delete *it;           
            enemies.erase(it);    
            return;
        }
    }
}

void Floor::removeItem(Position p) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if ((*it)->getPosition() == p) {
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

Item *Floor::getItemAt(Position& pos) const {
    for (auto n : items) {
        if (n->getPosition() == pos) {
            return n;
        }
    }
    return nullptr;
}

Enemy *Floor::getEnemyAt(Position &pos) const {
    for (auto n : enemies) {
        if (n->getPosition() == pos) {
            return n;
        }
    }
    return nullptr;
}

void Floor::printDebugInfo() const {
    std::cout << "=== FLOOR DEBUG INFO ===" << std::endl;
    std::cout << "Dimensions: " << FLOOR_WIDTH << "x" << FLOOR_HEIGHT << std::endl;
    std::cout << "Enemies: " << enemies.size() << std::endl;
    std::cout << "Items: " << items.size() << std::endl;
    std::cout << "Player: " << (player ? "Present" : "Not present") << std::endl;
}

void Floor::printFloor() {
    for (int i = 0; i < FLOOR_HEIGHT; ++i) {
        for (int j = 0; j < FLOOR_WIDTH; ++j) {
            std::cout << map[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}