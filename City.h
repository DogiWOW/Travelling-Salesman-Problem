#pragma once
#ifndef CITY_H
#define CITY_H

#include <iostream>

class City {
private:
    int x;
    int y;

public:
    City(int x, int y);

    bool operator==(const City& other) const;

    double distance(const City& city);

    friend std::ostream& operator<<(std::ostream& os, const City& city);
};

#endif