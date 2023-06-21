#include <iostream>
#include <map>
#include <cmath>
#include <functional>
#include <iterator>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <numeric>
#include <conio.h>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include "City.h"
#include "Fitness.h"

using namespace std;

vector<City> createRoute(const vector<City>& cityList) {
	vector<City> route = cityList;
	random_device rd;
	mt19937 generator(rd());
	shuffle(route.begin(), route.end(), generator);
	return route;
}

vector<vector<City>> initialPopulation(int popSize, const vector<City>& cityList) {
	vector<vector<City>> population;

	for (int i = 0; i < popSize; i++) {
		population.push_back(createRoute(cityList));
	}

	return population;
}

vector<pair<int, double>> rankRoutes(const vector<vector<City>>& population) {
	map<int, double> fitnessResults;

	for (int i = 0; i < population.size(); i++) {
		fitnessResults[i] = Fitness(population[i]).routeFitness();
	}

	vector<pair<int, double>> sortedResults;
	copy(fitnessResults.begin(), fitnessResults.end(), back_inserter(sortedResults));

	sort(sortedResults.begin(), sortedResults.end(),
		[](const pair<int, double>& a, const pair<int, double>& b) {
			return a.second > b.second;
		});

	return sortedResults;
}

vector<int> selection(const vector<pair<int, double>>& popRanked, int eliteSize) {
	vector<int> selectionResults;
	vector<double> cumPerc;
	double fitnessSum = 0.0;

	for (const auto& ranked : popRanked) {
		fitnessSum += ranked.second;
	}

	double cumSum = 0.0;
	for (const auto& ranked : popRanked) {
		cumSum += ranked.second;
		cumPerc.push_back(100.0 * cumSum / fitnessSum);
	}

	for (int i = 0; i < eliteSize; i++) {
		selectionResults.push_back(popRanked[i].first);
	}

	random_device rd;
	mt19937 generator(rd());

	for (int i = 0; i < popRanked.size() - eliteSize; i++) {
		double pick = 100.0 * generate_canonical<double, 10>(generator);
		for (int j = 0; j < popRanked.size(); j++) {
			if (pick <= cumPerc[j]) {
				selectionResults.push_back(popRanked[j].first);
				break;
			}
		}
	}

	return selectionResults;
}

vector<vector<City>> matingPool(const vector<vector<City>>& population, const vector<int>& selectionResults) {
	vector<vector<City>> matingpool;

	for (int i = 0; i < selectionResults.size(); i++) {
		int index = selectionResults[i];
		matingpool.push_back(population[index]);
	}

	return matingpool;
}

vector<City> breed(const vector<City>& parent1, const vector<City>& parent2) {
	vector<City> child;
	vector<City> childP1;
	vector<City> childP2;

	int geneA = static_cast<int>(rand() % parent1.size());
	int geneB = static_cast<int>(rand() % parent1.size());

	int startGene = min(geneA, geneB);
	int endGene = max(geneA, geneB);

	for (int i = startGene; i < endGene; i++) {
		childP1.push_back(parent1[i]);
	}

	for (const auto& item : parent2) {
		if (find(childP1.begin(), childP1.end(), item) == childP1.end()) {
			childP2.push_back(item);
		}
	}

	child.insert(child.end(), childP1.begin(), childP1.end());
	child.insert(child.end(), childP2.begin(), childP2.end());

	return child;
}

vector<vector<City>> breedPopulation(const vector<vector<City>>& matingpool, int eliteSize) {
	vector<vector<City>> children;
	int length = matingpool.size() - eliteSize;
	vector<vector<City>> pool = matingpool;
	random_device rd;
	mt19937 generator(rd());
	shuffle(pool.begin(), pool.end(), generator);

	for (int i = 0; i < eliteSize; i++) {
		children.push_back(matingpool[i]);
	}

	for (int i = 0; i < length; i++) {
		vector<City> child = breed(pool[i], pool[matingpool.size() - i - 1]);
		children.push_back(child);
	}

	return children;
}

vector<City> mutate(const vector<City>& individual, double mutationRate) {
	vector<City> mutatedIndividual = individual;

	for (int swapped = 0; swapped < individual.size(); swapped++) {
		if (rand() < mutationRate) {
			int swapWith = rand() % individual.size();

			City city1 = mutatedIndividual[swapped];
			City city2 = mutatedIndividual[swapWith];

			mutatedIndividual[swapped] = city2;
			mutatedIndividual[swapWith] = city1;
		}
	}

	return mutatedIndividual;
}

vector<vector<City>> mutatePopulation(const vector<vector<City>>& population, double mutationRate) {
	vector<vector<City>> mutatedPop;

	for (int ind = 0; ind < population.size(); ind++) {
		vector<City> mutatedInd = mutate(population[ind], mutationRate);
		mutatedPop.push_back(mutatedInd);
	}

	return mutatedPop;
}

vector<vector<City>> nextGeneration(const vector<vector<City>>& currentGen, int eliteSize, double mutationRate) {
	vector<pair<int, double>> popRanked = rankRoutes(currentGen);
	vector<int> selectionResults = selection(popRanked, eliteSize);
	vector<vector<City>> matingpool = matingPool(currentGen, selectionResults);
	vector<vector<City>> children = breedPopulation(matingpool, eliteSize);
	vector<vector<City>> nextGeneration = mutatePopulation(children, mutationRate);
	return nextGeneration;
}

