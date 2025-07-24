
export module character;
import game;
import <string>;
import position;
import item;

// Forward declaration to avoid circular dependency
export class Game;

class Floor{
    void award(int n = 0);
};

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
    virtual Position getPosition() const {return pos;}
    int getHP() const {return hp;}
    virtual int getAtk() const {return atk;}
    virtual int getDef() const {return def;}
    char getRace() const {return race;}
    virtual void set(Position p) {pos = p};

    virtual void death() = 0;
    virtual void useItem(Item &used) = 0;
    virtual void heal(int hp) = 0;
    virtual void attack(Character &onWho);
    virtual int attacked(Character &byWho);
    virtual void move(int direction) = 0;

    std::strong_ordering operator<=>(Character &other);
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
    void gainGold(int value) {gold += value;}
    int getGold() {return gold;}
    void death();
    void useItem(Item &used);
    void heal(int hp);
    void move(int direction);
};

export class Enemy: public Character {
    Floor *theFloor;
public:
    Enemy(): pos{0, 0}, theFloor{nullptr} {}
    virtual void death();
    virtual void move();
    virtual void attack(Character &onWho) override;
};
