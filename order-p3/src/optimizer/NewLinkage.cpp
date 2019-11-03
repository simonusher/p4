#include "../../include/order-p3/optimizer/NewLinkage.h"
#include <iostream>

NewLinkage::NewLinkage(int problemSize, std::mt19937& randomGenerator) :
	problemSize(problemSize),
	randomGenerator(randomGenerator),
	relativeOrderingInformationSum(problemSize, vector<double>(problemSize, 0.0)),
	adjacencyInformationSum(problemSize, vector<double>(problemSize, 0.0)),
	distanceMeasureMatrix(problemSize, vector<double>(problemSize, 0.0)),
	bleDistribution(0, 1)
{	
}

void NewLinkage::update(Solution* newSolution, int currentPopulationSize) {
	updateLinkageInformation(newSolution);
	if(currentPopulationSize > 1) {
		recalculateDistances(currentPopulationSize);
		rebuildTree();
	}
}

const vector<vector<int>>& NewLinkage::getClusters() const {
	return this->clusters;
}

void NewLinkage::update(std::vector<Solution*>& population) {

	if(population.size() > 1) {
		for (int i = 0; i < problemSize; i++)
		{
			for (int j = i + 1; j < problemSize; j++)
			{
				/* Compute entropy of probability that variable i has a smaller value than variable j, use inverted entropy */
				double p = 0.0;
				for (Solution* solution : population)
				{
					std::vector<double>* genotypePtr = solution->getGenotypePtr();
					if(genotypePtr->at(i) < genotypePtr->at(j)) {
						p += 1.0;
					}
				}
				p /= static_cast<double>(population.size());
				double entropy = (p == 0) || (p == 1) ? 0 : -(p * log2(p) + (1.0 - p) * log2(1.0 - p));

				distanceMeasureMatrix.at(i).at(j) = 1.0 - entropy;

				/* Multiply by inverted average distance between variables */
				double average_distance = 0.0;
				for (Solution * solution : population)
				{
					std::vector<double>* genotypePtr = solution->getGenotypePtr();
					double diff = genotypePtr->at(i) - genotypePtr->at(j);
					average_distance += (diff * diff);
				}
				average_distance /= static_cast<double>(population.size());
				distanceMeasureMatrix.at(i).at(j) *= 1.0 - average_distance;

				/* Create symmetric matrix */
				distanceMeasureMatrix.at(j).at(i) *= distanceMeasureMatrix.at(i).at(j);
			}
		}
		rebuildTree();
	}
}

void NewLinkage::updateLinkageInformation(Solution* solution) {
	vector<double>* genotype = solution->getGenotypePtr();
	updateRelativeOrderingInformation(genotype);
	updateAdjacencyInformation(genotype);
}

void NewLinkage::recalculateDistances(const int currentPopulationSize) {
	for(int firstGeneIndex = 0; firstGeneIndex < problemSize - 1; firstGeneIndex++) {
		for(int secondGeneIndex = firstGeneIndex + 1; secondGeneIndex < problemSize; secondGeneIndex++) {
			const double dependencyBetweenGenes = calculateDependencyBetweenGenes(firstGeneIndex, secondGeneIndex, currentPopulationSize);
			distanceMeasureMatrix[firstGeneIndex][secondGeneIndex] = dependencyBetweenGenes;
			distanceMeasureMatrix[secondGeneIndex][firstGeneIndex] = dependencyBetweenGenes;
		}
	}
}

double NewLinkage::calculateDependencyBetweenGenes(const int firstGeneIndex, const int secondGeneIndex, const int currentPopulationSize) {
	const double relativeOrderingMeasure = calculateRelativeOrderingMeasure(firstGeneIndex, secondGeneIndex, currentPopulationSize);
	const double adjacencyMeasure = calculateAdjacencyMeasure(firstGeneIndex, secondGeneIndex, currentPopulationSize);
	return relativeOrderingMeasure * adjacencyMeasure;
}

double NewLinkage::calculateRelativeOrderingMeasure(const int firstGeneIndex, const int secondGeneIndex, const int currentPopulationSize) {
	const double pij = relativeOrderingInformationSum[firstGeneIndex][secondGeneIndex] / double(currentPopulationSize);
	double entropy;
	if(pij == 0 || pij == 1) {
		entropy = 0;
	} else {
		entropy = -(pij * log2(pij) + (1.0 - pij) * log2(1.0 - pij));
	}
	return 1.0 - entropy;
}

double NewLinkage::calculateAdjacencyMeasure(const int firstGeneIndex, const int secondGeneIndex, const int currentPopulationSize) {
	const double adjacencyMeasure = adjacencyInformationSum[firstGeneIndex][secondGeneIndex] / double(currentPopulationSize);
	return 1.0 - adjacencyMeasure;
}


