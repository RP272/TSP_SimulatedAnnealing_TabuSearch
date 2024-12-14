#include "FileReader.hpp"
#include <sstream>
#include <fstream>

ConfigurationData FileReader::readConfigurationDataFile()
{
    ConfigurationData result;
    std::ifstream configurationFile("configuration_file.txt");
    if (configurationFile.good()) {
        std::string line;
        std::getline(configurationFile, line);
        result.filename = line.substr(line.find(':') + 1);
        std::getline(configurationFile, line);
        result.stopAfterNSeconds = std::stoi(line.substr(line.find(':') + 1));
        std::getline(configurationFile, line);
        result.temperatureChangeParameter = std::stof(line.substr(line.find(':') + 1));
    }
    configurationFile.close();
    return result;
}

InputData FileReader::readInputDataFile(ConfigurationData config)
{
    InputData result = { 0 };
    std::ifstream inputFile(config.filename);
    if (inputFile.good()) {
        std::string line;
        // pobierz atrybut NAME
        std::getline(inputFile, line);
        // pobierz atrybut TYPE
        std::getline(inputFile, line);
        // pobierz atrybut COMMENT
        std::getline(inputFile, line);
        // pobierz atrybut DIMENSION
        std::getline(inputFile, line);
        result.numberOfCities = std::stoi(line.substr(line.find(':') + 1));
        // pobierz atrybut EDGE_WEIGHT_TYPE
        std::getline(inputFile, line);
        // pobierz atrybut EDGE_WEIGHT_FORMAT
        std::getline(inputFile, line);
        // pobierz atrybut EDGE_WEIGHT_SECTION
        std::getline(inputFile, line);

        // pobierz wartości wag grafu
        int rowCounter = 0, columnCounter, tmp;
        result.costMatrix = new int* [result.numberOfCities];
        while (rowCounter < result.numberOfCities) {
            result.costMatrix[rowCounter] = new int[result.numberOfCities];
            columnCounter = 0;

            while (columnCounter < result.numberOfCities) {
                std::getline(inputFile, line);
                std::istringstream row(line);
                while (row >> tmp) {
                    result.costMatrix[rowCounter][columnCounter] = tmp;
                    columnCounter++;
                }
            }
            rowCounter++;
        }
    }
    inputFile.close();
    return result;
};