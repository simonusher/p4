#pragma once
#include "SolutionMixer.h"

class OptimalMixer : public SolutionMixer {
public:
	OptimalMixer(Problem* problem);
	virtual ~OptimalMixer() = default;
	virtual bool mix(Solution* destination, Solution* source, std::vector<int>* cluster) override;

	virtual void setDestinationSolution(Solution* destinationSolution);
	virtual void setSourceSolution(Solution* sourceSolution);
	virtual void setCluster(std::vector<int>* cluster);
protected:
	virtual void revertSource();
	virtual void handleGenotypeChange();
	virtual bool swapSolutionsGenesInCluster();
	virtual bool mixGenotypes();
	
	Solution* destinationSolution;
	Solution* sourceSolution;
	std::vector<double>* destinationGenotype;
	std::vector<double>* sourceGenotype;
	std::vector<int>* cluster;
	
	Problem* problem;
};
