//
// Created by Szymon on 22.11.2018.
//

#include "../../include/order-p3/optimizer/Linkage.h"



Linkage::Linkage(int numberOfGenes, std::mt19937& randomGenerator) : randomGenerator(randomGenerator) {
    this->numberOfGenes = numberOfGenes;
    initializeDistanceMatrix();
	initializeDistanceMeasures();
	createCachedDistances();
}

Linkage::~Linkage() {
	deleteMatrix();
}

void Linkage::deleteMatrix() {
	for (int i = 0; i < numberOfGenes; i++) {
		delete (*distanceMeasureMatrix)[i];
	}
	delete distanceMeasureMatrix;
}

void Linkage::initializeDistanceMatrix(){
    distanceMeasureMatrix = new vector<vector<double>*>(numberOfGenes);
    for(int i = 0; i < numberOfGenes; i++){
        (*distanceMeasureMatrix)[i] = new vector<double>(numberOfGenes);
    }
}

void Linkage::initializeDistanceMeasures() {
	for (int i = 0; i < numberOfGenes - 1; i++) {
		for (int j = i + 1; j < numberOfGenes; j++) {
			relativeOrderingInformation[i][j] = 0;
			adjacencyInformation[i][j] = 0;
		}
	}
}


void Linkage::recalculate(int currentPopulationSize, Solution* newSolution) {
	updatePopulationInformation(newSolution);
	if(currentPopulationSize > 1) {
		recalculateDistances(currentPopulationSize);
		buildTree();
	}
}

void Linkage::recalculateDistances(int currentPopulationSize) {
	for(int i = 0; i < numberOfGenes - 1; i++) {
		for(int j = i + 1; j < numberOfGenes; j++) {
			distanceMeasureMatrix->at(i)->at(j) = calculateDistanceBetweenGenes(i, j, currentPopulationSize);
		}
	}
}

void Linkage::print() {
    printDistanceMatrix();
    printClusters();
}

void Linkage::createCachedDistances(){
	cachedDistances = vector<vector<double>>(2 * numberOfGenes, vector<double>(2 * numberOfGenes));
}

void Linkage::resetCachedDistances() {
	for(vector<double>& vec : cachedDistances) {
		std::fill(vec.begin(), vec.end(), -1);
	}
}

