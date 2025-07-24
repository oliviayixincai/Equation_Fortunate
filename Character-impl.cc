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
virtual void PlayerCharacter::attack(Character &onWho) {
    
    if(onWho->getPosition() == {0, 0}) {
        theGame->updateMessage("attacked nobody!");
        return;
    }
    onWho.attacked(*this);
}

virtual PlayerCharacter *remove() {
    return this;
}

virtual void PlayerCharacter::useItem(Item &used) {
    if(used->getPosition() == {0, 0}) {
        theGame->updateMessage("drank nothing!");
        return;
    }
    used.use();
}

virtual void PlayerCharacter::heal(int n) {
    hp = min(hp + n, maxHp);
}

virtual void PlayerCharacter::move(int direction) {
    char cell = atPosition(pos + direction);
    if (cell == '.' || cell == '+' || cell == '#' || cell == 'P') {
    theGame->updatePlayer(pos, pos + direction);
    pos += direction;
    } else if (cell == 'G') {
        useItem(theGame->getFloor->getItemAt(pos + direction));
        theGame->getFloor->updatePlayer(pos, pos + direction);
        pos += direction;
    } else if (cell == '\\') {
        theGame->nextLevel();
        throw string{"next level"};
    } else {
        return;
    }
}

virtual void Enemy::attack(Character &onWho) {
    if (prng(1) == 1) {
    onWho.attacked(*this);
    }
}

void Enemy::death() {
    theFloor->award();
    theFloor->removeEnemy(pos);
}

virtual void Enemy::move() {
    vector<int> v;
    for (int i = 1; i <= 8; i++) {
        if(theChamber->atPosition(pos+i) == '.') {
            v.push_back(i);
        }
    }
    if (v.size != 0) {
    int index = prng(v.size());
    theFloor->update(pos, pos + v.at(index))
    pos += (v.at(index));
    }
}

std::strong_ordering Character::operator<=>(Character &other) {
    return pos <=> other.pos;
}