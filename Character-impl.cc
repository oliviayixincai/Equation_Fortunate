module character;
import prng;

PRNG prng;

inline int min(int a, int b) {
    return a < b ? a : b;
}

Character::Character(Position pos, int hp, int atk, int def, char race, Observer *observer):
pos{pos},hp{hp}, atk{atk}, def{def}, race{race}, observer{observer} {}

int Character::attacked(Character &byWho) { //Damage(Def ender) = ceiling((100/(100 + Def (Def ender))) ∗Atk(Attacker))
    int n = ((100 * byWho.getAtk()) /(100 + getDef())) + ((100 * byWho.getAtk()) % (100 + getDef()) != 0);
    hp -= n;
    if (this->hp <= 0) {
        death();
        return -1;
    }
    return 1;
}
PlayerCharacter::PlayerCharacter(Position pos, int hp, int atk, int def, char race, Observer *observer):
Character{pos, hp, atk, def, race, observer}, maxHp{hp} {}
PlayerCharacter::PlayerCharacter(Observer *observer):
Character{{0,0}, 125, 25, 25, 's', observer}, maxHp{125} {}
void PlayerCharacter::death() {
    observer->notify(pos, {0, 0}, 0);
}
void PlayerCharacter::attack(Character &onWho) {
    
    if(!onWho.getPosition()) {
//        theGame->updateMessage("attacked nobody!");
//        return;
        throw (0);
    } else {
      onWho.attacked(*this);  
    }
//    onWho.attacked(*this);
}

PlayerCharacter *PlayerCharacter::remove() {
    return this;
}

void PlayerCharacter::useItem(Item &used) {
//    if(used->getPosition() == {0, 0}) {
//        theGame->updateMessage("drank nothing!");
//        return;
//    }
if (!used.getPosition()) {
    throw (0);
} else {
    used.use();
}
}

void PlayerCharacter::heal(int n) {
    hp = min(hp + n, maxHp);
}

//virtual void PlayerCharacter::move(int direction) {
//    char cell = atPosition(pos + direction);
//    if (cell == '.' || cell == '+' || cell == '#' || cell == 'P') {
//    theGame->updatePlayer(pos, pos + direction);
//    pos += direction;
//    } else if (cell == 'G') {
//      useItem(theGame->getFloor->getItemAt(pos + direction));
//        theGame->getFloor->updatePlayer(pos, pos + direction);
//        pos += direction;
//    } else if (cell == '\\') {
//        theGame->nextLevel();
//        throw string{"next level"};
//    } else {
//        return;
//   }
//}

void PlayerCharacter::move(int direction) {
    observer->notify(pos, pos+=direction, 0);
}

Enemy::Enemy(Position pos, int hp, int atk, int def, char race, Observer *observer):
Character{pos, hp, atk, def, race, observer} {}

Enemy::Enemy(Observer *observer):
Character{{0, 0}, 0, 0, 0, 'G', observer} {}
void Enemy::attack(Character &onWho) {
    if (prng(1) == 1) {
    onWho.attacked(*this);
    }
}

void Enemy::death() {
    observer->notify(pos, {0, 0}, 1);
    // observer->award();
    // observer->removeEnemy(pos);
}

void Enemy::move(int direction) {

    observer->notify(pos, pos+=direction, 1);
//    vector<int> v;
//    for (int i = 1; i <= 8; i++) {
//        if(theChamber->atPosition(pos+i) == '.') {
//          v.push_back(i);
//        }
//    }
//    if (v.size != 0) {
//    int index = prng(v.size());
//    observer->update(pos, pos + v.at(index))
//    pos += (v.at(index));
//    }
}

std::strong_ordering Character::operator<=>(Character &other) {
    return pos <=> other.pos;
}