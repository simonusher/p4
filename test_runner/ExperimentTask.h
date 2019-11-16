#pragma once
#include <functional>
#include "../order-p3/include/order-p3/problem/Problem.h"
#include "../order-p3/include/order-p3/optimizer/Pyramid.h"

class ExperimentTask {
public:

	ExperimentTask(int flowshopIndex, bool useRescaling, bool useReencoding, int numberOfRuns, int ffeBudget, const std::string& outputPath);

	void execute();

	int getFlowshopIndex() const;
	
private:
	int flowshopIndex;
	bool useRescaling;
	bool useReencoding;
	int numberOfRuns;
	int ffeBudget;
	std::string outputPath;
};
