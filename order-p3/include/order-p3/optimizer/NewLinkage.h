#pragma once
#include <random>
#include <vector>
#include "solution/Solution.h"
using std::vector;

class NewLinkage {
public:
	NewLinkage(int problemSize, std::mt19937& randomGenerator);
	void update(Solution* newSolution, int currentPopulationSize);
	void update(std::vector<Solution*>& population);
	const vector<vector<int>>& getClusters() const;
private:
	void updateLinkageInformation(Solution* solution);
	void recalculateDistances(const int currentPopulationSize);
	double calculateDependencyBetweenGenes(const int firstGeneIndex, const int secondGeneIndex, const int currentPopulationSize);
	double calculateRelativeOrderingMeasure(const int firstGeneIndex, const int secondGeneIndex, const int currentPopulationSize);
	double calculateAdjacencyMeasure(const int firstGeneIndex, const int secondGeneIndex, const int currentPopulationSize);
	void rebuildTree();
	double getDistanceBetweenClusters(vector<int>& firstCluster, vector<int>& secondCluster);
	double getDistanceBetweenClusters(vector<int>& first, vector<int>& second, int firstStart, int firstEnd, int secondStart, int secondEnd);
	void clearClusters();
	void updateRelativeOrderingInformation(vector<double>* genotype);
	void updateAdjacencyInformation(vector<double>* genotype);
	static double calculateRelativeOrderingInformation(double firstGeneValue, double secondGeneValue);
	static double calculateAdjacencyInformation(double firstGeneValue, double secondGeneValue);
	int problemSize;
	std::mt19937& randomGenerator;
	vector<vector<double>> relativeOrderingInformationSum;
	vector<vector<double>> adjacencyInformationSum;
	vector<vector<double>> distanceMeasureMatrix;
	vector<vector<int>> clusters;
};