void Linkage::printDistanceMatrix() {
    vector<double> *row;
    std::cout << "Distance matrix: " << std::endl;
    for(int i = 0; i < numberOfGenes; i++){
        row = distanceMeasureMatrix->at(i);
        for(int j = 0; j < numberOfGenes; j++){
            std::cout << "    " << row->at(j) << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Linkage::printClusters() {
    std::cout << "Clusters: " << std::endl;
    for(int i = 0; i < clusters.size(); i++){
        for(int j = 0; j < clusters.at(i).size(); j++){
            std::cout << clusters.at(i).at(j) << " ";
        }
        std::cout << std::endl;
    }
}

void Linkage::clearClusters() {
    clusters.clear();
}

void Linkage::buildTree() {
	resetCachedDistances();
    clearClusters();
	vector<vector<int>> useful;
	useful.reserve(2 * numberOfGenes);
	vector<vector<int>> unmerged(numberOfGenes, vector<int>(1));
	unmerged.reserve(2 * numberOfGenes);
	vector<int> options(numberOfGenes);
	std::iota(options.begin(), options.end(), 0);

    for(int i = 0; i < numberOfGenes; i++){
		unmerged[i][0] = i;
    }
	int startIndex = 0;

    double smallestDistance;
    double newDistance;
    int bestFirstIndex;
    int bestSecondIndex;
	double cachedDistance;
    while(options.size() - startIndex > 1){
        smallestDistance = 1;
		std::shuffle(options.begin() + startIndex, options.end(), randomGenerator);
		int firstOption;
		int secondOption;
		for (int i = startIndex; i < options.size(); i++) {
			firstOption = options[i];
			for (int j = i + 1; j < options.size(); j++) {
				secondOption = options[j];
				cachedDistance = cachedDistances[firstOption][secondOption];
				if(cachedDistance == -1)
				{
					newDistance = getDistanceBetweenClusters(unmerged[firstOption], unmerged[secondOption]);
					cachedDistances[firstOption][secondOption] = newDistance;
				} else
				{
					newDistance = cachedDistance;
				}
				if (newDistance <= smallestDistance) {
					smallestDistance = newDistance;
					bestFirstIndex = i;
					bestSecondIndex = j;
				}
			}
		}
		vector<int> &first = unmerged.at(options[bestFirstIndex]);
		vector<int> &second = unmerged.at(options[bestSecondIndex]);
        vector<int> mergedClusters;
		mergedClusters.reserve(first.size() + second.size());
        mergedClusters.insert( mergedClusters.end(), first.begin(), first.end() );
        mergedClusters.insert( mergedClusters.end(), second.begin(), second.end() );
        if(smallestDistance != 0){
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
	std::sort(useful.begin(), useful.end(), [](const auto& firstCluster, const auto& secondCluster) { return firstCluster.size() < secondCluster.size(); });
	clusters = useful;
}

const vector<vector<int>>& Linkage::getClusters() const {
	return clusters;
}

double Linkage::getDistanceBetweenClusters(vector<int>& first, vector<int>& second) {
	return getDistanceBetweenClusters(first, second, 0, first.size(), 0, second.size());
}

double Linkage::getDistanceBetweenClusters(vector<int> &first, vector<int> &second,
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
			return getDistanceMeasure(first.at(0), second.at(0));
		}
		else {
			return getDistanceBetweenClusters(second, first, secondStart, secondEnd, firstStart, firstEnd);
		}
	}
}

double Linkage::getDistanceMeasure(int firstGeneIndex, int secondGeneIndex) {
	if(firstGeneIndex > secondGeneIndex) {
		return getDistanceMeasure(secondGeneIndex, firstGeneIndex);
	}
	return distanceMeasureMatrix->at(firstGeneIndex)->at(secondGeneIndex);
}

double Linkage::calculateDistanceBetweenGenes(int firstGeneIndex, int secondGeneIndex, int currentPopulationSize) {
	double relativeOrderingMeasure = calculateRelativeOrderingInformation(firstGeneIndex, secondGeneIndex, currentPopulationSize);
	double adjacencyMeasure = calculateAdjacencyInformation(firstGeneIndex, secondGeneIndex, currentPopulationSize);
	return relativeOrderingMeasure * adjacencyMeasure;
}

double Linkage::calculateRelativeOrderingInformation(int firstGeneIndex, int secondGeneIndex, int currentPopulationSize) {
	double pij =  1 / (double(currentPopulationSize)) * relativeOrderingInformation[firstGeneIndex][secondGeneIndex];
	double firstTerm = 0;
	double secondTerm = 0;
	if(pij != 0) {
		firstTerm = pij * log2(pij);
	}
	if(1 - pij != 0) {
		secondTerm = (1 - pij) * log2(1 - pij);
	}
	return 1 - (-firstTerm + secondTerm);
}

double Linkage::calculateAdjacencyInformation(int firstGeneIndex, int secondGeneIndex, int currentPopulationSize) {
	return 1 - (1 / double(currentPopulationSize)) * adjacencyInformation[firstGeneIndex][secondGeneIndex];
}

void Linkage::updatePopulationInformation(Solution* newSolution) {
	updateRelativeOrderingInformation(newSolution);
	updateAdjacencyInformation(newSolution);
}

void Linkage::updateRelativeOrderingInformation(Solution* newSolution) {
	updateRelativeOrderingInformation(newSolution->getGenotypePtr());
}

void Linkage::updateRelativeOrderingInformation(vector<double>* genotype) {
	for (size_t i = 0; i < genotype->size() - 1; i++) {
		for (size_t j = i + 1; j < genotype->size(); j++) {
			relativeOrderingInformation[i][j] += calculateRelativeOrderingInformation(genotype->at(i), genotype->at(j));
		}
	}
}

void Linkage::updateAdjacencyInformation(Solution* newSolution) {
	updateAdjacencyInformation(newSolution->getGenotypePtr());
}

void Linkage::updateAdjacencyInformation(vector<double>* genotype) {
	for(size_t i = 0; i < genotype->size() - 1; i++) {
		for(size_t j = i + 1; j < genotype->size(); j++) {
			adjacencyInformation[i][j] += calculateAdjacencyInformation(genotype->at(i), genotype->at(j));
		}
	}
}

double Linkage::calculateRelativeOrderingInformation(double firstGeneValue, double secondGeneValue) {
	if(firstGeneValue < secondGeneValue) {
		return 1;
	} else {
		return 0;
	}
}

double Linkage::calculateAdjacencyInformation(double firstGeneValue, double secondGeneValue) {
	double difference = firstGeneValue - secondGeneValue;
	return difference * difference;
}
