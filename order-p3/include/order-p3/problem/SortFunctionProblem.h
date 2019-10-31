#pragma once
#include "Problem.h"

class SortFunctionProblem : public Problem {
public:
	explicit SortFunctionProblem(int problemSize);

	double evaluate(std::vector<int>& solution) override;
	int getProblemSize() override;
	int getFitnessFunctionEvaluations() override;
private:
	int ffe;
	int problemSize;
};