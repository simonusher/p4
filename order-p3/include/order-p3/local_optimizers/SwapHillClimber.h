#pragma once
#include "HillClimber.h"
#include <unordered_set>
#include "../old/PairHasher.h"

class SwapHillClimber: public HillClimber {
public:
	SwapHillClimber(Problem* problem, RandomKeyEncoder* encoder);
	void hillClimb(Solution& solution) override;
private:
	void optimize(Solution& solution);
	void initialize();
	void generateAllIndexPairs();
	void initializeRandomGenerator();
	void resetHelperFields();
	void initializeSolutionData(const Solution& solution);
	void runOptimizationIteration();
	void noteImprovement(double newFitness);
	void trySwappingPossiblePairs();
	void saveImprovedSolution(Solution& solution);
	
	std::vector<int> currentPhenotype;
	double currentFitness;
	std::unordered_set<std::pair<int, int>, PairHasher> triedIndexPairs;
	bool improvementMadeLastIteration;
	bool anyImprovementMade;
	
	std::vector<std::pair<int, int>> possibleIndexPairs;
	std::mt19937 randomGenerator;
	RandomKeyEncoder* encoder;
};
