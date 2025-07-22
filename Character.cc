
export module character;
import game;
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

    virtual int getDef() const {return def;}
    char getRace() const {return race;}
    void set(Position p) {pos = p};

    virtual void death() = 0;
    virtual void useItem(Item &used) = 0;
    virtual void heal(int hp) = 0;
    virtual void attack(Character &onWho);
    virtual int attacked(Character &byWho);
    virtual void move(int direction) = 0;
};

export class PlayerCharacter: public Character {
protected:
    int gold = 0;
    int maxHp;
    Game *theGame;
public:
    virtual PlayerCharacter *remove();
    PlayerCharacter(Game *theGame);
    void newFloor();

    void death();
    void useItem(Item &used);
    void heal(int hp);
    void move(int direction);
};

class Enemy: public Character {
    int code;
    Chamber *theChamber;
public:
    virtual void death();
    virtual void move();
}
