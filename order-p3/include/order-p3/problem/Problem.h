#pragma once
#include "../Solution.h"

class Problem {
public:
	virtual ~Problem() = default;
	virtual double evaluate(Solution& solution);
	virtual double evaluate(std::vector<int>& solution) = 0;
	virtual int getProblemSize() = 0;
};