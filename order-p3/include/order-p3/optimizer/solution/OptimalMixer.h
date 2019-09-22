#pragma once
#include "SolutionMixer.h"

class OptimalMixer : public SolutionMixer {
public:
	OptimalMixer(Problem* problem, RandomKeyDecoder* decoder);
	bool mix(Solution* destination, Solution* source, std::vector<int>* cluster) override;

	void setDestinationSolution(Solution* destinationSolution);
	void setSourceSolution(Solution* sourceSolution);
	void setCluster(std::vector<int>* cluster);
private:
	void revertSource() const;
	void handleGenotypeChange() const;
	bool swapSolutionsGenesInCluster() const;
	bool mixGenotypes() const;
	
	Solution* destinationSolution;
	Solution* sourceSolution;
	std::vector<double>* destinationGenotype;
	std::vector<double>* sourceGenotype;
	std::vector<int>* cluster;

	Problem* problem;
	RandomKeyDecoder* decoder;
};
