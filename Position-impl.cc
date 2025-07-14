module position;

int Position::getX() {
    return x;
}
int Position::getY() {
    return y;
}
Position Position::moveX (int x) {
    this.x += x;
    return *this;
}
Position Position::moveY (int y) {
    this.y += y;
    return *this;
}
std::strong_ordering Position::operator<=>(Position &other) {
    auto xDiff = x <=> other.x;
    auto yDiff = y <=> other.y;
    if (xDiff != 0) {
        return xDiff;
    } else {
        return yDiff;
    }
}