void NewLinkage::rebuildTree() {
	clearClusters();
	vector<vector<int>> useful;
	useful.reserve(2 * problemSize);
	vector<vector<int>> unmerged(problemSize, vector<int>(1));
	unmerged.reserve(2 * problemSize);
	vector<int> options(problemSize);
	std::iota(options.begin(), options.end(), 0);

	for (int i = 0; i < problemSize; i++) {
		unmerged[i][0] = i;
	}
	int startIndex = 0;

	double greatestDependency;
	double newDistance;
	int bestFirstIndex;
	int bestSecondIndex;
	while (options.size() - startIndex > 1) {
		greatestDependency = -1;
		std::shuffle(options.begin() + startIndex, options.end(), randomGenerator);
		int firstOption;
		int secondOption;
		for (int i = startIndex; i < options.size(); i++) {
			firstOption = options[i];
			for (int j = i + 1; j < options.size(); j++) {
				secondOption = options[j];
				newDistance = getDistanceBetweenClusters(unmerged[firstOption], unmerged[secondOption]);
				if (newDistance >= greatestDependency) {
					greatestDependency = newDistance;
					bestFirstIndex = i;
					bestSecondIndex = j;
				}
			}
		}
		// std::cout << greatestDependency << std::endl;
		vector<int>& first = unmerged.at(options[bestFirstIndex]);
		vector<int>& second = unmerged.at(options[bestSecondIndex]);
		vector<int> mergedClusters;
		mergedClusters.reserve(first.size() + second.size());
		mergedClusters.insert(mergedClusters.end(), first.begin(), first.end());
		mergedClusters.insert(mergedClusters.end(), second.begin(), second.end());
		if (greatestDependency != 0) {
			useful.push_back(first);
			useful.push_back(second);
		}
		std::swap(options[startIndex], options[bestFirstIndex]);
		startIndex++;
		std::swap(options[startIndex], options[bestSecondIndex]);
		startIndex++;
		options.push_back(unmerged.size());
		unmerged.push_back(mergedClusters);
	}
	// std::sort(useful.begin(), useful.end(), [](const auto& firstCluster, const auto& secondCluster) { return firstCluster.size() < secondCluster.size(); });
	// std::shuffle(useful.begin(), useful.end(), randomGenerator);
	clusters = useful;
}

double NewLinkage::getDistanceBetweenClusters(vector<int>& firstCluster, vector<int>& secondCluster) {
	return getDistanceBetweenClusters(firstCluster, secondCluster, 0, firstCluster.size(), 0, secondCluster.size());
}

double NewLinkage::getDistanceBetweenClusters(vector<int>& first, vector<int>& second,
	int firstStart, int firstEnd, int secondStart, int secondEnd) {
	int firstSize = firstEnd - firstStart;
	int secondSize = secondEnd - secondStart;
	if (secondSize > 1) {
		return 1.0 / secondSize * getDistanceBetweenClusters(first, second,
			firstStart, firstEnd, secondStart, secondStart + 1) +                        // 1.0 / secondSize is equal to 1/(1 + secondSize - 1) which in turn is equal to
			(double)(secondSize - 1) / secondSize * getDistanceBetweenClusters(first, second, firstStart, firstEnd, secondStart + 1, secondEnd);   // first.size()/(firstSubvector.size() + secondSubvector.size())
	}
	else {
		if (firstSize == 1) {
			return distanceMeasureMatrix[first.at(0)][second.at(0)];
		}
		else {
			return getDistanceBetweenClusters(second, first, secondStart, secondEnd, firstStart, firstEnd);
		}
	}
}


void NewLinkage::clearClusters() {
	this->clusters.clear();
}

void NewLinkage::updateRelativeOrderingInformation(vector<double>* genotype) {
	for(int firstGeneIndex = 0; firstGeneIndex < problemSize - 1; firstGeneIndex++) {
		const double firstGeneValue = genotype->at(firstGeneIndex);
		for(int secondGeneIndex = firstGeneIndex + 1; secondGeneIndex < problemSize; secondGeneIndex++) {
			const double secondGeneValue = genotype->at(secondGeneIndex);
			relativeOrderingInformationSum[firstGeneIndex][secondGeneIndex] += calculateRelativeOrderingInformation(firstGeneValue, secondGeneValue);
		}
	}
}

void NewLinkage::updateAdjacencyInformation(vector<double>* genotype) {
	for (int firstGeneIndex = 0; firstGeneIndex < problemSize - 1; firstGeneIndex++) {
		const double firstGeneValue = genotype->at(firstGeneIndex);
		for (int secondGeneIndex = firstGeneIndex + 1; secondGeneIndex < problemSize; secondGeneIndex++) {
			const double secondGeneValue = genotype->at(secondGeneIndex);
			adjacencyInformationSum[firstGeneIndex][secondGeneIndex] += calculateAdjacencyInformation(firstGeneValue, secondGeneValue);
		}
	}
}

double NewLinkage::calculateRelativeOrderingInformation(const double firstGeneValue, const double secondGeneValue) {
	if(firstGeneValue < secondGeneValue) {
		return 1.0;
	} else {
		return 0.0;
	}
}

double NewLinkage::calculateAdjacencyInformation(const double firstGeneValue, const double secondGeneValue) {
	const double valuesDifference = firstGeneValue - secondGeneValue;
	return valuesDifference * valuesDifference;
}
