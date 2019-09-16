//
// Created by Szymon on 24.11.2018.
//
#pragma once
#include <unordered_set>
#include <vector>
#include <random>
#include "Population.h"
#include "../util/VectorHasher.h"
#include "solution/SolutionFactory.h"
#include "../util/PairHasher.h"
#include "../local_optimizers/LocalOptimizer.h"

using std::unordered_set;
using std::vector;

class Pyramid {
public:
	Pyramid(Problem* problem, SolutionFactory* solutionFactory, LocalOptimizer* localOptimizer);
    void runSingleIteration();
    vector<int> getBestSolutionPhenotype() const;
	Solution* getBestSolution() const;
    double getBestFitness() const;

private:
    bool tryAddSolutionToPyramid(Solution* solution);
    bool tryAddSolutionToPyramid(Solution* solution, int level);
    bool addSolutionToPyramidIfUnique(Solution* solution, int level);
	void ensurePyramidCapacity(int level);
	void checkIfBest(Solution* solution);

	Solution* bestSolution;
    unordered_set<vector<int>, VectorHasher>::const_iterator seenSolution;
    unordered_set<vector<int>, VectorHasher> seen;
    vector<unique_ptr<Population>> populations;
	Problem* problem;
	SolutionFactory* solutionFactory;
	LocalOptimizer* localOptimizer;
};