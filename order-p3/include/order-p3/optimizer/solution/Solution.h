#pragma once
#include <vector>
#include "../encoding/RandomKeyEncoder.h"
#include "../encoding/RandomKeyDecoder.h"
#include "../../problem/Problem.h"

class Solution
{
public:
	Solution(std::vector<double> genotype, std::vector<int> phenotype, double fitness);
	Solution(RandomKeyEncoder& encoder, RandomKeyDecoder& decoder);
	double evaluate(Problem& problem);
	std::vector<double> getGenotype() const;
	std::vector<int> getPhenotype() const;
	std::vector<double>* getGenotypePtr();
	std::vector<int>* getPhenotypePtr();
	double getFitness() const;
	void setFitness(double newFitness);
	void setPhenotype(std::vector<int>& newPhenotype, RandomKeyEncoder& encoder);
	void setGenotype(std::vector<double>& newGenotype, RandomKeyDecoder& decoder);
	void recalculatePhenotype(RandomKeyDecoder& decoder);
	void recalculateGenotype(RandomKeyEncoder& encoder);
private:
	std::vector<double> genotype;
	std::vector<int> phenotype;
	double fitness;
};
