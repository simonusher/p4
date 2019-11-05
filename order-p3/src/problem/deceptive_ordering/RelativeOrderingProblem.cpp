#include "../../../include/order-p3/problem/deceptive_ordering/RelativeOrderingProblem.h"

const std::unordered_map<std::vector<int>, double, VectorHasher> RelativeOrderingProblem::functionValues {
	{ {0, 1, 2, 3}, 4.0 },
	{ {0, 1, 3, 2}, 1.1 },
	{ {0, 2, 1, 3}, 1.1 },
	{ {0, 3, 2, 1}, 1.1 },
	{ {1, 0, 2, 3}, 1.1 },
	{ {2, 1, 0, 3}, 1.1 },
	{ {3, 1, 2, 0}, 1.1 },
	{ {1, 3, 2, 0}, 1.2 },
	{ {1, 2, 0, 3}, 1.2 },
	{ {2, 0, 1, 3}, 1.2 },
	{ {0, 2, 3, 1}, 1.2 },
	{ {2, 1, 3, 0}, 1.2 },
	{ {3, 1, 0, 2}, 1.2 },
	{ {3, 0, 2, 1}, 1.2 },
	{ {0, 3, 1, 2}, 1.2 },
	{ {1, 2, 3, 0}, 1.5 },
	{ {3, 0, 1, 2}, 2.1 },
	{ {2, 3, 0, 1}, 2.2 },
	{ {2, 0, 3, 1}, 2.2 },
	{ {1, 0, 3, 2}, 2.4 },
	{ {3, 2, 1, 0}, 2.4 },
	{ {3, 2, 0, 1}, 2.4 },
	{ {1, 3, 0, 2}, 2.4 },
	{ {2, 3, 1, 0}, 3.2 },
};

RelativeOrderingProblem::RelativeOrderingProblem(int numberOfFunctions): numberOfFunctions(numberOfFunctions) {
	problemSize = numberOfFunctions * 4;
	maximumFitness = numberOfFunctions * 4.0;
}

double RelativeOrderingProblem::evaluate(std::vector<int>& solution) {
	ffe++;
	std::vector<int> elementIndices(4);
	double sum = 0;
	std::vector<int> indices(4);
	std::iota(indices.begin(), indices.end(), 0);

	int startIndex = 0;
	for (int i = 0; i < numberOfFunctions; i++) {
		std::vector<int> subfunctionIndices(indices);

		std::sort(subfunctionIndices.begin(), subfunctionIndices.end(), [&](int first, int second) { return solution[first + startIndex] < solution[second + startIndex];});
		sum += functionValues.at(subfunctionIndices);
		startIndex += 4;
	}
	return sum / maximumFitness;
}
