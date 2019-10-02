#pragma once
#include "LocalOptimizer.h"
#include "../../order-p3/util/PairHasher.h"
#include "../optimizer/encoding/RandomKeyEncoder.h"
#include "../optimizer/solution/Solution.h"
#include <unordered_set>
#include <memory>

class SwapHillClimber: public LocalOptimizer {
public:
	SwapHillClimber(Problem* problem, RandomKeyEncoder* encoder);
	void optimize(Solution& solution) override;
private:
	void hillClimb(Solution& solution);
	void initialize();
	void generateAllIndexPairs();
	void initializeRandomGenerator();
	void resetHelperFields();
	void initializeSolutionData(Solution& solution);
	void runOptimizationIteration();
	void noteImprovement(double newFitness);
	void trySwappingPossiblePairs();
	void saveImprovedSolution(Solution& solution);
	
	std::vector<int>* solutionPhenotypePtr;
	double currentFitness;
	std::unordered_set<std::pair<int, int>*> triedIndexPairs;
	bool improvementMadeLastIteration;
	bool anyImprovementMade;
	
	std::vector<std::unique_ptr<std::pair<int, int>>> possibleIndexPairs;
	std::mt19937 randomGenerator;
	RandomKeyEncoder* encoder;
};
