#include "../../../include/order-p3/optimizer/solution/Solution.h"

Solution::Solution(std::vector<double> genotype, std::vector<int> phenotype, double fitness): genotype(genotype), phenotype(phenotype), fitness(fitness) {}

Solution::Solution(RandomKeyEncoder& encoder, RandomKeyDecoder& decoder): genotype(encoder.getRandomEncoding()), phenotype(encoder.getNumberOfGenes()) {
	recalculatePhenotype(decoder);
}

void Solution::setPhenotypeAndRecalculateGenotype(std::vector<int>& newPhenotype, RandomKeyEncoder& encoder) {
	setPhenotype(newPhenotype);
	recalculateGenotype(encoder);
}

void Solution::setGenotypeAndRecalculatePhenotype(std::vector<double>& newGenotype, RandomKeyDecoder& decoder) {
	setGenotype(newGenotype);
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

void Solution::setGenotype(std::vector<double>& newGenotype) {
	genotype = newGenotype;
}

void Solution::setPhenotype(std::vector<int>& newPhenotype) {
	phenotype = newPhenotype;
}

void Solution::recalculatePhenotype(RandomKeyDecoder& decoder) {
	phenotype = decoder.decode(genotype);
}

void Solution::recalculateGenotype(RandomKeyEncoder& encoder) {
	genotype = encoder.getEncodingForPhenotype(phenotype);
}