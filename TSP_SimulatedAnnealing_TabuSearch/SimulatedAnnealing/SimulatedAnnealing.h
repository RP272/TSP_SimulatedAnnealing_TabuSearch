#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H
#include "../DataManagement/InputData.h"
#include "../DataManagement/ConfigurationData.h"
#include "../Path/Path.h"

class SimulatedAnnealing
{
public:
	static int eval(Path path, int** costMatrix);
	static float initialTempTwoSwap(InputData inputData);
	static float initialTempTwoOpt(InputData inputData);
	static void twoSwap(Path sourcePath, Path newPath);
	static void twoOpt(Path sourcePath, Path newPath);
	static void twoSwapGeometrical(Path path, InputData inputData, ConfigurationData config);
	static void twoSwapLogarithmic(Path path, InputData inputData, ConfigurationData config);
	static void twoOptGeometrical(Path path, InputData inputData, ConfigurationData config);
	static void twoOptLogarithmic(Path path, InputData inputData, ConfigurationData config);
};

#endif