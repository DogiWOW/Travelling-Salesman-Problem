#pragma once
#ifndef FITNESS_H
#define FITNESS_H

#include "City.h"
#include <vector>

class Fitness {
private:
    std::vector<City> route;
    double distance;
    double fitness;

public:
    Fitness(const std::vector<City>& route);

    double routeDistance();

    double routeFitness();
};

#endif