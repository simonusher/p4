#include "../../../include/order-p3/problem/deceptive_ordering/RelativeOrderingProblem.h"

const std::unordered_map<std::vector<int>, double, VectorHasher> RelativeOrderingProblem::functionValues{
	{{0, 1, 2, 3}, 4.0},
	{{0, 1, 3, 2}, 1.1},
	{{0, 3, 2, 1}, 1.1},
	{{0, 2, 1, 3}, 1.1},
	{{2, 1, 0, 3}, 1.1},
	{{3, 1, 2, 0}, 1.1},
	{{1, 0, 2, 3}, 1.1},
	{{0, 2, 3, 1}, 1.2},
	{{0, 3, 1, 2}, 1.2},
	{{3, 1, 0, 2}, 1.2},
	{{2, 1, 3, 0}, 1.2},
	{{3, 0, 2, 1}, 1.2},
	{{1, 3, 2, 0}, 1.2},
	{{2, 0, 1, 3}, 1.2},
	{{1, 2, 0, 3}, 1.2},
	{{1, 0, 3, 2}, 2.4},
	{{3, 0, 1, 2}, 2.1},
	{{2, 0, 3, 1}, 2.2},
	{{2, 3, 0, 1}, 2.2},
	{{3, 2, 0, 1}, 2.4},
	{{1, 2, 3, 0}, 1.5},
	{{2, 3, 1, 0}, 3.2},
	{{1, 3, 0, 2}, 2.4},
	{{3, 2, 1, 0}, 2.4},
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

	auto startingElement = solution.begin();
	for (int i = 0; i < numberOfFunctions; i++, startingElement += 4) {
		for(int j = 0; j < 4; j++) {
			int elementToFind = i * 4 + j;
			elementIndices[j] = std::distance(startingElement, std::find(solution.begin(), solution.end(), elementToFind));
		}
		std::vector<int> subfunctionIndices(indices);

		std::sort(subfunctionIndices.begin(), subfunctionIndices.end(), [&](int first, int second) { return elementIndices[first] < elementIndices[second];});
		sum += functionValues.at(subfunctionIndices);
	}
	return sum / maximumFitness;
}
