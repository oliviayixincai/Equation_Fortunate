export module Character;
import <string>;
import position;
import item;

// Forward declaration to avoid circular dependency
export class Game;

export class Character {
protected:
    Position pos;
    int hp;
    int atk;
    int def;
    char race;
public:
    Character() = default;
    virtual ~Character() = 0;
    Position getPosition() const {return pos;}
    int getHP() const {return hp;}
    virtual int getAtk() const {return atk;}
    virtual int getDef() const {return def;} // Fixed: was 'defense'
    char getRace() const {return race;}

    virtual void death();
    virtual void useItem(Item &used);
    virtual void heal(int hp);
    virtual void attack(Character &onWho);
    virtual int attacked(Character &byWho);
    virtual void move(int direction);
};

export class PlayerCharacter: public Character {
protected:
    int gold;
    int maxHp;
    Game *theGame;
public:
    virtual PlayerCharacter *remove();
    PlayerCharacter(Game *theGame);
    void newFloor(); // Fixed: was missing parentheses
    void death() override;
    void useItem(Item &used) override;
    void heal(int hp) override;
    void attack(Character &onWho) override;
    int attacked(Character &byWho) override;
    void move(int direction) override;
    
    // Additional methods for game integration
    int getGold() const;
    void addGold(int amount);
    int getMaxHP() const;
};

export class Enemy: public Character {
    PlayerCharacter &thePlayer;
    bool isFrozen;
public:
    Enemy(PlayerCharacter &player); // Added constructor
    void death() override;
    void heal(int hp) override;
    void attack(Character &onWho) override;
    int attacked(Character &byWho) override;
    void move(int direction) override;
    
    // Enemy-specific methods
    bool isEnemyFrozen() const;
    void setFrozen(bool frozen);
}; // Fixed: missing semicolon
