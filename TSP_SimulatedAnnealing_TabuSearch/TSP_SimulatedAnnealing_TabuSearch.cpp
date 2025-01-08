#include <iostream>
#include "DataManagement/FileReader.h"
#include "NearestNeighbor/NearestNeighbor.h"
#include "SimulatedAnnealing/SimulatedAnnealing.h"
#include <sstream>
#include <algorithm>
int main()
{
    // Wczytaj dane z pliku konfiguracyjnego.
    ConfigurationData config = FileReader::readConfigurationDataFile();
    
    // Wczytaj dane wejsciowe na podstawie sciezki podanej w pliku konfiguracyjnym.
    InputData inputData = FileReader::readInputDataFile(config);
    
    // Na podstawie danych w pliku konfiguracyjnym, uruchom odpowiednia wersje algorytmu symulowanego wyzarzania.
    switch (config.neighborGeneration) {
        case 0: {
            if (config.coolingScheme == 0) {
                // 2-swap, schladzanie geometryczne
                for (int a = 0; a < config.numberOfMeasurements; a++) {
                    Path startingPath = NearestNeighbor::solveTSP(inputData);
                    SimulatedAnnealing::twoSwapGeometrical(startingPath, inputData, config);
                }
            }
            else if (config.coolingScheme == 1) {
                // 2-swap, schladzanie logarytmiczne
                for (int a = 0; a < config.numberOfMeasurements; a++) {
                    Path startingPath = NearestNeighbor::solveTSP(inputData);
                    SimulatedAnnealing::twoSwapLogarithmic(startingPath, inputData, config);
                }
            }
            break;
        }
        case 1: {
            if (config.coolingScheme == 0) {
                // 2-opt, schladzanie geometryczne
                for (int a = 0; a < config.numberOfMeasurements; a++) {
                    Path startingPath = NearestNeighbor::solveTSP(inputData);
                    SimulatedAnnealing::twoOptGeometrical(startingPath, inputData, config);
                }
            }
            else if (config.coolingScheme == 1) {
                // 2-opt, schladzanie logarytmiczne
                for (int a = 0; a < config.numberOfMeasurements; a++) {
                    Path startingPath = NearestNeighbor::solveTSP(inputData);
                    SimulatedAnnealing::twoOptLogarithmic(startingPath, inputData, config);
                }
            }
            break;
        }
    }
}