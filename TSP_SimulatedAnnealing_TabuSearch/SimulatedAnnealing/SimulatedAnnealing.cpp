#include "SimulatedAnnealing.h"
#include "../DataManagement/RandomIntegerGenerator.h"
#include <iostream>
#include <chrono>
#include <random>

int SimulatedAnnealing::eval(Path path, int** costMatrix) 
{
	int cost = 0;
	for (int i = 0; i < path.length - 1; i++) {
		cost += costMatrix[path.cities[i]][path.cities[i + 1]];
	}
	cost += costMatrix[path.length - 1][0];
	return cost;
}

void SimulatedAnnealing::generateNewPath(Path sourcePath, Path newPath)
{
	// swap two places
	RandomIntegerGenerator rng(0, sourcePath.length-1);
	int idx1, idx2, tmp;
	idx1 = rng.generate();
	idx2 = rng.generate();
	for (int i = 0; i < sourcePath.length; i++) {
		newPath.cities[i] = sourcePath.cities[i];
	}
	tmp = newPath.cities[idx1];
	newPath.cities[idx1] = newPath.cities[idx2];
	newPath.cities[idx2] = tmp;
}

void SimulatedAnnealing::solveTSP(Path path, InputData inputData, int stopAfterNSeconds, float temperatureChangeParameter)
{
	std::mt19937_64 rng;
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	rng.seed(ss);
	std::uniform_real_distribution<double> unif(0, 1);

	float temperature = 5000.0;
	std::chrono::duration<float, std::milli> timer;
	int stopCriteriumInMS = stopAfterNSeconds * 1000;
	int bestCost, currentCost, newCost;
	Path bestPath, currentPath, newPath;
	currentPath = path;
	bestPath.length = currentPath.length;
	bestPath.cities = new int[bestPath.length];
	newPath.length = currentPath.length;
	newPath.cities = new int[path.length];
	for (int i = 0; i < currentPath.length; i++) bestPath.cities[i] = currentPath.cities[i];
	bestCost = eval(bestPath, inputData.costMatrix);
	currentCost = bestCost;
	auto begin = std::chrono::high_resolution_clock::now();
	double probability;
	float exponent;
	while (timer.count() < stopCriteriumInMS) {
		for (int a = 0; a < (path.length*(path.length-1))/2; a++) {
			generateNewPath(currentPath, newPath);
			newCost = eval(newPath, inputData.costMatrix);
			probability = unif(rng);
			exponent = exp((currentCost-newCost) / temperature);
			if (newCost < currentCost || probability < exponent) {
				for (int i = 0; i < newPath.length; i++) currentPath.cities[i] = newPath.cities[i];
				currentCost = newCost;
				if (newCost < bestCost) {
					bestCost = newCost;
					std::cout << "new best cost: " << bestCost << " temperature: " << temperature << std::endl;
					for (int j = 0; j < newPath.length; j++) bestPath.cities[j] = newPath.cities[j];
				}
			}
		}
		
		temperature = temperature * temperatureChangeParameter;
		auto end = std::chrono::high_resolution_clock::now();
		timer = end - begin;
	}
	std::cout << "best cost found: " << bestCost << " temperature: " << temperature << std::endl;
	std::cout << "best path: ";
	for (int i = 0; i < bestPath.length; i++) {
		std::cout << bestPath.cities[i] << "->";
	}
	std::cout << bestPath.cities[0] << std::endl;
}