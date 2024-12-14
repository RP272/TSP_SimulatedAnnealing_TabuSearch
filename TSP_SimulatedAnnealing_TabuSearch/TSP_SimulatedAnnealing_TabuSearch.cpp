#include <iostream>
#include "DataManagement/FileReader.h"
#include "NearestNeighbor/NearestNeighbor.h"

int main()
{
    std::cout << "Hello World!\n";
    ConfigurationData config = FileReader::readConfigurationDataFile();
    InputData inputData = FileReader::readInputDataFile(config);
    Path startingPath = NearestNeighbor::solveTSP(inputData);
    for (int i = 0; i < startingPath.length; i++) {
        std::cout << startingPath.cities[i] << "->";
    }
    std::cout << startingPath.cities[0];
}