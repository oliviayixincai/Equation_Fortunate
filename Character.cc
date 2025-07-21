export module Character;
import game;
import <string>;

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
    virtual int getDef() const {return defense;}
    char getRace() const {return race;}

    virtual void death();
    virtual void useItem(Item &used);
    virtual void heal(int hp);
    virtual void attack(Character &onWho);
    virtual int attacked(Character &byWho);
    virtual void move(int direction);
};

class PlayerCharacter: public Character {
protected:
    int gold;
    int maxHp;
    Game *theGame;
public:
    virtual PlayerCharacter *remove();
    PlayerCharacter(Game *theGame);
    void newFloor;
    void death();
    void useItem(Item &used);
    void heal(int hp);
    void attack(Character &onWho);
    int attacked(Character &byWho);
    void move(int direction);
};

class Enemy: public Character {
    PlayerCharacter &thePlayer;
    bool isFrozen;
public:
    void death();
    void heal(int hp);
    void attack(Character &onWho);
    int attacked(Character &byWho);
    void move(int direction);
}
