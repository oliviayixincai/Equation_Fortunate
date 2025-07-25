module position;

Position Position::moveX (int x) {
    this->x += x;
    return *this;
}
Position Position::moveY (int y) {
    this->y += y;
    return *this;
}
std::strong_ordering Position::operator<=>(Position &other) {
    auto xDiff = x <=> other.x;
    auto yDiff = y <=> other.y;
    if (yDiff != 0) {
        return yDiff;
    } else {
        return xDiff;
    }
}
bool Position::near(const Position &other) const {
    return (x - other.x) <= 1 && (x - other.x) >= -1 && (y - other.y) <= 1 && (y - other.y) >= -1;
}

Position &Position::operator+=(int n) {
    switch (n)
    {
    case 0: break;
    case 1: moveX(-1); moveY(-1); break;
    case 2: moveX(0); moveY(-1); break;
    case 3: moveX(1); moveY(-1); break;
    case 4: moveX(-1); moveY(0); break;
    case 5: moveX(1); moveY(0); break;
    case 6: moveX(-1); moveY(1); break;
    case 7: moveX(0); moveY(1); break;
    case 8: moveX(1); moveY(1); break;
    default:
        break;
    }
    return *this;
}

Position Position::operator+(int n) {
    Position temp = {x, y};
    temp += n;
    return temp;
}
bool Position::operator==(Position &other) const {
    return x == other.x && y == other.y;
}
bool Position::operator!() const {
    return x == 0 && y == 0;
}