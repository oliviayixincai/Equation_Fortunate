module decorator;

Decorator::Decorator(PlayerCharacter *next, info info): PlayerCharacter{*next}, next{next}, potion{info} {}
Decorator::~Decorator() {
    delete next;
}
PlayerCharacter *Decorator::remove() {
    potion = {0, 0};
    PlayerCharacter *temp = next->remove();
    next = nullptr;
    delete this;
    return temp; 
}
int Decorator::getAtk() const {
    return potion.atk + next->getAtk();
}
int Decorator::getDef() const{
    return potion.def + next->getDef();
}