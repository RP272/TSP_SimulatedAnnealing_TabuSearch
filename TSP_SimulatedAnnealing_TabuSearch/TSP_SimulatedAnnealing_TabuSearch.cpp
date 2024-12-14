#include <iostream>
#include "DataManagement/FileReader.h"
#include "NearestNeighbor/NearestNeighbor.h"
#include "SimulatedAnnealing/SimulatedAnnealing.h"

int main()
{
    std::cout << "Hello World!\n";
    ConfigurationData config = FileReader::readConfigurationDataFile();
    InputData inputData = FileReader::readInputDataFile(config);
    Path startingPath = NearestNeighbor::solveTSP(inputData);
    SimulatedAnnealing::solveTSP(startingPath, inputData, config.stopAfterNSeconds, config.temperatureChangeParameter);
}