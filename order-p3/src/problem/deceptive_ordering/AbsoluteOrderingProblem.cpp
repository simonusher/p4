
#include "../../../include/order-p3/problem/deceptive_ordering/AbsoluteOrderingProblem.h"
#include <random>

AbsoluteOrderingProblem::AbsoluteOrderingProblem(int numberOfFunctions) : problemCoding(numberOfFunctions * 4) {
	this->numberOfFunctions = numberOfFunctions;
	initializeFunctionValues();
	fitnessFunctionEvaluations = 0;
	maximumFitness = numberOfFunctions * 4.0;
	std::iota(problemCoding.begin(), problemCoding.end(), 0);
	std::random_device d;
	std::mt19937 generator(d());
	std::shuffle(problemCoding.begin(), problemCoding.end(), generator);
	// problemCoding = { 0, 8, 16, 24, 1, 9, 17, 25, 2, 10, 18, 26, 3, 11, 19, 27, 4, 12, 20, 28, 4, 13, 21, 29, 6, 14, 22, 30, 7, 15, 23, 31 };
	problemCoding = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
	// problemCoding = { 0, 2, 4, 6, 1, 3, 5, 7, 8, 10, 12, 14, 9, 11, 13, 15, 16, 18, 20, 22, 17, 19, 21, 23, 24, 26, 28, 30, 25, 27, 29, 31 };
}

double AbsoluteOrderingProblem::evaluateAbsolute(const std::vector<int>& solution) {
	fitnessFunctionEvaluations++;
	std::vector<int> elementIndices(4);
	double sum = 0;
	std::vector<int> indices(4);
	std::iota(indices.begin(), indices.end(), 0);

	for (int i = 0; i < numberOfFunctions; i++) {
		for (int j = 0; j < 4; j++) {
			int elementToFind = i * 4 + j;
			auto it = std::find(solution.begin(), solution.end(), elementToFind);
			int index = std::distance(solution.begin(), it) - i * 4;
			elementIndices[j] = index;
		}

		auto t = functionValues.find(elementIndices);
		if (t != functionValues.end()) {
			sum += t->second;
		}
		else {
			bool orderCorrect = true;
			int correctlyPlacedValues = 0;
			for (int i = 0; i < 3 && orderCorrect; i++) {
				if (elementIndices[i] >= elementIndices[i + 1]) {
					orderCorrect = false;
				}
				else {
					if (elementIndices[i] == i) {
						correctlyPlacedValues++;
					}
				}
			}
			if (elementIndices[3] == 3) {
				correctlyPlacedValues++;
			}
			if (orderCorrect) {
				sum += correctlyPlacedValues / 2;
			}
		}
	}
	return sum / maximumFitness;
}

double AbsoluteOrderingProblem::evaluateRelative(const std::vector<int>& solution) {
	fitnessFunctionEvaluations++;
	std::vector<int> elementIndices(4);
	double sum = 0;
	std::vector<int> indices(4);
	std::iota(indices.begin(), indices.end(), 0);

	for (int i = 0; i < numberOfFunctions; i++) {
		for (int j = 0; j < 4; j++) {
			int elementToFind = i * 4 + j;
			auto it = std::find(solution.begin(), solution.end(), elementToFind);
			int index = std::distance(solution.begin(), it);
			elementIndices[j] = index;
		}

		std::vector<int> currentOrdering(indices);
		std::sort(currentOrdering.begin(), currentOrdering.end(), [&](int first, int second) { return elementIndices[first] < elementIndices[second]; });
		std::vector<int> finalOrdering(4);
		for (int j = 0; j < 4; j++) {
			finalOrdering[currentOrdering[j]] = j;
		}
		sum += relativeFunctionValues[currentOrdering];
	}
	return sum / maximumFitness;
}

double AbsoluteOrderingProblem::evaluate(std::vector<int>& solution) {
	return evaluateAbsolute(solution);
	// return evaluateRelative(solution);
}


int AbsoluteOrderingProblem::getProblemSize() {
	return numberOfFunctions * 4;
}

int AbsoluteOrderingProblem::getFitnessFunctionEvaluations() {
	return fitnessFunctionEvaluations;
}

double AbsoluteOrderingProblem::evaluate(std::vector<double>& solutionGenotype) {
	std::vector<double> copy(solutionGenotype.size());
	for (int i = 0; i < problemCoding.size(); i++) {
		copy[i] = solutionGenotype[problemCoding[i]];
	}
	std::vector<int> phenotype(copy.size());
	std::iota(phenotype.begin(), phenotype.end(), 0);
	std::sort(phenotype.begin(), phenotype.end(),
		[&copy](int firstIndex, int secondIndex) {
			return copy[firstIndex] < copy[secondIndex];
		});

	return evaluate(phenotype);
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


	relativeFunctionValues[{0, 1, 2, 3}] = 4.0;
	relativeFunctionValues[{0, 1, 3, 2}] = 1.1;
	relativeFunctionValues[{0, 2, 1, 3}] = 1.1;
	relativeFunctionValues[{0, 3, 2, 1}] = 1.1;
	relativeFunctionValues[{1, 0, 2, 3}] = 1.1;
	relativeFunctionValues[{2, 1, 0, 3}] = 1.1;
	relativeFunctionValues[{3, 1, 2, 0}] = 1.1;
	relativeFunctionValues[{1, 3, 2, 0}] = 1.2;
	relativeFunctionValues[{1, 2, 0, 3}] = 1.2;
	relativeFunctionValues[{2, 0, 1, 3}] = 1.2;
	relativeFunctionValues[{0, 2, 3, 1}] = 1.2;
	relativeFunctionValues[{2, 1, 3, 0}] = 1.2;
	relativeFunctionValues[{3, 1, 0, 2}] = 1.2;
	relativeFunctionValues[{3, 0, 2, 1}] = 1.2;
	relativeFunctionValues[{0, 3, 1, 2}] = 1.2;
	relativeFunctionValues[{1, 2, 3, 0}] = 1.5;
	relativeFunctionValues[{3, 0, 1, 2}] = 2.1;
	relativeFunctionValues[{2, 3, 0, 1}] = 2.2;
	relativeFunctionValues[{2, 0, 3, 1}] = 2.2;
	relativeFunctionValues[{1, 0, 3, 2}] = 2.4;
	relativeFunctionValues[{3, 2, 1, 0}] = 2.4;
	relativeFunctionValues[{3, 2, 0, 1}] = 2.4;
	relativeFunctionValues[{1, 3, 0, 2}] = 2.4;
	relativeFunctionValues[{2, 3, 1, 0}] = 3.2;

}
