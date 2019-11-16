#pragma once
#include <vector>
#include "Pyramid.h"


class PyramidSizingP3 {
public:
	PyramidSizingP3(Problem* problem);
	~PyramidSizingP3();
	void runSingleIteration();
	double getBestFitness();
	Solution* getBestSolution();
private:
	int nextSize;
	Problem* problem;
	RandomKeyEncoder* encoder;
	RandomKeyDecoder* decoder;
	LocalOptimizer* localOptimizer;
	SolutionFactory* solutionFactory;
	PopulationFactory* populationFactory;
	SolutionMixer* solutionMixer;
	void ensureCapacity(int level);
	bool checkIfBestInPyramid(int pyramidIndex);
	void checkWhichPyramidsToStop();
	void runSingleIteration(int pyramidIndex);
	void stopAllPyramidsUntil(int pyramidIndex);
	std::vector<Pyramid*> pyramids;
	std::vector<int> pyramidSizes;
	std::vector<int> iterationsWithoutImprovement;
	int pyramid0IterationsWithoutImprovement;
	Solution* bestSolution;
	std::mt19937 randomGenerator;
};
