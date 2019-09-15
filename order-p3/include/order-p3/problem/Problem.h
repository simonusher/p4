#pragma once
#include <vector>

class Problem {
public:
	virtual ~Problem() = default;
	virtual double evaluate(std::vector<int>& solution) = 0;
	virtual int getProblemSize() = 0;
};