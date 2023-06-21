#include "City.h"
#include <cmath>

City::City(int x, int y) {
    this->x = x;
    this->y = y;
}

bool City::operator==(const City& other) const {
    return (x == other.x) && (y == other.y);
}

double City::distance(const City& city) {
    int xDis = abs(this->x - city.x);
    int yDis = abs(this->y - city.y);
    double distance = sqrt(pow(xDis, 2) + pow(yDis, 2));
    return distance;
}

std::ostream& operator<<(std::ostream& os, const City& city) {
    os << "(" << city.x << "," << city.y << ")";
    return os;
}