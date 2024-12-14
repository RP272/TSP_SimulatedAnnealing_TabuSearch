#ifndef CONFIGURATIONDATA_H
#define CONFIGURATIONDATA_H
#include "../DataManagement/InputData.h"

class SimulatedAnnealing
{
public:
	static void solveTSP(InputData inputData, int stopAfterNSeconds, float temperatureChangeParameter);
};

#endif