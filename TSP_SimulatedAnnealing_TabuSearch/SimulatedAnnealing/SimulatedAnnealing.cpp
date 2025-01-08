#include "SimulatedAnnealing.h"
#include "../DataManagement/RandomIntegerGenerator.h"
#include "../Path/RandomPathGenerator.h"
#include <iostream>
#include <chrono>
#include <random>
#include <fstream>

float SimulatedAnnealing::initialTempTwoSwap(InputData inputData)
{
	/*
		Metoda zwraca temperature poczatkowa. 
		Na podstawie 10000 probek, oblicz srednia roznice kosztow rozwiazan: 
			1. Wygeneruj losowe rozwiazanie.
			2. Nastepnie stworz rozwiazanie z jej sasiedztwa (2-swap).
			3. Oblicz roznice kosztow i dodaj ja do sredniej.
		Tak oblicza srednia roznic jest temperatura poczatkowa wykorzystywana przez algorytm.
	*/
	float difference = 0.0;
	for (int a = 0; a < 10000; a++) {
		Path p = RandomPathGenerator::generate(inputData.numberOfCities);
		int cost = eval(p, inputData.costMatrix);
		twoSwap(p, p);
		int newCost = eval(p, inputData.costMatrix);
		difference += abs(cost - newCost);
		delete[] p.cities;
	}
	difference = difference / 10000.0;
	return -1 * difference / log(0.99);
}

float SimulatedAnnealing::initialTempTwoOpt(InputData inputData)
{
	/*
		Metoda zwraca temperature poczatkowa.
		Na podstawie 10000 probek, oblicz srednia roznice kosztow rozwiazan:
			1. Wygeneruj losowe rozwiazanie.
			2. Nastepnie stworz rozwiazanie z jej sasiedztwa (2-opt).
			3. Oblicz roznice kosztow i dodaj ja do sredniej.
		Tak oblicza srednia roznic jest temperatura poczatkowa wykorzystywana przez algorytm.
	*/
	float difference = 0.0;
	for (int a = 0; a < 10000; a++) {
		Path p = RandomPathGenerator::generate(inputData.numberOfCities);
		int cost = eval(p, inputData.costMatrix);
		Path n;
		n.length = p.length;
		n.cities = new int[n.length];
		for (int i = 0; i < n.length; i++) n.cities[i] = p.cities[i];
		twoOpt(p, n);
		int newCost = eval(n, inputData.costMatrix);
		difference += abs(cost - newCost);
		delete[] p.cities;
		delete[] n.cities;
	}
	difference = difference / 10000.0;
	return -1 * difference / log(0.99);
}

int SimulatedAnnealing::eval(Path path, int** costMatrix) 
{
	// Oblicz koszt sciezki.
	int cost = 0;
	for (int i = 0; i < path.length - 1; i++) {
		cost += costMatrix[path.cities[i]][path.cities[i + 1]];
	}
	cost += costMatrix[path.cities[path.length - 1]][path.cities[0]];
	return cost;
}

