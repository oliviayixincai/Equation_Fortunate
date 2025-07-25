export module observer;
import position;
export class Observer {
public:
    virtual ~Observer() = default;
    virtual void notify(Position pos1, Position pos2, int who) = 0; // who == 0: fromplayer, who == 1: from enemy, who == 2: from Item;
                                                                    // pos2 == {0 , 0} indicates death/consumption, otherwise moves from pos1 to pos2
};