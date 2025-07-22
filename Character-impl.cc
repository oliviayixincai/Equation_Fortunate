inline int min(int a, int b) {
    return a < b ? a : b;
}



PlayerCharacter::PlayerCharacter(Game *theGame): pos{0, 0}, hp{125}, maxHp{125}, atk{25}, def{25}, theGame{theGame}, rece{'s'} {}
virtual void PlayerCharacter::death() {
    game->gameOver();
}

virtual void PlayerCharacter::useItem(Item &used) {
    gold += used.getValue();
    used.activate();
}

virtual void PlayerCharacter::heal(int n) {
    hp = min(hp + n, maxHp);
}

virtual void PlayerCharacter::attack(Character &onWho) {
    onWho.attached(*this);
}

virtual int PlayerCharacter::attacked(Character &byWho) { //Damage(Def ender) = ceiling((100/(100 + Def (Def ender))) ∗Atk(Attacker))
    int n = ((100/(100 + getDef())) * byWho.getAtk()) + ((100 * byWho.getAtk()) % (100 + getDef()) != 0)
    heal(-n);
    if (this->hp <= 0) {
        death();
        return -1;
    }
    return 1;
}

virtual void PlayerCharacter::move(int direction) {
    switch (direction)
    {
    case 0: break;
    case 1: pos.moveX(-1); pos.moveY(1); break;
    case 2: pos.moveX(0); pos.moveY(1); break;
    case 3: pos.moveX(1); pos.moveY(1); break;
    case 4: pos.moveX(-1); pos.moveY(0); break;
    case 5: pos.moveX(1); pos.moveY(0); break;
    case 6: pos.moveX(-1); pos.moveY(-1); break;
    case 7: pos.moveX(0); pos.moveY(-1); break;
    case 8: pos.moveX(1); pos.moveY(-1); break;
    default:
    cerr << "invalid_move" << endl;
        break;
    }
}