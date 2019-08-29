#pragma once
#include "Solution.h"

class BinarySolution: public Solution<int, int, double> {
public:
	std::vector<int>* getGenotype() override;
	std::vector<int>* getPhenotype() override;
	double getFitness() override;
private:
	std::vector<int> genotype;
	double fitness;
};
