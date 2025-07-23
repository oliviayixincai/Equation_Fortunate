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
    static const Item ITEM_NOTHING;
    
    char map[FLOOR_HEIGHT][FLOOR_WIDTH + 1]; // +1 for null terminator
    std::vector<Enemy> enemies;
    std::vector<Item> items;
    PlayerCharacter* player;
    Position stairPosition;
    Observer* observer;
    
public:
    Floor(Observer* obs = nullptr);
    ~Floor();
    
    // Floor loading and generation
    bool loadFromFile(const std::string& filename);
    void initializeEmpty();
    
    // Entity management
    void addPlayer(PlayerCharacter* p);
    void addEnemy(Enemy* enemy);
    void addItem(Item* item);
    void removeEnemy(Enemy* enemy);
    void removeItem(Item* item);
    

    // useful functions: ( more useful than these ^)
    char atPosition(Position pos);
    void update(Position pos1, Position pos2);
    void award(int value = 0);

    // Position queries
    bool isValidPosition(const Position& pos) const;
    bool isFloorTile(const Position& pos) const;
    bool isWall(const Position& pos) const;
    bool isDoor(const Position& pos) const;
    bool isPassage(const Position& pos) const;
    
    // Entity queries
    Character* getCharacterAt(const Position& pos) const;
    Item* getItemAt(const Position& pos) const;
    std::vector<Enemy*> getAdjacentEnemies(const Position& pos) const;
    std::vector<Item*> getAdjacentItems(const Position& pos) const;
    
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
};