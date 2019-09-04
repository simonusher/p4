#pragma once
#include <vector>
#include "RandomKeyEncoder.h"
#include "RandomKeyDecoder.h"

class Solution
{
public:
	void decodePhenotype(RandomKeyDecoder& decoder);
	Solution(RandomKeyEncoder& encoder, RandomKeyDecoder& decoder);
	std::vector<double>& getGenotype();
	std::vector<int>& getPhenotype();
private:
	std::vector<double> genotype;
	std::vector<int> phenotype;
};
