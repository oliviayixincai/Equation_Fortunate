export module position;
import <compare>;

export struct Position {
    int x;
    int y;
    Position moveX (int x);
    Position moveY (int y);
    std::strong_ordering operator<=>(Position &other);
}