void SimulatedAnnealing::twoSwap(Path sourcePath, Path newPath)
{
	// Generowanie sasiedztwa za pomoca zamiany dwoch miast w sciezce.
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

void SimulatedAnnealing::twoOpt(Path sourcePath, Path newPath)
{
	// Generowanie sasiedztwa za pomoca odwracania sciezki pomiedzy dwoma miastami.
	RandomIntegerGenerator rng(0, sourcePath.length - 1);
	int idx1, idx2, tmp;
	idx1 = rng.generate();
	idx2 = rng.generate();
	while (idx2 == idx1) idx2 = rng.generate();
	if (idx1 > idx2) {
		tmp = idx1;
		idx1 = idx2;
		idx2 = tmp;
	}
	for (int i = 0; i < sourcePath.length; i++) {
		if (i < idx1 || i > idx2) {
			newPath.cities[i] = sourcePath.cities[i];
		}
		else {
			newPath.cities[i] = sourcePath.cities[idx2-(i-idx1)];
		}
	}
}

void SimulatedAnnealing::twoSwapGeometrical(Path path, InputData inputData, ConfigurationData config)
{
	// Symulowane wyzarzanie - 2-swap ze schladzaniem geometrycznym.
	std::mt19937_64 rng;
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	rng.seed(ss);
	std::uniform_real_distribution<double> unif(0, 1);
	std::ofstream outputFile;
	std::chrono::duration<float, std::milli> timer;
	int stopCriteriumInMS = config.stopAfterNSeconds * 1000;
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
	auto end = std::chrono::high_resolution_clock::now();
	double probability;
	float exponent, temperature, initialTemperature, bestPathTimestamp = 0.0;
	initialTemperature = initialTempTwoSwap(inputData);
	temperature = initialTemperature;
	std::cout << temperature << " : " << config.temperatureChangeParameter << std::endl;
	timer = end - begin;

	// size - rozmiar otoczenia obliczany w zaleznosci od metody generowania sasiedztwa
	int size = (path.length * (path.length - 1)) / 2;

	// Algorytm posiada kryterium zatrzymania zalezne od czasu wykonania oraz temperatury koncowej.
	while (timer.count() < stopCriteriumInMS && temperature > config.minimalTemperature) {
		for (int a = 0; a < size; a++) {
			// Wygeneruj nowa sciezke.
			twoSwap(currentPath, newPath);
			newCost = eval(newPath, inputData.costMatrix);
			probability = unif(rng);
			exponent = exp((currentCost-newCost) / temperature);

			// Jezeli koszt nowej sciezki jeset mniejszy od kosztu obecnej sciezki ALBO kryterium Metropolisa jest spelnione: ustaw nowa sciezke jako aktualna sciezke.
			if (newCost < currentCost || probability < exponent) {
				for (int i = 0; i < newPath.length; i++) currentPath.cities[i] = newPath.cities[i];
				currentCost = newCost;
				if (newCost < bestCost) {
					// Nowy najlepszy koszt znaleziony. Ustaw nowa sciezke jako najlepsza sciezke.
					bestCost = newCost;
					std::cout << "new best cost: " << bestCost << " temperature: " << temperature << std::endl;
					timer = end - begin;
					bestPathTimestamp = timer.count();
					outputFile.open(config.outputFilename, std::ios::app);
					if (outputFile.is_open()) {
						outputFile << bestCost << "," << bestPathTimestamp << std::endl;
					}
					outputFile.close();
					for (int j = 0; j < newPath.length; j++) bestPath.cities[j] = newPath.cities[j];
				}
			}
		}
		// Obniz temperature.
		temperature = temperature * config.temperatureChangeParameter;
		end = std::chrono::high_resolution_clock::now();
		timer = end - begin;
	}
	std::cout << "best cost found: " << bestCost << " temperature: " << temperature << std::endl;
	std::string resultPath = "";
	for (int i = 0; i < bestPath.length; i++) {
		std::cout << bestPath.cities[i] << "->";
		resultPath += std::to_string(bestPath.cities[i]) + "->";
	}
	std::cout << bestPath.cities[0];
	resultPath += std::to_string(bestPath.cities[0]);

	outputFile.open(config.outputFilename, std::ios::app);
	if (outputFile.is_open()){
		outputFile << bestCost << "," << config.temperatureChangeParameter << "," << initialTemperature << "," << temperature << "," << bestPathTimestamp << "," << resultPath << std::endl;
	}
	outputFile.close();
}

void SimulatedAnnealing::twoOptGeometrical(Path path, InputData inputData, ConfigurationData config)
{
	// Symulowane wyzarzanie - 2-opt ze schladzaniem geometrycznym.
	std::mt19937_64 rng;
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	rng.seed(ss);
	std::uniform_real_distribution<double> unif(0, 1);

	std::ofstream outputFile;

	std::chrono::duration<float, std::milli> timer;
	int stopCriteriumInMS = config.stopAfterNSeconds * 1000;
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
	auto end = std::chrono::high_resolution_clock::now();
	double probability;
	float exponent, temperature, initialTemperature, bestPathTimestamp = 0.0;
	initialTemperature = initialTempTwoOpt(inputData);
	temperature = initialTemperature;
	std::cout << temperature << " : " << config.temperatureChangeParameter << std::endl;
	timer = end - begin;

	// size - rozmiar otoczenia obliczany w zaleznosci od metody generowania sasiedztwa
	int size = ((path.length * (path.length - 1)) / 2) - path.length;

	// Algorytm posiada kryterium zatrzymania zalezne od czasu wykonania oraz temperatury koncowej.
	while (timer.count() < stopCriteriumInMS && temperature > config.minimalTemperature) {
		for (int a = 0; a < size; a++) {
			// Wygeneruj nowa sciezke.
			twoOpt(currentPath, newPath);
			newCost = eval(newPath, inputData.costMatrix);
			probability = unif(rng);
			exponent = exp((currentCost - newCost) / temperature);

			// Jezeli koszt nowej sciezki jeset mniejszy od kosztu obecnej sciezki ALBO kryterium Metropolisa jest spelnione: ustaw nowa sciezke jako aktualna sciezke.
			if (newCost < currentCost || probability < exponent) {
				for (int i = 0; i < newPath.length; i++) currentPath.cities[i] = newPath.cities[i];
				currentCost = newCost;

				if (newCost < bestCost) {
				// Nowy najlepszy koszt znaleziony. Ustaw nowa sciezke jako najlepsza sciezke.
					bestCost = newCost;
					std::cout << "new best cost: " << bestCost << " temperature: " << temperature << std::endl;
					timer = end - begin;
					bestPathTimestamp = timer.count();
					outputFile.open(config.outputFilename, std::ios::app);
					if (outputFile.is_open()) {
						outputFile << bestCost << "," << bestPathTimestamp << std::endl;
					}
					outputFile.close();
					for (int j = 0; j < newPath.length; j++) bestPath.cities[j] = newPath.cities[j];
				}
			}
		}
		// Obniz temperature.
		temperature = temperature * config.temperatureChangeParameter;
		end = std::chrono::high_resolution_clock::now();
		timer = end - begin;
	}
	std::cout << "best cost found: " << bestCost << " temperature: " << temperature << std::endl;
	std::string resultPath = "";
	for (int i = 0; i < bestPath.length; i++) {
		std::cout << bestPath.cities[i] << "->";
		resultPath += std::to_string(bestPath.cities[i]) + "->";
	}
	std::cout << bestPath.cities[0];
	resultPath += std::to_string(bestPath.cities[0]);

	outputFile.open(config.outputFilename, std::ios::app);
	if (outputFile.is_open()) {
		outputFile << bestCost << "," << config.temperatureChangeParameter << "," << initialTemperature << "," << temperature << "," << bestPathTimestamp << "," << resultPath << std::endl;
	}
	outputFile.close();
}

void SimulatedAnnealing::twoSwapLogarithmic(Path path, InputData inputData, ConfigurationData config)
{
	// Symulowane wyzarzanie - 2-swap ze schladzaniem logarytmicznym.
	std::mt19937_64 rng;
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	rng.seed(ss);
	std::uniform_real_distribution<double> unif(0, 1);

	std::ofstream outputFile;

	std::chrono::duration<float, std::milli> timer;
	int stopCriteriumInMS = config.stopAfterNSeconds * 1000;
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
	auto end = std::chrono::high_resolution_clock::now();
	double probability;
	float exponent, initialTemperature, bestPathTimestamp = 0.0;
	double temperature;
	initialTemperature = initialTempTwoSwap(inputData);
	std::cout << initialTemperature << std::endl;
	timer = end - begin;
	
	// size - rozmiar otoczenia obliczany w zaleznosci od metody generowania sasiedztwa
	int size = (path.length * (path.length - 1)) / 2;
	int k = 1;
	temperature = initialTemperature / log(1 + k);
	k += 1;
	while (timer.count() < stopCriteriumInMS && temperature > config.minimalTemperature) {
		for (int a = 0; a < size; a++) {
			// Wygeneruj nowa sciezke.
			twoSwap(currentPath, newPath);
			newCost = eval(newPath, inputData.costMatrix);
			probability = unif(rng);
 			exponent = exp((currentCost - newCost) / temperature);

			// Jezeli koszt nowej sciezki jeset mniejszy od kosztu obecnej sciezki ALBO kryterium Metropolisa jest spelnione: ustaw nowa sciezke jako aktualna sciezke.
			if (newCost < currentCost || probability < exponent) {
				for (int i = 0; i < newPath.length; i++) currentPath.cities[i] = newPath.cities[i];
				currentCost = newCost;
				if (newCost < bestCost) {
					// Nowy najlepszy koszt znaleziony. Ustaw nowa sciezke jako najlepsza sciezke.
					bestCost = newCost;
					std::cout << "new best cost: " << bestCost << " temperature: " << temperature << std::endl;
					timer = end - begin;
					bestPathTimestamp = timer.count();
					outputFile.open(config.outputFilename, std::ios::app);
					if (outputFile.is_open()) {
						outputFile << bestCost << "," << bestPathTimestamp << std::endl;
					}
					outputFile.close();
					for (int j = 0; j < newPath.length; j++) bestPath.cities[j] = newPath.cities[j];
				}
			}
		}
		// Obniz temperature.
		temperature = initialTemperature / log(1 + k);
		k += 1;
		end = std::chrono::high_resolution_clock::now();
		timer = end - begin;
	}
	std::cout << "best cost found: " << bestCost << " temperature: " << temperature << std::endl;
	std::string resultPath = "";
	for (int i = 0; i < bestPath.length; i++) {
		std::cout << bestPath.cities[i] << "->";
		resultPath += std::to_string(bestPath.cities[i]) + "->";
	}
	std::cout << bestPath.cities[0];
	resultPath += std::to_string(bestPath.cities[0]);

	outputFile.open(config.outputFilename, std::ios::app);
	if (outputFile.is_open()) {
		outputFile << bestCost << "," << config.temperatureChangeParameter << "," << initialTemperature << "," << temperature << "," << bestPathTimestamp << "," << resultPath << std::endl;
	}
	outputFile.close();
}

void SimulatedAnnealing::twoOptLogarithmic(Path path, InputData inputData, ConfigurationData config)
{
	// Symulowane wyzarzanie - 2-opt ze schladzaniem logarytmicznym.
	std::mt19937_64 rng;
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	rng.seed(ss);
	std::uniform_real_distribution<double> unif(0, 1);

	std::ofstream outputFile;

	std::chrono::duration<float, std::milli> timer;
	int stopCriteriumInMS = config.stopAfterNSeconds * 1000;
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
	auto end = std::chrono::high_resolution_clock::now();
	double probability;
	float exponent, temperature, initialTemperature, bestPathTimestamp = 0.0;
	initialTemperature = initialTempTwoOpt(inputData);
	std::cout << initialTemperature << std::endl;
	timer = end - begin;

	// size - rozmiar otoczenia obliczany w zaleznosci od metody generowania sasiedztwa
	int size = ((path.length * (path.length - 1)) / 2) - path.length;
	int k = 1;
	temperature = initialTemperature / log(1 + k);
	k++;
	while (timer.count() < stopCriteriumInMS && temperature > config.minimalTemperature) {
		for (int a = 0; a < size; a++) {
			// Wygeneruj nowa sciezke.
			twoOpt(currentPath, newPath);
			newCost = eval(newPath, inputData.costMatrix);
			probability = unif(rng);
			exponent = exp((currentCost - newCost) / temperature);

			// Jezeli koszt nowej sciezki jeset mniejszy od kosztu obecnej sciezki ALBO kryterium Metropolisa jest spelnione: ustaw nowa sciezke jako aktualna sciezke.
			if (newCost < currentCost || probability < exponent) {
				for (int i = 0; i < newPath.length; i++) currentPath.cities[i] = newPath.cities[i];
				currentCost = newCost;
				if (newCost < bestCost) {
					// Nowy najlepszy koszt znaleziony. Ustaw nowa sciezke jako najlepsza sciezke.
					bestCost = newCost;
					std::cout << "new best cost: " << bestCost << " temperature: " << temperature << std::endl;
					timer = end - begin;
					bestPathTimestamp = timer.count();
					outputFile.open(config.outputFilename, std::ios::app);
					if (outputFile.is_open()) {
						outputFile << bestCost << "," << bestPathTimestamp << std::endl;
					}
					outputFile.close();
					for (int j = 0; j < newPath.length; j++) bestPath.cities[j] = newPath.cities[j];
				}
			}
		}
		// Obniz temperature.
		temperature = initialTemperature / log(1 + k);
		k++;
		end = std::chrono::high_resolution_clock::now();
		timer = end - begin;
	}
	std::cout << "best cost found: " << bestCost << " temperature: " << temperature << std::endl;
	std::string resultPath = "";
	for (int i = 0; i < bestPath.length; i++) {
		std::cout << bestPath.cities[i] << "->";
		resultPath += std::to_string(bestPath.cities[i]) + "->";
	}
	std::cout << bestPath.cities[0];
	resultPath += std::to_string(bestPath.cities[0]);

	// save result to file
	outputFile.open(config.outputFilename, std::ios::app);
	if (outputFile.is_open()) {
		outputFile << bestCost << "," << config.temperatureChangeParameter << "," << initialTemperature << "," << temperature << "," << bestPathTimestamp << "," << resultPath << std::endl;
	}
	outputFile.close();
}