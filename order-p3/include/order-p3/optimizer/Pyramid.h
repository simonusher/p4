//
// Created by Szymon on 24.11.2018.
//
#pragma once
#include <unordered_set>
#include <vector>
#include <random>
#include "../util/VectorHasher.h"
#include "solution/SolutionFactory.h"
#include "../util/PairHasher.h"
#include "../local_optimizers/LocalOptimizer.h"
#include <functional>
#include "population/PopulationFactory.h"

using std::unordered_set;
using std::vector;

class Pyramid {
public:
	Pyramid(
		Problem& problem, 
		SolutionFactory& solutionFactory, 
		PopulationFactory& populationFactory, 
		LocalOptimizer& localOptimizer, 
		std::function<void(Solution* solution)> onBestSolutionFound = nullptr
	);
	virtual ~Pyramid();
    virtual void runSingleIteration();
    vector<int> getBestSolutionPhenotype() const;
	Solution* getBestSolution() const;
    double getBestFitness() const;

	int getIterationsRun();

	int getNumberOfPopulations();
	
protected:
	Solution* tryToAddImprovedSolutions(Solution* solution, int level);
    Solution* tryAddSolutionToPyramid(Solution* solution);
	Solution* tryAddSolutionToPyramid(Solution* solution, int level);
    bool addSolutionToPyramidIfUnique(Solution* solution, int level);
	void addSolutionToPyramid(Solution* solution, int level);
	void ensurePyramidCapacity(int level);
	void checkIfBest(Solution* solution);
	int iterationsRun;
	Solution* bestSolution;
    vector<Population*> populations;
	Problem& problem;
	SolutionFactory& solutionFactory;
	PopulationFactory& populationFactory;
	LocalOptimizer& localOptimizer;
	std::function<void(Solution * solution)> onBestSolutionFound;
};