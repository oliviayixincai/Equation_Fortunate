export module chamber;
import position;

int MAX_ENEMIES = 20;
int MAX_ITEMS = 20;

export class Chamber {
    unsigned int capacity;
    unsigned int space;
    bool containsPlayer;
    Enemy *enemies[MAX_ENEMIES];
    int nofe;
    Item *items[MAX_ITEM];
    int nofi;
    public:
    void addItem (Position p);
    void addEnemy (Position p);
    Chamber(int capacity);
    ~Chamber();
};

export class SmallChamber: public Chamber {
    char content[15][6];
    friend Chamber *makeSmall (int n);

};

export class MedChamber: public Chamber {
    char content[20][8];
    friend Chamber *makeMed (int n);
};

export class LargeChamber: public Chamber {
    char content[30][15];
    friend Chamber *makeLarge (int n);
};

Chamber *makeSmall (int n);

Chamber *makeMed (int n);

Chamber *makeLarge (int n);
