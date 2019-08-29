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
using std::vector;
using std::string;
using std::unordered_map;


class Linkage {
public:
    Linkage(int numberOfGenes, vector<int> &geneDomain);
    ~Linkage();
	void recalculate(int currentPoplationSize, vector<int> &solution, std::mt19937& random);
    void print();
    const vector<vector<int>> &getClusters() const;
private:
	void updateGeneOccurrences(std::vector<int>& solution);
	void recalculateDistances(int currentPopulationSize);
	void initGeneValueIndices();
	void initGeneOccurrences();
	void deleteGeneOccurrences();
    double getDistanceBetweenClusters(vector<int> &first, vector<int> &second);
	double getDistanceBetweenClusters(vector<int> &first, vector<int> &second,
		int firstStart, int firstEnd, int secondStart, int secondEnd);
    double getDistanceMeasure(int fstGeneIndex, int sndGeneIndex);
    void initializeDistanceMatrix();
    void deleteMatrix();
    void buildTree(std::mt19937& random);
    void clearClusters();
    int numberOfGenes;
    vector<vector<int>> clusters;
    vector<int> geneDomain;
    vector<vector<double> *> *distanceMeasureMatrix;
	unordered_map<int, unordered_map<int, int*>> geneOccurrences;
	unordered_map<int, int> geneValueIndices;

	void createCachedDistances();
	void resetCachedDistances();
	vector<vector<double>> cachedDistances;

    void printDistanceMatrix();

    void printClusters();
};