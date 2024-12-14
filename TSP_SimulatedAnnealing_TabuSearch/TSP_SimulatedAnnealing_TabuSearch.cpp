// TSP_SimulatedAnnealing_TabuSearch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "DataManagement/FileReader.hpp"

int main()
{
    std::cout << "Hello World!\n";
    ConfigurationData config = FileReader::readConfigurationDataFile();
    InputData result = FileReader::readInputDataFile(config);
}