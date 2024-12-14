#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H
#include "../DataManagement/InputData.h"
#include "../Path/Path.h"

class SimulatedAnnealing
{
public:
	static int eval(Path path, int** costMatrix);
	static void solveTSP(Path path, InputData inputData, int stopAfterNSeconds, float temperatureChangeParameter);
	static void generateNewPath(Path sourcePath, Path newPath);
};

#endif