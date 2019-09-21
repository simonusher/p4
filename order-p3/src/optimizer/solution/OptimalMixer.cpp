#include "../../../include/order-p3/optimizer/solution/OptimalMixer.h"

OptimalMixer::OptimalMixer(Problem* problem, RandomKeyDecoder* decoder) : problem(problem), decoder(decoder) {}

bool OptimalMixer::mix(Solution* destination, Solution* source, std::vector<int>* cluster) {
	destinationSolution = destination;
	sourceSolution = source;
	destinationGenotype = destination->getPhenotypePtr();
	sourceGenotype = source->getPhenotypePtr();
	this->cluster = cluster;
	return mixPhenotypes();
}

bool OptimalMixer::mixPhenotypes() const {
	bool anyGeneChanged = swapSolutionsGenesInCluster();
	
	if (anyGeneChanged) {
		handleGenotypeChange();
	}
	return anyGeneChanged;
}

void OptimalMixer::handleGenotypeChange() const {
	double oldFitness = destinationSolution->getFitness();
	destinationSolution->recalculatePhenotype(*decoder);
	double newFitness = destinationSolution->evaluate(*problem);

	if (oldFitness <= newFitness) {
		revertSource();
	}
	else {
		swapSolutionsGenesInCluster();
		destinationSolution->setFitness(oldFitness);
	}
}

bool OptimalMixer::swapSolutionsGenesInCluster() const {
	bool solutionsDiffer = false;
	for (const int& index : *cluster) {
		solutionsDiffer |= (*destinationGenotype)[index] != (*sourceGenotype)[index];
		std::swap((*destinationGenotype)[index], (*sourceGenotype)[index]);
	}
	return solutionsDiffer;
}

void OptimalMixer::revertSource() const {
	for (const int& index : *cluster) {
		(*sourceGenotype)[index] = (*destinationGenotype)[index];
	}
}
