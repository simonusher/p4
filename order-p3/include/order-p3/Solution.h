#pragma once
#include <vector>
#include "RandomKeyEncoder.h"
#include "RandomKeyDecoder.h"
#include "evaluation/Evaluator.h"

class Solution
{
public:
	void decodePhenotype(RandomKeyDecoder& decoder);
	Solution(RandomKeyEncoder& encoder, RandomKeyDecoder& decoder);
	std::vector<double>& getGenotype();
	std::vector<int>& getPhenotype();
	double evaluate(CEvaluator& evaluator);
	double getFitness() const;
private:
	std::vector<double> genotype;
	std::vector<int> phenotype;
	double fitness;
};
