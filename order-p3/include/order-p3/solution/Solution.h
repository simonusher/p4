#pragma once
#include <vector>
template <typename TGenotype, typename TPhenotype, typename TFitness>
class Solution {
public:
	virtual ~Solution() = default;
	virtual std::vector<TGenotype>* getGenotype() = 0;
	virtual std::vector<TPhenotype>* getPhenotype() = 0;
	virtual TFitness getFitness() = 0;
};