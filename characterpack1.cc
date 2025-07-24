import character;
export module charpack1;

export class Drow: public PlayerCharacter {
public:
    Drow(Game *theGame);
    void useItem(Item &used) override;
}

export class Vampire: public PlayerCharacter {
public:
    Vampire(Game *theGame);
    void attack(Character &onWho) override;
    void heal(int hp) override;
};

export class Troll: public PlayerCharacter {
public:
    Troll(Game *theGame);
    void move(int direction) override;
};

export class Goblin: public PlayerCharacter {
public:
    Goblin(Game *theGame);
    void attack() override;
};

export class Human: public Enemy {
public:
    Human(Floor *theFloor);
    void death() override;
};

export class Dwarf: public Enemy {
public:
    Dwarf(Floor *theFloor);
    int attacked(Character &byWho) override;
};

export class Elf: public Enemy {
public:
    Elf(Floor *theFloor);
    void attack(Character &onWho) override;
};

export class Orc: public Enemy {
public:
    Orc(Floor *theFloor);
    void attack(Character &onWho) override;
};

export class Merchant: public Enemy {
    static bool isHostile = false;
    bool sold = false;
public:
    Merchant(Floor *theFloor);
    int attacked(Character &byWho) override;
    void death() override;
};

export class Dragon: public Enemy {
    Observer *treasure;
    friend class DragonHoard;
    
public:
    Position getPosition() override;
    Dragon(Floor *theFloor)
    void set(Position pos) override;
    void move() override;
    void death() override;
};

export class Halfling: public Enemy {
public:
    Halfling(Floor *theFloor);
    int attacked() override;
};