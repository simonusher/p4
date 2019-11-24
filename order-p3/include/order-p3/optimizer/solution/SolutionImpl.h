#pragma once
#include "Solution.h"
#include <vector>
#include "../encoding/RandomKeyEncoder.h"
#include "../encoding/RandomKeyDecoder.h"
#include "../../problem/Problem.h"

class SolutionImpl : public Solution {
public:
	Solution(std::vector<double> genotype, std::vector<int> phenotype, double fitness, RandomKeyEncoder& encoder, RandomKeyDecoder& decoder);
	Solution(RandomKeyEncoder& encoder, RandomKeyDecoder& decoder);
private:
	std::vector<double> genotype;
	std::vector<int> phenotype;
	double fitness;
	RandomKeyEncoder* encoder;
	RandomKeyDecoder* decoder;
};