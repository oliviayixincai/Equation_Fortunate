module charpack1;

Drow::Drow(Game *theGame):  pos{0, 0}, hp{150}, maxHp{150}, atk{20}, def{15}, theGame{theGame}, race{'d'} {}

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

Human::Human(Chamber *theChamber): pos{0, 0}, hp{140}, atk{20}, def{20}, theChamber{theChamber}, race{'H'} {}

void Human::death() {
    theChamber->genGold(pos, 2);
    theChamber->genGold(pos, 2);
    theChamber->removeEnemy(this);
}

Dwarf::Dwarf(Chamber *theChamber): pos{0, 0}, hp{100}, atk{20}, def{30}, theChamber{theChamber}, race{'D'} {}

int Dwarf::attacked(Character &byWho) {
    if (byWho.getRace() == 'v') {
        byWho.heal(-10);
    }
}