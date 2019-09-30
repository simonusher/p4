#include "../../../include/order-p3/problem/deceptive_ordering/AbsoluteOrderingProblem.h"

AbsoluteOrderingProblem::AbsoluteOrderingProblem(int numberOfFunctions) {
	this->numberOfFunctions = numberOfFunctions;
	initializeFunctionValues();
	fitnessFunctionEvaluations = 0;
	maximumFitness = numberOfFunctions * 4.0;
}

double AbsoluteOrderingProblem::evaluate(std::vector<int>& solution) {
	fitnessFunctionEvaluations++;
	std::vector<int> elementIndices(4);
	std::vector<int>::const_iterator begin = solution.begin();
	std::vector<int>::const_iterator end;
	double sum = 0;
	for(int i = 0; i < numberOfFunctions; i++) {
		for(int j = 0; j < 4; j++) {
			int elementToFind = i * 4 + j;
			auto it = std::find(solution.begin(), solution.end(), elementToFind);
			int index = std::distance(solution.begin(), it) - i * 4;
			elementIndices[j] = index;
		}
		/*end = begin + 4;
		std::vector<int> subvector(begin, end);
		for(auto& elem : subvector) {
			elem -= (i * 4);
		}*/
		auto t = functionValues.find(elementIndices);
		if(t != functionValues.end()) {
			sum += t->second;
		} else {
			bool orderCorrect = true;
			int correctlyPlacedValues = 0;
			for(int i = 0; i < 3 && orderCorrect; i++) {
				if(elementIndices[i] >= elementIndices[i+1]) {
					orderCorrect = false;
				} else {
					if(elementIndices[i] == i) {
						correctlyPlacedValues++;
					}
				}
			}
			if(elementIndices[3] == 3) {
				correctlyPlacedValues++;
			}
			if(orderCorrect) {
				sum += correctlyPlacedValues / 2;
			}
		}
		// begin = begin + 4;
	}
	return sum / maximumFitness;
}

int AbsoluteOrderingProblem::getProblemSize() {
	return numberOfFunctions * 4;
}

int AbsoluteOrderingProblem::getFitnessFunctionEvaluations() {
	return fitnessFunctionEvaluations;
}

void AbsoluteOrderingProblem::initializeFunctionValues() {
	functionValues[{0, 1, 2, 3}] = 4.0;
	functionValues[{3, 1, 2, 0}] = 1.8;
	functionValues[{0, 2, 1, 3}] = 1.8;
	functionValues[{0, 1, 3, 2}] = 1.8;
	functionValues[{0, 3, 2, 1}] = 1.8;
	functionValues[{2, 1, 0, 3}] = 1.8;
	functionValues[{1, 0, 2, 3}] = 1.8;
	functionValues[{3, 1, 0, 2}] = 2.0;
	functionValues[{3, 0, 2, 1}] = 2.0;
	functionValues[{2, 0, 1, 3}] = 2.0;
	functionValues[{0, 2, 3, 1}] = 2.0;
	functionValues[{1, 2, 0, 3}] = 2.0;
	functionValues[{1, 3, 2, 0}] = 2.0;
	functionValues[{2, 1, 3, 0}] = 2.0;
	functionValues[{0, 3, 1, 2}] = 2.0;
	functionValues[{3, 0, 1, 2}] = 2.6;
	functionValues[{2, 3, 0, 1}] = 2.6;
	functionValues[{1, 2, 3, 0}] = 2.6;
	functionValues[{1, 3, 0, 2}] = 2.6;
	functionValues[{1, 0, 3, 2}] = 2.6;
	functionValues[{3, 2, 1, 0}] = 2.6;
	functionValues[{3, 2, 0, 1}] = 2.6;
	functionValues[{2, 0, 3, 1}] = 2.6;
	functionValues[{2, 3, 1, 0}] = 3.3;
}
