module item;

virtual int Item::getValue() {return 0;}

virtual int Gold::getValue() {return value;}

Gold::Gold(int value, Game *theGame, Chamber *theChamber): pos{0, 0}, value{value}, theGame{theGame}, theChamber{theChamber} {}

void Gold::use() {theChamber->removeItem(this);}

DragonHoard::DragonHoard(int value, Game *theGame, Chamber *theChamber): theDragon{nullptr}, pos{0, 0}, value{value}, theGame{theGame}, theChamber{theChamber} {}

DragonHoard::set(Position p) {
    pos = p;
    theDragon = new Dragon{theChamber, this};
    theDragon->set(p);
    theDragon->move();
}

void DragonHoard::notify() {
    delete theDragon;
    available = true;
}

void DragonHoard::use() {
    if (available) {
        theChamber->removeItem(this);
    }
}