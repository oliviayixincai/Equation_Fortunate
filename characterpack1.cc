import character;
export module charpack1;

export class Drow: public PlayerCharacter {
public:
    Drow(Game *theGame);
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
    Human(Chamber *theChamber);
    void death() override;
};

export class Dwarf: public Enemy {
public:
    Dwarf(Chamber *theChamber);
    int attacked(Character &byWho) override;
};

export class Elf: public Enemy {
public:
    Elf(Chamber *theChamber);
    void attack(Character &onWho) override;
};

export class Orc: public Enemy {
public:
    Orc(Chamber *theChamber);
    void attack(Character &onWho) override;
};

export class Merchant: public Enemy {
    static bool isHostile = false;
public:
    Merchant(Chamber *theChamber);
    int attacked(Character &byWho) override;
    void death() override;
};

export class Dragon: public Enemy {
    friend class DragonHoard;
    Dragon(Chamber *theChamber);
public:
    void death() override;
};

export class Halfling: public Enemy {
public:
    Halfling(Chamber *theChamber);
    int attacked() override;
};