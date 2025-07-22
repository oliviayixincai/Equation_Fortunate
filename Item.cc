export module item;
import position;

export class Item {
protected:
    Position pos;
    char symbol;
    
public:
    Item(Position position, char sym) : pos(position), symbol(sym) {}
    virtual ~Item() = default;
    
    Position getPosition() const { return pos; }
    void setPosition(Position newPos) { pos = newPos; }
    char getSymbol() const { return symbol; }
    
    virtual void activate() = 0;
    virtual int getValue() const = 0;
};

export class Potion : public Item {
    int potionType; // 0-5 for different potion types
public:
    Potion(Position pos, int type) : Item(pos, 'P'), potionType(type) {}
    void activate() override;
    int getValue() const override { return 0; } // Potions have no gold value
    int getType() const { return potionType; }
};

export class Gold : public Item {
    int amount;
public:
    Gold(Position pos, int goldAmount, char sym = 'G') : Item(pos, sym), amount(goldAmount) {}
    void activate() override;
    int getValue() const override { return amount; }
};

export class DragonHoard : public Gold {
    class Dragon* guardian; // Forward declaration
public:
    DragonHoard(Position pos, Dragon* dragon) : Gold(pos, 6, 'G'), guardian(dragon) {}
    void activate() override;
};