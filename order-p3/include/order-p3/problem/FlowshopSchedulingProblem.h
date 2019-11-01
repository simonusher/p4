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
	
	int getProblemSize() override;
	int getFitnessFunctionEvaluations() override;
private:
	double sortFunctionProblemEvaluation(std::vector<int>& parameters);
	double taillardFlowshopProblemEvaluation(std::vector<int>& parameters);
	void loadTaillardFlowshop(int, int, char*, int);
	int problemIndex;
	int problemSize;
	int ffe = 0;
	int nJobs = 0;
	int nMachines = 0;
	std::vector<std::vector<int>> processingTimes;
	std::vector<std::vector<int>> fitnessCalculationCache;
};
