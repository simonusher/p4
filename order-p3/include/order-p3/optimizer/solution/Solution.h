#pragma once
#include <vector>
#include "../encoding/RandomKeyEncoder.h"
#include "../encoding/RandomKeyDecoder.h"
#include "../../problem/Problem.h"

class Solution
{
public:
	Solution(std::vector<double> genotype, std::vector<int> phenotype, double fitness, RandomKeyEncoder& encoder, RandomKeyDecoder& decoder);
	Solution(RandomKeyEncoder& encoder, RandomKeyDecoder& decoder);
	double evaluate(Problem& problem);
	std::vector<double> getGenotype() const;
	std::vector<int> getPhenotype() const;
	std::vector<double>* getGenotypePtr();
	std::vector<int>* getPhenotypePtr();
	double getFitness() const;
	void setFitness(double newFitness);
	void setGenotype(std::vector<double>& newGenotype);
	void setPhenotype(std::vector<int>& newPhenotype);
	void recalculatePhenotype();
	void recalculateGenotype();
	void reEncode();
private:
	std::vector<double> genotype;
	std::vector<int> phenotype;
	double fitness;
	RandomKeyEncoder* encoder;
	RandomKeyDecoder* decoder;
};
