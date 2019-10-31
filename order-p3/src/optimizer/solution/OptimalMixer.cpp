#include "../../../include/order-p3/optimizer/solution/OptimalMixer.h"

OptimalMixer::OptimalMixer(Problem* problem) : problem(problem) {}

bool OptimalMixer::mix(Solution* destination, Solution* source, std::vector<int>* cluster) {
	setSourceSolution(source);
	setDestinationSolution(destination);
	setCluster(cluster);
	return mixGenotypes();
}

void OptimalMixer::setDestinationSolution(Solution* destinationSolution) {
	this->destinationSolution = destinationSolution;
	this->destinationGenotype = destinationSolution->getGenotypePtr();
}

void OptimalMixer::setSourceSolution(Solution* sourceSolution) {
	this->sourceSolution = sourceSolution;
	this->sourceGenotype = sourceSolution->getGenotypePtr();
}

bool OptimalMixer::mixGenotypes() {
	bool anyGeneChanged = swapSolutionsGenesInCluster();
	
	if (anyGeneChanged) {
		handleGenotypeChange();
	}
	return anyGeneChanged;
}

void OptimalMixer::handleGenotypeChange() {
	double oldFitness = destinationSolution->getFitness();
	std::vector<int> oldPhenotype(destinationSolution->getPhenotype());
	double newFitness = destinationSolution->evaluate(*problem);

	if (oldFitness <= newFitness) {
		revertSource();
	}
	else {
		swapSolutionsGenesInCluster();
		destinationSolution->setPhenotype(oldPhenotype);
		destinationSolution->setFitness(oldFitness);
	}
}

bool OptimalMixer::swapSolutionsGenesInCluster() {
	bool solutionsDiffer = false;
	for (const int& index : *cluster) {
		solutionsDiffer |= (*destinationGenotype)[index] != (*sourceGenotype)[index];
		std::swap((*destinationGenotype)[index], (*sourceGenotype)[index]);
	}
	return solutionsDiffer;
}

void OptimalMixer::setCluster(std::vector<int>* cluster) {
	this->cluster = cluster;
}

void OptimalMixer::revertSource() {
	for (const int& index : *cluster) {
		(*sourceGenotype)[index] = (*destinationGenotype)[index];
	}
}
