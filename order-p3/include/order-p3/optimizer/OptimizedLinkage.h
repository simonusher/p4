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


class OptimizedLinkage {
public:
	OptimizedLinkage(int problemSize, std::mt19937& randomGenerator);
	void update(Solution* newSolution, int currentPopulationSize);
	void update(const std::vector<Solution*>& population);

	class ClusterIterator {
		public:
			ClusterIterator(size_t currentIndex, OptimizedLinkage& linkage);
			bool operator!=(const ClusterIterator& other) const;
			std::vector<int>& operator*() const;
			ClusterIterator& operator++();
		private:
			size_t currentClusterOrderingIndex;
			OptimizedLinkage& linkage;
	};

	class RandomClusterIterator {
		public:
			RandomClusterIterator(OptimizedLinkage& linkage, bool end = false);
			bool operator!=(const RandomClusterIterator& other) const;
			std::vector<int>& operator*() const;
			RandomClusterIterator& operator++();
		private:
			int unused;
			int currentIndex;
			void generateNextIndex();
			std::uniform_int_distribution<int> indexDistribution;
			std::vector<int> options;
			OptimizedLinkage& linkage;
	};
	
	ClusterIterator begin();
	ClusterIterator end();

	RandomClusterIterator randomBegin();
	RandomClusterIterator randomEnd();
	static vector<vector<double>> preprocessedLinkage;
	static bool usePreprocessedLinkage;
	
private:
	double getDistance(int firstIndex, int secondIndex);
	void rebuildTree();
	void updateLinkageInformation(Solution* solution, int currentPopulationSize);
	double calculateDependencyBetweenGenes(const int firstGeneIndex, const int secondGeneIndex, const int currentPopulationSize);
	double calculateRelativeOrderingMeasure(const int firstGeneIndex, const int secondGeneIndex, const int currentPopulationSize);
	double calculateAdjacencyMeasure(const int firstGeneIndex, const int secondGeneIndex, const int currentPopulationSize);
	static double calculateRelativeOrderingInformation(double firstGeneValue, double secondGeneValue);
	static double calculateAdjacencyInformation(double firstGeneValue, double secondGeneValue);
	int problemSize;
	std::mt19937& randomGenerator;
	vector<vector<double>> relativeOrderingInformationSum;
	vector<vector<double>> adjacencyInformationSum;
	vector<vector<double>> distanceMeasureMatrix;
	vector<vector<int>> clusters;
	vector<int> clusterOrdering;

	static vector<vector<double>> distances;
};