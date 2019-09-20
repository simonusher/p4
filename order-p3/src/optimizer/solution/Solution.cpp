#include "../../../include/order-p3/optimizer/solution/Solution.h"

Solution::Solution(RandomKeyEncoder& encoder, RandomKeyDecoder& decoder): genotype(encoder.getRandomEncoding()), phenotype(encoder.getNumberOfGenes()) {
	recalculatePhenotype(decoder);
}

void Solution::setPhenotype(std::vector<int>& newPhenotype, RandomKeyEncoder& encoder) {
	phenotype = newPhenotype;
	recalculateGenotype(encoder);
}

void Solution::setGenotype(std::vector<double>& newGenotype, RandomKeyDecoder& decoder) {
	this->genotype = newGenotype;
	recalculatePhenotype(decoder);
}

std::vector<double> Solution::getGenotype() const {
	return genotype;
}

std::vector<int> Solution::getPhenotype() const {
	return phenotype;
}

std::vector<double>* Solution::getGenotypePtr() {
	return &genotype;
}

std::vector<int>* Solution::getPhenotypePtr() {
	return &phenotype;
}

double Solution::evaluate(Problem& problem) {
	this->fitness = problem.evaluate(this->phenotype);
	return fitness;
}

double Solution::getFitness() const { return fitness; }

void Solution::setFitness(double newFitness) {
	this->fitness = newFitness;
}

void Solution::recalculatePhenotype(RandomKeyDecoder& decoder) {
	phenotype = decoder.decode(genotype);
}

void Solution::recalculateGenotype(RandomKeyEncoder& encoder) {
	genotype = encoder.getEncodingForPhenotype(phenotype);
}