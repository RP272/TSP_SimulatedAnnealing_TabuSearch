#ifndef CONFIGURATIONDATA_H
#define CONFIGURATIONDATA_H
#include <string>

struct ConfigurationData
{
	std::string filename;
	int stopAfterNSeconds; 
	float temperatureChangeParameter;
	std::string outputFilename;
	int neighborGeneration;
	int coolingScheme;
	int numberOfMeasurements;
	float minimalTemperature;
};

#endif
