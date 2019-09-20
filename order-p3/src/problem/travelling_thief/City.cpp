
#include "../../../include/order-p3/problem/travelling_thief/City.h"

City::City(int index, double x, double y) : index(index), x(x), y(y) {}

int City::getIndex() const {
    return index;
}

double City::getX() const {
    return x;
}

double City::getY() const {
    return y;
}

double City::distance(City *other) {
    double xDiff = this->x - other->x;
    double yDiff = this->y - other->y;
    return sqrt(xDiff * xDiff + yDiff * yDiff);
}
