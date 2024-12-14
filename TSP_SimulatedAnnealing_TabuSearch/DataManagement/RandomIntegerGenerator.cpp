#include "RandomIntegerGenerator.h"

RandomIntegerGenerator::RandomIntegerGenerator(int min, int max) : rng(rd()), uni(min, max) {};

int RandomIntegerGenerator::generate()
{
	return uni(rng);
}