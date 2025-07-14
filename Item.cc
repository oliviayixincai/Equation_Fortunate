export module item;
import position;

export class Item {
    Position pos;

    public:
    virtual void activate() = 0;
    
}