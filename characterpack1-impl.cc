module charpack1;
import <stdexcept>;
import prng;

PRNG prng;
bool MERCHANT_IS_HOSTILE = false;

Drow::Drow(Observer *theGame): PlayerCharacter{{0, 0}, 150, 20, 15, 'd', theGame} {}

void Drow::useItem(Item &used) {
    used *= 1.5;
    used.use();
}

Vampire::Vampire(Observer *theGame): PlayerCharacter{{0, 0}, 50, 25, 25, 'v', theGame} {}
void Vampire::attack(Character &onWho) {
if(!onWho.getPosition()) {
//        theGame->updateMessage("attacked nobody!");
//        return;
        throw (0);
    }
if (onWho.attacked(*this) != 0) {
    heal(5);
}
}

void Vampire::heal(int n) {
    hp += n;
}

Troll::Troll(Observer *theGame): PlayerCharacter{{0, 0}, 120, 25, 15, 't', theGame} {}

void Troll::move(int direction) {
    PlayerCharacter::move(direction);
    heal(5);
}

Goblin::Goblin(Observer *theGame): PlayerCharacter{{0, 0}, 110, 15, 20, 'g', theGame} {}

void Goblin::attack(Character &onWho) {
    if(!onWho.getPosition()) {
//        theGame->updateMessage("attacked nobody!");
//        return;
        throw (0);
    }
if (onWho.attacked(*this) == -1) {
    gold += 5;
}   
}

Human::Human(Observer *theFloor): Enemy{{0, 0}, 140, 20, 20, 'H', theFloor} {}

Dwarf::Dwarf(Observer *theFloor): Enemy{{0, 0}, 100, 20, 30, 'W', theFloor} {}

int Dwarf::attacked(Character &byWho) {
    if (byWho.getRace() == 'v') {
        byWho.heal(-10);
    }
    return Character::attacked(byWho);
}

Elf::Elf(Observer *theFloor): Enemy{{0, 0}, 140, 30, 10, 'E', theFloor} {}

void Elf::attack(Character &onWho) {
    Enemy::attack(onWho);
    if (onWho.getRace() != 'd') {
        Enemy::attack(onWho);
    }
}

Orc::Orc(Observer *theFloor): Enemy{{0, 0}, 180, 30, 25, 'O', theFloor} {}
void Orc::attack(Character &onWho) {
    if (onWho.getRace() == 'g') {
        atk += (atk/2);
    }
    Enemy::attack(onWho);
    atk = 30;
}

Merchant::Merchant(Observer *theFloor): Enemy{{0, 0}, 30, 70, 5, 'M', theFloor} {}

void Merchant::attack(Character &onWho) {
    if (MERCHANT_IS_HOSTILE == true) {
        Enemy::attack(onWho);
    } else {
        sell();
    }
}

void Merchant::sell() {
    // TODO: Implement merchant selling logic
}

int Merchant::attacked(Character &byWho) {
    MERCHANT_IS_HOSTILE = true;
    return Character::attacked(byWho);
}

Dragon::Dragon(Observer *theFloor, Position treasure):Enemy{{0, 0}, 150, 20, 20, 'D', theFloor}, treasure{treasure} {}
bool Dragon::near(Position p) {return pos.near(p) || treasure.near(p);}

void Dragon::move(int direction) {}

void Dragon::death() {
    observer->notify(pos, {0, 0}, 1);
}

Halfling::Halfling(Observer *theFloor): Enemy{{0, 0}, 100, 15, 20, 'L', theFloor} {}
int Halfling::attacked(Character &byWho) {
    if (prng(1) == 0) {
        return 0;
    } else {
        return Enemy::attacked(byWho);
    }
}

// Add missing base class virtual method definitions to fix linker errors
void Character::death() {
    // Default implementation (can be empty)
}

void Character::attack(Character &onWho) {
    // Default implementation (can be empty)
}