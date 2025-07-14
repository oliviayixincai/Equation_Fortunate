export module position;
import <compare>;

export class Position {
    int x;
    int y;

    public:
    int getX();
    int getY();
    Position moveX (int x);
    Position moveY (int y);
    std::strong_ordering operator<=>(Position &other);
}