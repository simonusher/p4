#pragma once
#include "SolutionMixer.h"

class OptimalMixer : public SolutionMixer {
public:
	OptimalMixer(Problem* problem, RandomKeyDecoder* decoder);
	bool mix(Solution* destination, Solution* source, std::vector<int>* cluster) override;
private:
	void revertSource() const;
	void handleGenotypeChange() const;
	bool swapSolutionsGenesInCluster() const;
	bool mixPhenotypes() const;
	
	Solution* destinationSolution;
	Solution* sourceSolution;
	std::vector<int>* destinationGenotype;
	std::vector<int>* sourceGenotype;
	std::vector<int>* cluster;

	Problem* problem;
	RandomKeyDecoder* decoder;
};
