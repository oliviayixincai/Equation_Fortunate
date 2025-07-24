export module item;
import position;

export struct info {
    int atk;
    int def;
};

export class Item {

    protected:
    Position pos;
    Floor *theFloor
    Item(): pos{0, 0}, theFloor{nullptr};

    public:
    Position getPosition {return pos};
    virtual void use() = 0;
    virtual void set(Position p) {pos = p;}
    virtual int getValue();
    virtual Item &operator*=(double n) {return *this};
    std::strong_ordering operator<=>(Item &other);
};

export class Gold: public Item {
    int value;
    public:
    int getValue() override;
    Gold(int value, Floor *theFloor);
    virtual void use() override;
};

export class DragonHoard: public Gold {
    bool available = false;
    public:
    DragonHoard(Floor *theFloor);
    void use() override;
}

export class Potion: public Item {
    info effect;
    int heal;
    public:
    Potion(info effect, int heal, Floor *theFloor);
    void use() override;
    Item &operator*=(double n) override;
};
