export module item;
import position;

export struct info {
    int atk;
    int def;
};

export class Item {

    protected:
    Position pos;
    Game *theGame
    Chamber *theChamber;

    public:
    Position getPos {return pos};
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
    Gold(int value, Game *theGame, Chamber *theChamber);
    virtual void use() override;
};

export class DragonHoard: public Gold {
    Dragon *theDragon;
    bool available = false;
    public:
    DragonHoard(int value, Game *theGame, Chamber *theChamber);
    void use() override;
}

export class Potion: public Item {
    info effect;
    int heal;
    public:
    Potion(info effect, int heal, Game *theGame, Chamber *theChamber);
    void use() override;
    Item &operator*=(double n) override;
};

export class Stair: public Item {
    public:
    Stair(Game *theGame);
    void use() override;
};
