module charpack1;

PRNG prng;

Drow::Drow(Game *theGame):  pos{0, 0}, hp{150}, maxHp{150}, atk{20}, def{15}, theGame{theGame}, race{'d'} {}

void Drow::useItem(Item &used) {
    gold += used.getValue();
    used *= 1.5;
    used.use();
}

Vampire::Vampire(): pos{0, 0}, hp{50}, maxHp{0}, atk{25}, def{25}, theGame{theGame}, race{'v'} {}
void Vampire::attack(Character &onWho) {
if (onWho.attacked(*this) != 0) {
    heal(5);
}
}

void Vampire::heal(int n) {
    hp += n;
}

Troll::Troll(Game *theGame): pos{0, 0}, hp{120}, maxHp{120}, atk{25}, def{15}, theGame{theGame}, race{'t'} {}

void Troll::move(int direction) {
    PlayerCharacter::move(direction);
    heal(5);
}

Goblin::Goblin(Game *theGame): pos{0, 0}, hp{110}, maxHp{110}, atk{15}, def{20}, theGame{theGame}, race{'g'} {}

void Goblin::attack() {
if (onWho.attacked(*this) == -1) {
    gold += 5;
}   
}

Human::Human(Floor *theFloor): pos{0, 0}, hp{140}, atk{20}, def{20}, theChamber{theChamber}, race{'H'} {}

void Human::death() {
    theFloor->award(4);
    theChamber->removeEnemy(this);
}

Dwarf::Dwarf(Floor *theFloor): pos{0, 0}, hp{100}, atk{20}, def{30}, theChamber{theChamber}, race{'W'} {}

int Dwarf::attacked(Character &byWho) {
    if (byWho.getRace() == 'v') {
        byWho.heal(-10);
    }
    return Character::attacked(byWho);
}

Elf::Elf(Floor *theFloor): pos{0, 0}, hp{140}, atk{30}, def{10}, theChamber{theChamber}, race{'E'} {}

void Elf::attack(Character &onWho) {
    Character::attack(onWho);
    if (onWho.getRace() != 'd') {
        Character::attack(onWho);
    }
}

Orc::Orc(Floor *theFloor): pos{0, 0}, hp{180}, atk{30}, def{25}, theChamber{theChamber}, race{'O'} {}
void Orc::attack(Character &onWho) {
    if (onWho.getRace() == 'g') {
        atk += (atk/2);
    }
    Character::attack(onWho);
    atk = 30;
}

Merchant::Merchant(Floor *theFloor): pos{0, 0}, hp{30}, atk{70}, def{5}, theChamber{theChamber}, race{'M'} {}

void Merchant::attack(Character &onWho) {
    if (isHostile == true) {
        Character::attack(onWho);
    } else if (sold == false) {
        sell();
        sold = true;
    }
}

int Merchant::attacked(Character &byWho) {
    isHostile = true;
    return Character::attacked(byWho);
}

void Merchant::death() {
    theChamber->award(4);
    theChamber->removeEnemy(this);
}

Dragon::Dragon(Floor *theFloor): observer{nullptr}, pos{0, 0}, hp{150}, atk{20}, def{20}, theChamber{theChamber}, race{'D'} {}

void Dragon::move() {}
void Dragon::set(Position p) {
    pos = p;
    for (int i = 1; i <= 8; i++) {
        if (theFloor->getItemAt(p + i)->getValue() == 6) {
            observer = theFloor->getItemAt(p + i);
            return;
        }
    }
}

Position Dragon::getPosition() {
    return observer->getPosition;
}
void Dragon::death() {
    theFloor->removeEnemy(pos);
    treasure->notify();
}

Halfling::Halfling(Floor *theFloor): pos{0, 0}, hp{100}, atk{15}, def{20}, theChamber{theChamber}, race{'L'} {}
int Halfling::attacked(Character &byWho) {
    if (prng(1) == 0) {
        return 0;
    } else {
        return Character::attacked(byWho);
    }
}