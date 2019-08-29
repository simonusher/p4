#pragma once
#include <vector>
class Solution {
public:
	std::vector<double>* getGenotype();
	std::vector<int>* getPhenotype();
private:
	void calculatePhenotype();
	std::vector<double> genotype;
	std::vector<int> phenotype;
	double fitness;
};


