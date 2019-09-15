#pragma once
#include <vector>
#include "../encoding/RandomKeyEncoder.h"
#include "../encoding/RandomKeyDecoder.h"
#include "../../problem/Problem.h"

class Solution
{
public:
	Solution(RandomKeyEncoder& encoder, RandomKeyDecoder& decoder);
	double evaluate(Problem& problem);
	std::vector<double> getGenotype() const;
	std::vector<int> getPhenotype() const;
	double getFitness() const;
	void setPhenotype(std::vector<int>& newPhenotype, RandomKeyEncoder& encoder);
	void setGenotype(std::vector<double>& newGenotype, RandomKeyDecoder& decoder);;
private:
	void decodeGenotypeToPhenotype(RandomKeyDecoder& decoder);
	void encodePhenotypeToGenotype(RandomKeyEncoder& encoder);
	std::vector<double> genotype;
	std::vector<int> phenotype;
	double fitness;
};