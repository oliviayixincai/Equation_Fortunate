export module Floor;
import <vector>;
import <string>;
import <iostream>;
import Character;
import chamber;
import position;

export class Floor {
    char map[80][25];
    std::vector<Chamber*> chambers;
    PlayerCharacter* player;
    std::vector<Enemy*> enemies;
    std::vector<Item*> items;
    Position stairPosition;
    
public:
    Floor();
    ~Floor();
    
    // Floor display
    void printFloor();
    void displayGameInfo(PlayerCharacter* player);
    
    // Floor generation
    void generateFloor();
    void loadFromFile(const std::string& filename);
    void spawnPlayer(PlayerCharacter* player);
    void spawnEnemies();
    void spawnItems();
    void spawnStairs();
    
    // Game mechanics
    bool canMoveTo(Position pos);
    Character* getCharacterAt(Position pos);
    Item* getItemAt(Position pos);
    void moveCharacter(Character* character, Position newPos);
    void removeCharacter(Character* character);
    void removeItem(Item* item);
    
    // Getters
    Position getStairPosition() const;
    std::vector<Enemy*>& getEnemies();
    std::vector<Item*>& getItems();
    char getCell(int x, int y) const;
    void setCell(int x, int y, char c);
};