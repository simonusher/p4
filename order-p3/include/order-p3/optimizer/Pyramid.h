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

class Pyramid : public LocalOptimizer {
public:
	Pyramid(Problem* problem, SolutionFactory* solutionFactory, PopulationFactory* populationFactory, LocalOptimizer* localOptimizer);
	virtual ~Pyramid();
    virtual void runSingleIteration();
    vector<int> getBestSolutionPhenotype() const;
	Solution* getBestSolution() const;
    double getBestFitness() const;

	int getIterationsRun();

	double mean();
	int getNumberOfPopulations();
	
	void optimizeLocally(Solution& solution) override;
protected:
	void reEncode();
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
	Problem* problem;
	SolutionFactory* solutionFactory;
	PopulationFactory* populationFactory;
	LocalOptimizer* localOptimizer;
};