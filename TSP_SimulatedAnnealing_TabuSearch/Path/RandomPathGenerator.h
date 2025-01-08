#ifndef RANDOMPATHGENERATOR_H
#define RANDOMPATHGENERATOR_H

#include "Path.h"

class RandomPathGenerator
{
public:
	static void swap(int* a, int* b);
	static Path generate(int numberOfCities);
};

#endif

