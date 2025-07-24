export module Floor;
import <vector>;
import <string>;
import <iostream>;
import <fstream>;
import character;
import item;
import position;
import observer;

export class Floor {
private:
    static const int FLOOR_WIDTH = 79;
    static const int FLOOR_HEIGHT = 25;
    static const Item ITEM_NOTHING = Item();
    static const Enemy ENEMY_NOTHING = Enemy();
    
    char map[FLOOR_HEIGHT][FLOOR_WIDTH + 1]; // +1 for null terminator
    std::vector<Enemy *> enemies;
    std::vector<Item *> items;
    PlayerCharacter* player;
    char occupied = '.';

public:
    Floor();
    ~Floor();
    
    void usePotion(Potion &pot);
    // Entity management
    void addPlayer(PlayerCharacter* p);
    void addEnemy(Enemy* enemy);
    void addItem(Item* item);
    void removeEnemy(Enemy* enemy);
    void removeItem(Item* item);
    

    // useful functions: ( more useful than these ^)
    char atPosition(Position pos) const;
    void update(Position pos1, Position pos2);
    void award(int value = 0);
    void printFloor();
    void updatePlayer(Position pos1, Position pos2);

    
    // Entity queries  
    Character& getEnemyAt(const Position& pos) const;
    Item& getItemAt(const Position& pos) const;
    
    // Movement and updates
    bool moveCharacter(Character* character, const Position& newPos);
    void updateEnemies(bool frozen = false);
    void processPlayerAction();
    
    // Game mechanics
    Position getStairPosition() const { return stairPosition; }
    PlayerCharacter* getPlayer() const { return player; }
    std::vector<Enemy*>& getEnemies() { return enemies; }
    std::vector<Item*>& getItems() { return items; }
    
    // Observer notifications
    void notifyObserver();
    void setObserver(Observer* obs) { observer = obs; }
    
    // Display data for Observer
    char getCell(int row, int col) const;
    char getDisplayChar(const Position& pos) const;
    int getWidth() const { return FLOOR_WIDTH; }
    int getHeight() const { return FLOOR_HEIGHT; }
    
    // Debug and utility
    void printDebugInfo() const;

    void printFloor() const;

    friend istream &operator>>(istream &in, Floor &floor);
};
