module item;

int Item::getValue() {return 0;}

int Gold::getValue() {return value;}

void Item::use() {
    theFloor->notify(pos, {0, 0}, 2);
}

Gold::Gold(int value, Observer *theFloor): Item{theFloor}, value{value} {}

// void Gold::use() {
//    theFloor->notify(pos, {0, 0}, 2);
//    theFloor->award(value);
//    theFloor->removeItem(pos);
// }

DragonHoard::DragonHoard(Observer *theFloor): Gold{6, theFloor} {}

void DragonHoard::use() {
    if (available) {
//      theFloor->award(value);
//      theFloor->removeItem(pos);
        theFloor->notify(pos, {0, 0}, 2);
    }
}

Item &Potion::operator*=(double n) {
    effect.atk *= n;
    effect.def *= n;
    heal *= n;
    return *this;
}

Potion::Potion(info effect, int heal, Observer *theFloor):Item{theFloor}, effect{effect}, heal{heal} {}

// void Potion::use() {
    // if (theFloor->getPlayer()->getRace() == 'd') {
    //    (*this) *= 1.5;
    // }
    // theFloor->usePotion(*this);
    // theFloor->removeItem(pos);
//     theFloor->notify(pos, {0, 0}, 2);
// }
