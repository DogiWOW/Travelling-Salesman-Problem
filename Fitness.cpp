#include "Fitness.h"

Fitness::Fitness(const std::vector<City>& route) {
    this->route = route;
    this->distance = 0.0;
    this->fitness = 0.0;
}

double Fitness::routeDistance() {
    if (distance == 0.0) {
        double pathDistance = 0.0;
        for (size_t i = 0; i < route.size(); i++) {
            City& fromCity = route[i];
            City& toCity = (i + 1 < route.size()) ? route[i + 1] : route[0];
            pathDistance += fromCity.distance(toCity);
        }
        distance = pathDistance;
    }
    return distance;
}

double Fitness::routeFitness() {
    if (fitness == 0.0) {
        fitness = 1.0 / routeDistance();
    }
    return fitness;
}