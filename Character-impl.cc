module character;

inline int min(int a, int b) {
    return a < b ? a : b;
}

virtual void Character::attack(Character &onWho) {
    onWho.attacked(*this);
}

virtual int Character::attacked(Character &byWho) { //Damage(Def ender) = ceiling((100/(100 + Def (Def ender))) ∗Atk(Attacker))
    int n = ((100 * byWho.getAtk()) /(100 + getDef())) + ((100 * byWho.getAtk()) % (100 + getDef()) != 0)
    heal(-n);
    if (this->hp <= 0) {
        death();
        return -1;
    }
    return 1;
}

PlayerCharacter::PlayerCharacter(Game *theGame): pos{0, 0}, hp{125}, maxHp{125}, atk{25}, def{25}, theGame{theGame}, rece{'s'} {}
virtual void PlayerCharacter::death() {
    game->gameOver();
}

virtual void PlayerCharacter::useItem(Item &used) {
    gold += used.getValue();
    used.use();
}

virtual void PlayerCharacter::heal(int n) {
    hp = min(hp + n, maxHp);
}

virtual void PlayerCharacter::move(int direction) {
    pos += direction;
}

void Enemy::death() {
    theChamber->removeEnemy(this);
}

void move() {
    vector<int> v;
    for (int i = 1; i <= 8; i++) {
        if(theChamber->atPosition(pos+i) == '.') {
            v.push_back(i);
        }
    }
    
}