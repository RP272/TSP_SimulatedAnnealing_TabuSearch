#ifndef CONFIGURATIONDATA_HPP
#define CONFIGURATIONDATA_HPP
#include <string>

struct ConfigurationData
{
	std::string filename;
	int stopAfterNSeconds; 
	float temperatureChangeParameter;
};

#endif