vector<City> geneticAlgorithm(const vector<City>& population, int popSize, int eliteSize, double mutationRate, int generations) {
	vector<vector<City>> pop = initialPopulation(popSize, population);
	double previousDistance = 1 / rankRoutes(pop)[0].second;
	double startingDistance = previousDistance;
	double currentDistance{};
	cout << "Initial route: " << previousDistance << endl;
	cout << "Press any key to initialize the algorithm...\n";
	_getch();

	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < generations; i++) {
		pop = nextGeneration(pop, eliteSize, mutationRate);
		currentDistance = 1 / rankRoutes(pop)[0].second;
		if (currentDistance < previousDistance) {
			cout << "\t" << currentDistance << endl;
			previousDistance = currentDistance;
		}
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> duration = end - start;
	cout << "Duration: " << duration.count() << "s" << endl;
	cout << "Final shortest route found: " << currentDistance << endl;
	int bestRouteIndex = rankRoutes(pop)[0].first;
	vector<City> bestRoute = pop[bestRouteIndex];
	return bestRoute;
}

void getInput(int& variable, int defaultValue) {
	while (true) {
		if (cin.peek() == '\n') {
			variable = defaultValue;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		}
		else if (!(cin >> variable)) {
			cout << "Incorrect input! Try again...\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		else {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		}
	}
}

vector<City> readCitiesFromFile(const string& filename) {
	vector<City> cityList;
	ifstream inputFile("cities/"+filename+".txt");
	if (!inputFile.is_open()) {
		throw runtime_error("Couldn't open the file: " + filename);
		_getch();
	}

	string line;
	int lineNumber = 1;
	while (getline(inputFile, line)) {
		istringstream iss(line);
		int x, y;
		if (!(iss >> x >> y)) {
			throw runtime_error("Incorrect data in row number " + to_string(lineNumber) + ": " + line);
			_getch();
		}
		cityList.push_back({ x, y });
		lineNumber++;
	}

	inputFile.close();
	return cityList;
}

int main() {
	vector<City> cityList;
	int popSize, eliteSize, generations, numberOfCities{}, inputMethod, inputX{}, inputY{};
	string fileName;
	bool validInput = false;
	double mutationRate;

	random_device rd;
	mt19937 generator(rd());
	uniform_int_distribution<mt19937::result_type> dist(100, 800);

	cout << "Traveling Salesman Problem\n\n";

	cout << "Enter the following data:\n";

	cout << "Population count [default = 100]:\t";
	getInput(popSize, 100);
	cout << "Elite population count [default = 20]:\t";
	getInput(eliteSize, 20);
	cout << "Number of generations [default = 500]:\t";
	getInput(generations, 500);
	cout << "Mutation frequency [default = 0.1]:\t";
	while (true) {
		if (cin.peek() == '\n') {
			mutationRate = 0.1;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		}
		else if (!(cin >> mutationRate)) {
			cout << "Incorrect input! Try again...\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		else {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		}
	}
	cout << "\nChoose your preferred way of generating cities:\n";
	cout << "1.Randomized points\n2.From the terminal\n3.From a file\n";
	cin >> inputMethod;
	if (inputMethod == 1 || inputMethod == 2) {
		cout << "Cities count [default = 100]:\t";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getInput(numberOfCities, 100);
	}
	while(!validInput)
	switch (inputMethod) {
		case 1:
			
			for (int i = 0; i < numberOfCities; i++) {
				int x = static_cast<int>(dist(generator));
				int y = static_cast<int>(dist(generator));
				cityList.push_back(City(x, y));
			}
			validInput = !validInput;
			break;
		case 2:
			cout << "Enter the coordinates for " << numberOfCities << " cities in the following format: <X> <Y>.\n";
			for (int i = 0; i < numberOfCities; i++) {
				cout << "Enter the coordinates of citi number " << i + 1 << ":\t";
				cin >> inputX >> inputY;
				if (cin) {
					int x = static_cast<int>(inputX);
					int y = static_cast<int>(inputY);
					cityList.push_back(City(x, y));
				}
				else {
					cout << "Incorrect input! Try again...\n";
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					i--;
				}
			}
			validInput = !validInput;
			break;
		case 3:
			cout << "Enter the name of the file without '.txt' extension: ";
			cin >> fileName;
			try {
				cityList = readCitiesFromFile(fileName);
			}
			catch (const exception& e) {
				cerr << "Error: " << e.what() << endl;
				_getch();
				return 1;
			}
			validInput = !validInput;
			break;
		default:
			cout << "Incorrect choice. Try again...\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> inputMethod;
			break;
	}
	

	vector<City> bestRoute = geneticAlgorithm(cityList, popSize, eliteSize, mutationRate, generations);

	cout << "The shortest found route goes through the following cities:\n";
	for (const auto& city : bestRoute) {
		cout << city << ", ";
	}
	cout << endl;

	ofstream outputFile("bestRouteCities.txt");
	if (outputFile.is_open()) {
		outputFile << "The shortest found route goes through the following cities:\n";
		for (const auto& city : bestRoute) {
			outputFile << city << "\n";
		}
		outputFile.close();
		cout << "Results written to bestRouteCities.txt" << endl;
	} 
	else {
		cout << "Couldn't open the file" << endl;
	}
	_getch();
	return 0;
}