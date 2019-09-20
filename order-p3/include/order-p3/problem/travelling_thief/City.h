#pragma once
#include <cmath>

class City {
public:
    City(int index, double x, double y);

    int getIndex() const;

    double getX() const;

    double getY() const;

    double distance(City *other);

private:
    int index;
    double x;
    double y;
};