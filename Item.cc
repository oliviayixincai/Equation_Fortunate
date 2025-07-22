export module item;
import position;

struct info {
    int atk;
    int def;
}

export class Item {
    protected:
    Position pos;
    Game *theGame

    public:
    virtual void use() = 0;
    void set(Position p) {pos = p;}
}

export class Gold: public Item {
    int value;
    public:
    Gold(int value, Game *theGame);
    void use() override;
}

export class Potion: public Item {
    Effect effect;
    int heal;
    public:
    Potion(Effect effect, int heal, Game *theGame);
    void use() override;
}

export class Stair: public Item {
    public:
    Stair(Game *theGame);
    void use() override;
}