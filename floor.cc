export module floor;
import <vector>;
import <string>;
import <iostream>;
import <fstream>;
import character;
import charpack1;
import decorator;
import item;
import position;
import observer;

export class Floor: public Observer{
private:
    static const int FLOOR_WIDTH = 79;
    static const int FLOOR_HEIGHT = 25;

    
    char map[FLOOR_HEIGHT][FLOOR_WIDTH];
    std::vector<Enemy *> enemies;
    std::vector<Item *> items;
    PlayerCharacter *player;
    char occupied = '.';
    Observer *theGame;

    void usePotion(Potion &pot);
    // Entity management
    
    void removeEnemy(Position p);
    void removeItem(Position p);

public:
    Floor(Observer *theGame);
    ~Floor();
    
    void addPlayer(PlayerCharacter* p);
    void addEnemy(char race, Position pos);
    void addItem(char type, Position pos);
    void notify(Position pos1, Position pos2, int who) override;
    
    

    // useful functions: ( more useful than these ^)
    char atPosition(Position pos) const;
    void update(Position pos1, Position pos2);
    void award(int value = 0);
    void printFloor();
    void updatePlayer(Position pos1, Position pos2);

    
    // Entity queries  
    bool isValidPosition(const Position& pos) const;
    Enemy *getEnemyAt(Position& pos) const;
    Item *getItemAt(Position& pos) const;
    PlayerCharacter* getPlayer() const { return player;}
    void printDebugInfo() const;

    void printFloor() const;

    friend std::istream &operator>>(std::istream &in, Floor &floor);
};
