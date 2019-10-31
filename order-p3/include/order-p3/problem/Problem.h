#pragma once
#include <vector>

class Problem {
public:
	virtual ~Problem() = default;
	virtual double evaluate(std::vector<int>& solution) = 0;
	// virtual double evaluate(std::vector<double>& solution) = 0;
	virtual int getProblemSize() = 0;
	virtual int getFitnessFunctionEvaluations() = 0;
};
