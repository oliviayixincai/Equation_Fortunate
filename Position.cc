export module position;
import <compare>;

export struct Position {
    int x;
    int y;

    Position moveX (int x);
    Position moveY (int y);
    std::strong_ordering operator<=>(Position &other);
    Position &operator+=(int n);
    Position operator+(int n);
    bool operator==(Position &other) const;
    bool near(const Position &other) const;
    bool operator!() const;
};