//
// Created by Szymon on 22.11.2018.
//

#include "../../include/order-p3/optimizer/Linkage.h"

Linkage::Linkage(int numberOfGenes, vector<int> &geneDomain) {
    this->numberOfGenes = numberOfGenes;
    this->geneDomain = geneDomain;
    initializeDistanceMatrix();
	initGeneValueIndices();
	initGeneOccurrences();
	createCachedDistances();
}

void Linkage::initializeDistanceMatrix(){
    distanceMeasureMatrix = new vector<vector<double>*>(numberOfGenes);
    for(int i = 0; i < numberOfGenes; i++){
        (*distanceMeasureMatrix)[i] = new vector<double>(numberOfGenes);
    }
}

Linkage::~Linkage() {
    deleteMatrix();
	deleteGeneOccurrences();
}


void Linkage::deleteMatrix(){
    for(int i = 0; i < numberOfGenes; i++){
        delete (*distanceMeasureMatrix)[i];
    }
    delete distanceMeasureMatrix;
}

void Linkage::recalculate(int currentPopulationSize, vector<int>& solution, std::mt19937& random) {
	updateGeneOccurrences(solution);
	if(currentPopulationSize > 1) {
		recalculateDistances(currentPopulationSize);
		buildTree(random);
	}
}

void Linkage::recalculateDistances(int currentPopulationSize) {
	double dIJ = 0;
	double hIJ = 0;
	double dMIJ = 0;
	int domainSize = geneDomain.size();
	std::vector<double> geneValuePairProbs(domainSize * domainSize, 0);
	std::vector<double> iGeneValueProbs(domainSize, 0);
	std::vector<double> jGeneValueProbs(domainSize, 0);
	int *iJGeneOccurrences;
	for(int i = 0; i < numberOfGenes - 1; i++) {
		for(int j = i + 1; j < numberOfGenes; j++) {
			dIJ = 0;
			hIJ = 0;
			iJGeneOccurrences = geneOccurrences[i][j];
			for(int k = 0; k < domainSize * domainSize; k++) {
				geneValuePairProbs[k] = iJGeneOccurrences[k] / double(currentPopulationSize);
				iGeneValueProbs[k / domainSize] += iJGeneOccurrences[k];
				jGeneValueProbs[k % domainSize] += iJGeneOccurrences[k];
			}
			for(int k = 0; k < domainSize; k++) {
				iGeneValueProbs[k] /= currentPopulationSize;
				jGeneValueProbs[k] /= currentPopulationSize;
			}

			for(int k = 0; k < domainSize * domainSize; k++) {
				double pij = geneValuePairProbs[k];
				double pi = iGeneValueProbs[k / domainSize];
				double pj = jGeneValueProbs[k % domainSize];
				if (pij != 0) {
					if (pi != 0 && pj != 0) {
						dIJ += pij * log(pij / (pi * pj));
					}
					hIJ += pij * log(pij);
				}
			}
			hIJ *= -1;
			if (hIJ == 0) {
				dMIJ = 0;
			}
			else {
				dMIJ = (hIJ - dIJ) / hIJ;
			}
			distanceMeasureMatrix->at(i)->at(j) = dMIJ;
			std::fill(iGeneValueProbs.begin(), iGeneValueProbs.end(), 0);
			std::fill(jGeneValueProbs.begin(), jGeneValueProbs.end(), 0);
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




double Linkage::getDistanceMeasure(int fstGeneIndex, int sndGeneIndex) {
    if(fstGeneIndex > sndGeneIndex) {
        return getDistanceMeasure(sndGeneIndex, fstGeneIndex);
    }
    return distanceMeasureMatrix->at(fstGeneIndex)->at(sndGeneIndex);
}

void Linkage::clearClusters() {
    clusters.clear();
}

void Linkage::buildTree(std::mt19937& random) {
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
		std::shuffle(options.begin() + startIndex, options.end(), random);
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
	clusters = useful;
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

const vector<vector<int>> &Linkage::getClusters() const {
	return clusters;
}

double Linkage::getDistanceBetweenClusters(vector<int> &first, vector<int> &second) {
	return getDistanceBetweenClusters(first, second, 0, first.size(), 0, second.size());
}

void Linkage::initGeneValueIndices() {
	int geneValue;
	for(int i = 0; i < geneDomain.size(); i++) {
		geneValue = geneDomain[i];
		geneValueIndices.insert({ geneValue, i });
	}
}

void Linkage::initGeneOccurrences() {
	for(int i = 0; i < numberOfGenes - 1; i++) {
		geneOccurrences.insert({ i, unordered_map<int, int*>() });
		for(int j = i + 1; j < numberOfGenes; j++) {
			int* occurrences = new int[geneDomain.size() * geneDomain.size()]();
			geneOccurrences[i][j] = occurrences;
		}
	}
}

void Linkage::deleteGeneOccurrences() {
	for (int i = 0; i < numberOfGenes - 1; i++) {
		for (int j = i + 1; j < numberOfGenes; j++) {
			delete geneOccurrences[i][j];
		}
	}
}

void Linkage::updateGeneOccurrences(std::vector<int>& solution) {
	int geneIValue;
	int geneJValue;
	int geneValuePairIndex;
	for (int i = 0; i < numberOfGenes; i++) {
		for (int j = i + 1; j < numberOfGenes; j++) {
			geneIValue = solution[i];
			geneJValue = solution[j];
			geneValuePairIndex = geneValueIndices[geneIValue] * geneDomain.size() + geneValueIndices[geneJValue];
			geneOccurrences[i][j][geneValuePairIndex]++;
		}
	}
}

void Linkage::updateRelativeOrderingInformation(Solution* newSolution) {
	updateRelativeOrderingInformation(newSolution->getGenotypeRef());
}

void Linkage::updateRelativeOrderingInformation(vector<double>* genotype) {
	for (size_t i = 0; i < genotype->size() - 1; i++) {
		for (size_t j = i + 1; j < genotype->size(); j++) {
			relativeOrderingInformation[i][j] += calculateRelativeOrderingInformation(genotype->at(i), genotype->at(j));
		}
	}
}

void Linkage::updateAdjacencyInformation(Solution* newSolution) {
	updateAdjacencyInformation(newSolution->getGenotypeRef());
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
