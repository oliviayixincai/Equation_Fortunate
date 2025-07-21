module charpack1;

Drow::Drow(Game *theGame):  pos{0, 0}, hp{150}, maxHp{150}, atk{20}, def{15}, theGame{theGame}, race{'d'} {}

Vampire::Vampire(): pos{0, 0}, hp{50}, maxHp{0}, atk{25}, def{25}, theGame{theGame}, race{'v'} {}
void Vampire::attack(Character &onWho) {
if (onWho.attached(*this) != 0) {
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

Goblin::Goblin(Game *theGame): 
void Goblin::attack();