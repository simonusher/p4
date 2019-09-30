//
// Created by Szymon on 22.11.2018.
//
#pragma once
#include <algorithm>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <string>
#include <iostream>
#include <ctime>
#include <random>
#include <numeric>
#include "solution/Solution.h"
using std::vector;
using std::string;
using std::unordered_map;


class Linkage {
public:
    Linkage(int numberOfGenes, std::mt19937& randomGenerator);
    ~Linkage();
	void recalculate(int currentPopulationSize, Solution* newSolution);
    void print();
    const vector<vector<int>> &getClusters() const;
private:
	void recalculateDistances(int currentPopulationSize);
	void buildTree();
    double getDistanceBetweenClusters(vector<int> &first, vector<int> &second);
	double getDistanceBetweenClusters(vector<int> &first, vector<int> &second,
		int firstStart, int firstEnd, int secondStart, int secondEnd);
    double getDistanceMeasure(int firstGeneIndex, int secondGeneIndex);
    void initializeDistanceMatrix();
	void initializeDistanceMeasures();
    void deleteMatrix();
    void clearClusters();

	void createCachedDistances();
	void resetCachedDistances();

    void printDistanceMatrix();

    void printClusters();

	
	double calculateDependencyBetweenGenes(int firstGeneIndex, int secondGeneIndex, int currentPopulationSize);
	double calculateRelativeOrderingInformation(int firstGeneIndex, int secondGeneIndex, int currentPopulationSize);
	double calculateAdjacencyInformation(int firstGeneIndex, int secondGeneIndex, int currentPopulationSize);
	
	void updatePopulationInformation(Solution* newSolution);
	void updateRelativeOrderingInformation(Solution* newSolution);
	void updateRelativeOrderingInformation(vector<double>* genotype);
	void updateAdjacencyInformation(Solution* newSolution);
	void updateAdjacencyInformation(vector<double>* genotype);

    static double calculateRelativeOrderingInformation(double firstGeneValue, double secondGeneValue);
	static double calculateAdjacencyInformation(double firstGeneValue, double secondGeneValue);

	int numberOfGenes;
	vector<vector<int>> clusters;
	vector<vector<double>*>* distanceMeasureMatrix;
	vector<vector<double>> cachedDistances;
	unordered_map<int, unordered_map<int, double>> relativeOrderingInformation;
	unordered_map<int, unordered_map<int, double>> adjacencyInformation;
	std::mt19937& randomGenerator;
};