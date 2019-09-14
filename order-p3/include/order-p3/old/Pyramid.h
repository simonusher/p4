//
// Created by Szymon on 24.11.2018.
//
#pragma once
#include <unordered_set>
#include <vector>
#include <random>
#include "VectorHasher.h"
#include "Population.h"
#include "../SolutionFactory.h"

using std::unordered_set;
using std::vector;

class Pyramid {
public:
    Pyramid(vector <int> &geneDomain, CEvaluator& evaluator);
    ~Pyramid();
    void runSingleIteration();

    const vector<int> &getBestSolution() const;

    double getBestFitness() const;
	int getSeenIndividuals() const { return seen.size(); };

private:
    bool climb(vector<int>& solution, double &fitness);
    bool climb(vector<int>& solution, double &fitness, int level);
    vector<int> nextRandomSolution();
    void hillClimb(vector<int> &solution, double &fitness);
    int nextRandomGene();
    bool addUnique(vector<int> &solution, int level, double fitness);
    std::random_device randomDevice;
    std::mt19937 generator;
    std::uniform_int_distribution<> geneDistribution;
    int numberOfGenes;
    vector<int> geneDomain;
    vector<int> bestSolution;
    double bestFitness;
    unordered_set<vector<int>, VectorHasher>::const_iterator seenSolution;
    unordered_set<vector<int>, VectorHasher> seen;
    vector<Population*> populations;
	CEvaluator* evaluator;
	SolutionFactory* solutionFactory;
};