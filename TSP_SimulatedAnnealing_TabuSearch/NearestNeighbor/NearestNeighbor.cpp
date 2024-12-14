#include "NearestNeighbor.h"
#include <climits>

Path NearestNeighbor::solveTSP(InputData data)
{
	bool* visited = new bool[data.numberOfCities];
	Path bestPath, currentPath;
	bestPath.cities = new int[data.numberOfCities];
	bestPath.length = data.numberOfCities;
	int startingIndex, currentVertexIndex, visitedVertices, shortestEdge, shortestEdgeVertex, cost, minimumCost;
	minimumCost = INT_MAX;
	for (int a = 0; a < data.numberOfCities; a++) {
		currentPath.cities = new int[data.numberOfCities];
		currentPath.length = data.numberOfCities;
		for (int i = 0; i < data.numberOfCities; i++) visited[i] = false;
		visitedVertices = 0;
		cost = 0;
		startingIndex = a;
		currentVertexIndex = startingIndex;
		visited[currentVertexIndex] = true;
		currentPath.cities[visitedVertices] = currentVertexIndex;
		visitedVertices++;

		while (visitedVertices < data.numberOfCities) {
			shortestEdge = INT_MAX;
			for (int j = 0; j < data.numberOfCities; j++) {
				if (j != currentVertexIndex && visited[j] == false && data.costMatrix[currentVertexIndex][j] < shortestEdge) {
					shortestEdge = data.costMatrix[currentVertexIndex][j];
					shortestEdgeVertex = j;
				}
			}
			cost += shortestEdge;
			visited[shortestEdgeVertex] = true;
			currentPath.cities[visitedVertices] = shortestEdgeVertex;
			currentVertexIndex = shortestEdgeVertex;
			visitedVertices++;
		}
		cost += data.costMatrix[currentVertexIndex][startingIndex];
		if (cost < minimumCost) {
			minimumCost = cost;
			for (int i = 0; i < currentPath.length; i++) {
				bestPath.cities[i] = currentPath.cities[i];
			}
		};
		delete[] currentPath.cities;
	}
	delete[] visited;
	return bestPath;
}

