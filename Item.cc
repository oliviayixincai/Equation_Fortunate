export module item;
import <compare>;
import position;
import observer;

export struct info {
    int atk;
    int def;
};

export class Item {
    protected:
    Position pos;
    Observer *theFloor;
    
    bool available = true;

    public:
    Item(Observer *theFloor): pos{0, 0}, theFloor{theFloor} {}
    virtual ~Item() = default;
    Position getPosition() {return pos;}
    virtual void use();
    virtual void set(Position p) {pos = p;}
    virtual int getValue();
    virtual Item &operator*=(double n) {return *this;}
    void unlock() {available = true;}
};

export class Gold: public Item {
    protected:
    int value;
    public:
    int getValue() override;
    Gold(int value, Observer *theFloor);
};

export class DragonHoard: public Gold {
    public:
    DragonHoard(Observer *theFloor);
    void use() override;
};

export class Potion: public Item {
    info effect;
    int heal;
    public:
    int getHeal() {return heal;}
    info getInfo() {return effect;}
    Potion(info effect, int heal, Observer *theFloor);
    Item &operator*=(double n) override;
};
