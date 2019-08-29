#include "../include/order-p3/solution/BinarySolution.h"
std::vector<int>* BinarySolution::getGenotype() {
	return &this->genotype;
}

std::vector<int>* BinarySolution::getPhenotype() {
	return this->getGenotype();
}

double BinarySolution::getFitness() {
	return this->fitness;
}
