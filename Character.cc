
export module character;
import <string>;
import position;
import observer;
import item;

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
    Observer *observer;
public:
    Character(Position pos, int hp, int atk, int def, char race, Observer *observer);
    virtual ~Character() = 0;
    virtual Position getPosition() const {return pos;}
    int getHP() const {return hp;}
    virtual int getAtk() const {return atk;}
    virtual int getDef() const {return def;}
    char getRace() const {return race;}
    virtual void set(Position p) {pos = p;}
    virtual void death();
    virtual void attack(Character &onWho);
    virtual int attacked(Character &byWho);
    virtual void move(int direction) = 0;
    virtual void heal(int hp) {this->hp += hp;}

    std::strong_ordering operator<=>(Character &other);
};


export class PlayerCharacter: public Character {
protected:
    int gold = 0;
    int maxHp;
public:
    virtual PlayerCharacter *remove();
    PlayerCharacter(Position pos, int hp, int atk, int def, char race, Observer *observer);
    PlayerCharacter(Observer* observer);
    void attack(Character &onWho) override;
    void newFloor();
    void gainGold(int value) {gold += value;}
    int getGold() {return gold;}
    void death();
    virtual void useItem(Item &used);
    virtual void heal(int hp);
    void move(int direction);
};

export class Enemy: public Character {
public:
    Enemy(Observer *observer);
    Enemy(Position pos, int hp, int atk, int def, char race, Observer *observer);
    virtual bool near(Position p) {return pos.near(p);}
    virtual void death();
    virtual void move(int direction);
    virtual void attack(Character &onWho) override;
};
