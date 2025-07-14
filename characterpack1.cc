import character;

export class Vampire: public PlayerCharacter {
public:
    Vampire();
    void attack(Character &on Who) override;
    void heal(int hp) override;
};

export class Troll: public PlayerCharacter {
public:
    Troll();
    void move(int direction) override;
};

export class Goblin: public PlayerCharacter {
public:
    Goblin();
    void attack() override;
};

export class Human: public Enemy {
public:
    Human();
    void death() override;
};

export class Dwarf: public Enemy {
public:
    Dwarf()
    int attacked(Character &byWho) override;
};

export class Elf: public Enemy {
public:
    Elf()
    void attack() override;
};

export class Orc: public Enemy {
public:
    Orc();
    void attack() override;
};

export class Merchant: public Enemy {
    static bool isHostile = false;
public:
    Merchant();
    int attacked(Character &byWho) override;
    void death() override;
};

export class Dragon: public Enemy {
    friend class DragonHoard;
    Dragon();
public:
    void death() override;
};

export class Halfling: public Enemy {
public:
    Halfling();
    int attacked() override;
};