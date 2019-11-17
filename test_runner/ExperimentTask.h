#pragma once
#include <functional>
#include "../order-p3/include/order-p3/problem/Problem.h"
#include "../order-p3/include/order-p3/optimizer/Pyramid.h"

class ExperimentTask {
public:

	ExperimentTask(int flowshopIndex, bool useRescaling, bool useReencoding, int numberOfRuns, int ffeBudget, const std::string& outputPath,
		double knownBestSolutionFitness);

	void execute();

	int getFlowshopIndex() const;
	
private:
	void runExperiment(int experimentNumber);
	void writeSummary();
	int flowshopIndex;
	bool useRescaling;
	bool useReencoding;
	int numberOfRuns;
	int ffeBudget;
	double knownBestSolutionFitness;
	std::string outputPath;
	std::vector<std::pair<int, int>> bestSolutionsWithFfeFoundInRuns;
};
