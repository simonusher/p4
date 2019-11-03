#include "../../include/order-p3/problem/SortFunctionProblem.h"

SortFunctionProblem::SortFunctionProblem(int problemSize) {
	this->problemSize = problemSize;
}

double SortFunctionProblem::evaluate(std::vector<int>& solution) {
	ffe++;
	int    i, j;
	double result;

	result = 0.0;
	for (i = 0; i < problemSize; i++)
		for (j = i + 1; j < problemSize; j++)
			result += solution[i] > solution[j] ? 0 : 1;
	return result;
}