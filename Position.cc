export module position;
import <compare>;

export struct Position {
    int x;
    int y;
    
    Position(int x = 0, int y = 0) : x(x), y(y) {}
    
    Position moveX(int dx);
    Position moveY(int dy);
    Position move(int dx, int dy);
    
    std::strong_ordering operator<=>(const Position &other) const;
    bool operator==(const Position &other) const;
    
    bool near(const Position &other) const;
    double distanceTo(const Position &other) const;
};