#pragma once
#include <vector>
#include "RandomKeyEncoder.h"
#include "RandomKeyDecoder.h"
#include "evaluation/Evaluator.h"

class Solution
{
public:
	Solution(RandomKeyEncoder& encoder, RandomKeyDecoder& decoder);
	double evaluate(CEvaluator& evaluator);
	std::vector<double>& getGenotype();
	std::vector<int>& getPhenotype();
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
