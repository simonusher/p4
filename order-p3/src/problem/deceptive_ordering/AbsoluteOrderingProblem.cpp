#include "../../../include/order-p3/problem/deceptive_ordering/AbsoluteOrderingProblem.h"

const std::unordered_map<std::vector<int>, double, VectorHasher> AbsoluteOrderingProblem::functionValues{
	{{0, 1, 2, 3}, 4.0},
	{{0, 1, 3, 2}, 1.8},
	{{0, 3, 2, 1}, 1.8},
	{{0, 2, 1, 3}, 1.8},
	{{2, 1, 0, 3}, 1.8},
	{{3, 1, 2, 0}, 1.8},
	{{1, 0, 2, 3}, 1.8},
	{{0, 2, 3, 1}, 2.0},
	{{0, 3, 1, 2}, 2.0},
	{{3, 1, 0, 2}, 2.0},
	{{2, 1, 3, 0}, 2.0},
	{{3, 0, 2, 1}, 2.0},
	{{1, 3, 2, 0}, 2.0},
	{{2, 0, 1, 3}, 2.0},
	{{1, 2, 0, 3}, 2.0},
	{{1, 0, 3, 2}, 2.6},
	{{3, 0, 1, 2}, 2.6},
	{{2, 0, 3, 1}, 2.6},
	{{2, 3, 0, 1}, 2.6},
	{{3, 2, 0, 1}, 2.6},
	{{1, 2, 3, 0}, 2.6},
	{{2, 3, 1, 0}, 3.3},
	{{1, 3, 0, 2}, 2.6},
	{{3, 2, 1, 0}, 2.6},
};

AbsoluteOrderingProblem::AbsoluteOrderingProblem(int numberOfFunctions) : numberOfFunctions(numberOfFunctions) {
	problemSize = numberOfFunctions * 4;
	maximumFitness = numberOfFunctions * 4.0;
}


double AbsoluteOrderingProblem::evaluate(std::vector<int>& solution) {
	ffe++;
	std::vector<int> elementIndices(4);
	double sum = 0;
	std::vector<int> indices(4);
	std::iota(indices.begin(), indices.end(), 0);

	auto startingElement = solution.begin();
	for (int i = 0; i < numberOfFunctions; i++, startingElement += 4) {
		for (int j = 0; j < 4; j++) {
			int elementToFind = i * 4 + j;
			elementIndices[j] = std::distance(startingElement, std::find(solution.begin(), solution.end(), elementToFind));
		}

		auto it = functionValues.find(elementIndices);
		if (it != functionValues.end()) {
			sum += it->second;
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
				sum += correctlyPlacedValues / 2.0;
			}
		}
	}
	return sum / maximumFitness;
}