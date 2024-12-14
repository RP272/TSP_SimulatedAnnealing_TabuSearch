#ifndef NEARESTNEIGHBOR_H
#define NEARESTNEIGHBOR_H
#include "../DataManagement/InputData.h"
#include "../Path/Path.h"

class NearestNeighbor
{
public:
	static Path solveTSP(InputData data);
};

#endif
