#pragma once
#include "SolutionMixer.h"

class OptimalMixer : public SolutionMixer {
public:
	OptimalMixer(Problem& problem);
	virtual ~OptimalMixer() = default;
	bool mix(Solution* destination, Solution* source, std::vector<int>* cluster) override;

	void setDestinationSolution(Solution* destinationSolution);
	void setSourceSolution(Solution* sourceSolution);
	void setCluster(std::vector<int>* cluster);
protected:
	virtual bool mixGenotypes();
	void revertSource();
	void handleGenotypeChange();
	bool swapSolutionsGenesInCluster();
	
	Solution* destinationSolution;
	Solution* sourceSolution;
	std::vector<double>* destinationGenotype;
	std::vector<double>* sourceGenotype;
	std::vector<int>* cluster;
	
	Problem& problem;
};
