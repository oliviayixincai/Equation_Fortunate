export module decorator;
import character;
import item;

export class Decorator: public PlayerCharacter {
    PlayerCharacter *next;
    info potion;
    
public:
    Decorator(PlayerCharacter *next, info info);
    ~Decorator();
    PlayerCharacter *remove() override;
    int getAtk() const override;
    int getDef() const override;
};