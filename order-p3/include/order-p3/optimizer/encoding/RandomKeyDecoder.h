#pragma once
#include <vector>
#include <algorithm>
#include <numeric>

class RandomKeyDecoder
{
public:
	std::vector<int> decode(const std::vector<double>& encodedGenotype);
};

