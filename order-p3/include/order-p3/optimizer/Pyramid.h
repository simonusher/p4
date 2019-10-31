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
#include "PopulationFactory.h"

using std::unordered_set;
using std::vector;

class Pyramid {
public:
	Pyramid(Problem* problem, SolutionFactory* solutionFactory, PopulationFactory* populationFactory, LocalOptimizer* localOptimizer, bool removeDuplicates = true);
	virtual ~Pyramid();
    virtual void runSingleIteration();
    vector<int> getBestSolutionPhenotype() const;
	Solution* getBestSolution() const;
    double getBestFitness() const;

protected:
	void tryToAddImprovedSolutions(Solution* solution, int level);
	Solution* bestSolution;
    bool tryAddSolutionToPyramid(Solution* solution);
    bool tryAddSolutionToPyramid(Solution* solution, int level);
    bool addSolutionToPyramidIfUnique(Solution* solution, int level);
	void addSolutionToPyramid(Solution* solution, int level);
	void ensurePyramidCapacity(int level);
	void checkIfBest(Solution* solution);

    unordered_set<vector<int>, VectorHasher>::const_iterator seenSolution;
    unordered_set<vector<int>, VectorHasher> seen;
    vector<Population*> populations;
	Problem* problem;
	SolutionFactory* solutionFactory;
	PopulationFactory* populationFactory;
	LocalOptimizer* localOptimizer;
	bool removeDuplicates;
};