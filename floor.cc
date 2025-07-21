class floor {
    char map[80][25];
    FloorPattern layout;
    vector<Chamber> chambers;
public:
    friend istream &operator>>(istream &in, WholeFloor &floor);
    void printFloor();
}