#ifndef CONFIGURATIONDATA_H
#define CONFIGURATIONDATA_H
#include <string>

struct ConfigurationData
{
	std::string filename;
	int stopAfterNSeconds; 
	float temperatureChangeParameter;
};

#endif
