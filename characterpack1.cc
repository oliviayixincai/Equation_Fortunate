export module charpack1;
import character;
import observer;
import position;
import item;


export class Drow: public PlayerCharacter {
public:
    Drow(Observer *theGame);
    void useItem(Item &used) override;
};

export class Vampire: public PlayerCharacter {
public:
    Vampire(Observer *theGame);
    void attack(Character &onWho) override;
    void heal(int hp) override;
};

export class Troll: public PlayerCharacter {
public:
    Troll(Observer *theGame);
    void move(int direction) override;
};

export class Goblin: public PlayerCharacter {
public:
    Goblin(Observer *theGame);
    void attack(Character &onWho) override;
};

export class Human: public Enemy {
public:
    Human(Observer *theFloor);
    void death() override;
};

export class Dwarf: public Enemy {
public:
    Dwarf(Observer *theFloor);
    int attacked(Character &byWho) override;
};

export class Elf: public Enemy {
public:
    Elf(Observer *theFloor);
    void attack(Character &onWho) override;
};

export class Orc: public Enemy {
public:
    Orc(Observer *theFloor);
    void attack(Character &onWho) override;
};

export class Merchant: public Enemy {
    bool sold = false;
public:
    void sell();
    Merchant(Observer *theFloor);
    void attack(Character &onWho) override;
    int attacked(Character &byWho) override;
    void death() override;
};

export class Dragon: public Enemy {
public:
    Position treasure;
    bool near(Position p) override;
    Dragon(Observer *theFloor, Position treasure);
    void move(int direction) override;
    void death() override;
};

export class Halfling: public Enemy {
public:
    Halfling(Observer *theFloor);
    int attacked(Character &bywho) override;
};