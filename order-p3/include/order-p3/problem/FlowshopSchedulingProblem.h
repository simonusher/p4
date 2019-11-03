#pragma once
#include "Problem.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>


class FlowshopSchedulingProblem : public Problem {
public:
	void initializeProblem(int index);
	double evaluate(std::vector<int>& solution) override;
	
private:
	void loadTaillardFlowshop(int, int, char*, int);
	int problemIndex;
	int nJobs = 0;
	int nMachines = 0;
	std::vector<std::vector<int>> processingTimes;
	std::vector<std::vector<int>> fitnessCalculationCache;
};
