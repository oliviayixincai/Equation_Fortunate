module position;

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
bool Position::near(Position &other) {
    return this.x - other.x =< 1 && this.x - other.x >= -1 && this.y - other.y =< 1 && this.y - other.y >= -1
}


Position &operator+=(int n) {
    switch (n)
    {
    case 0: break;
    case 1: moveX(-1); moveY(1); break;
    case 2: moveX(0); moveY(1); break;
    case 3: moveX(1); moveY(1); break;
    case 4: moveX(-1); moveY(0); break;
    case 5: moveX(1); moveY(0); break;
    case 6: moveX(-1); moveY(-1); break;
    case 7: moveX(0); moveY(-1); break;
    case 8: moveX(1); moveY(-1); break;
    default:
    cerr << "invalid_move" << endl;
        break;
    }
    return *this;
}

Position &operator+(int n) {
    Position temp = {x, y};
    switch (n)
    {
    case 0: break;
    case 1: temp.moveX(-1); temp.moveY(1); break;
    case 2: temp.moveX(0); temp.moveY(1); break;
    case 3: temp.moveX(1); temp.moveY(1); break;
    case 4: temp.moveX(-1); temp.moveY(0); break;
    case 5: temp.moveX(1); temp.moveY(0); break;
    case 6: temp.moveX(-1); temp.moveY(-1); break;
    case 7: temp.moveX(0); temp.moveY(-1); break;
    case 8: temp.moveX(1); temp.moveY(-1); break;
    default:
    cerr << "invalid_add" << endl;
        break;
    }
    return temp;
}