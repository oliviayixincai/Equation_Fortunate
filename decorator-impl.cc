module decorator;

Decorator::Decorator(PlayerCharacter *next, info info): next{next}, info{info} {}
Decorator::~Decorator() {
    delete next;
}
PlayerCharacter *Decorator::remove() {
    info = {0, 0};
    PlayerCharacter *temp = next->remove();
    next = nullptr;
    delete this;
    return temp; 
}
int getAtk() {
    return info.atk + next->getAtk();
}
int getDef() {
    return info.dtk + next->getDef();
}