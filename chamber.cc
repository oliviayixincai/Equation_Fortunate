export module chamber;
import position;

int MAX_ENEMIES = 20;
int MAX_ITEMS = 20;

export class Chamber {
    unsigned int capacity;
    unsigned int space;
    bool containsPlayer;
    vector<Enemy> enemies;
    int nofe;
    vector<Item> items;
    int nofi;
    vector<string> content;
    public:
    void addItem (Position p);
    void addEnemy (Position p);
    Chamber(int capacity);
    ~Chamber();
};

export class SmallChamber: public Chamber {
    //15 * 6
    friend Chamber *makeSmall (int n);

};

export class MedChamber: public Chamber {
     //20 * 8
    friend Chamber *makeMed (int n);
};

export class LargeChamber: public Chamber {
    // 30 * 15
    friend Chamber *makeLarge (int n);
};

export class WholeFloor: public Chamber {
    // 80 * 25
    friend istream &operator>>(istream &in, WholeFloor &floor);
}

Chamber *makeSmall (int n);

Chamber *makeMed (int n);

Chamber *makeLarge (int n);
