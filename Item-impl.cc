module item;

virtual int Item::getValue() {return 0;}

virtual int Gold::getValue() {return value;}

Gold::Gold(int value, Floor *theFloor): pos{0, 0}, value{value}, theFloor{theFloor} {}

void Gold::use() {
    theFloor->award(value);
    theFloor->removeItem(pos);
}

DragonHoard::DragonHoard(Floor *theFloor): theDragon{nullptr}, pos{0, 0}, value{6}, theFloor{theFloor} {}

DragonHoard::set(Position p) {
    pos = p;
}

void DragonHoard::notify() {
    delete theDragon;
    available = true;
}

void DragonHoard::use() {
    if (available) {
        theFloor->award(value);
        theFloor->removeItem(pos);
    }
}

Item &Potion::operator*=(double n) {
    effect.atk *= n;
    effect.def *= n;
    heal *= n;
}

void Potion::use() {
    if (theFloor->getPlayer()->getRace() == 'd') {
        (*this) *= 1.5;
    }
    theFloor->usePotion(*this);
    theFloor->removeItem(pos);
}
std::strong_ordering operator<=>(Item &other) {
    return pos <=> other.pos;
}