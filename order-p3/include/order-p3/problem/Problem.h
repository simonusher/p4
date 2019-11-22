#pragma once
#include <vector>

class Problem {
public:
	Problem() {
		ffe = 0;
	}
	virtual ~Problem() = default;
	virtual double evaluate(std::vector<int>& solution) = 0;
	virtual int getProblemSize() {
		return problemSize;
	}
	
	virtual int getFitnessFunctionEvaluations() {
		return ffe;
	}
	virtual void reset() {
		ffe = 0;
	}
protected:
	int problemSize;
	int ffe;
};
