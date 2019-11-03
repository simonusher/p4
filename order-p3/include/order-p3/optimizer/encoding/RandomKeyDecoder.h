#pragma once
#include <vector>
#include <algorithm>
#include <numeric>

class RandomKeyDecoder
{
public:
	virtual ~RandomKeyDecoder() = default;
	virtual std::vector<int> decode(const std::vector<double>& encodedGenotype);
};

