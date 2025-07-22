import character;
export module decorator;

struct info {
    int atk;
    int def;
}

export class Decorator: public PlayerCharacter {
    info potion;
    PlayerCharacter *next;
public:
    Decorator();
    ~Decorator();
    PlayerCharacter *remove() override;
    int getAtk() override;
    int getDef() override;
};