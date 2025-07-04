export module Character;
import <string>

export class Character {
protected:
    Position pos;
    int hp;
    int attack;
    int defense;
    std::string race;
public:
    Character(int x, int y, int hp, int attack, int defense, const std::string& race)
        : x{x}, y{y}, hp{hp}, attack{attack}, defense{defense}, race{race} {}
    virtual ~Character();

    int getX() const { 
        return x; 
    }
    int getY() const { 
        return y; 
    }
    int getHP() const { 
        return hp; 
    }
    int getAttack() const { 
        return attack; 
    }
    int getDefense() const { 
        return defense; 
    }
    std::string getRace() const {
        return race; 
    }

    virtual void setPosition();
    virtual void update_hp();
    virtual void attack();
};
