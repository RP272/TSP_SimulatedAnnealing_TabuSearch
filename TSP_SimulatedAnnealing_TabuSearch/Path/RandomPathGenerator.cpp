#include "RandomPathGenerator.h"
#include "../DataManagement/RandomIntegerGenerator.h"

void RandomPathGenerator::swap(int* a, int* b)
{
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

Path RandomPathGenerator::generate(int numberOfCities) 
{
	Path p;
	p.length = numberOfCities;
	p.cities = new int[numberOfCities];
	RandomIntegerGenerator rng(0, numberOfCities - 1);
	for (int i = 0; i < numberOfCities; i++) p.cities[i] = i;
	int visitedVertices = 0;
	int currentVertexIndex = rng.generate();
	swap(&p.cities[currentVertexIndex], &p.cities[0]);
	visitedVertices++;
	while (visitedVertices < numberOfCities) {
		RandomIntegerGenerator rng(visitedVertices, numberOfCities - 1);
		currentVertexIndex = rng.generate();
		swap(&p.cities[currentVertexIndex], &p.cities[visitedVertices]);
		visitedVertices++;
	}
	return p;
}
