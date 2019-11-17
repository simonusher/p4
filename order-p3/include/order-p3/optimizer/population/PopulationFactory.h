#pragma once
#include "Population.h"

class PopulationFactory {
public:
	virtual ~PopulationFactory() = default;
	virtual Population* newPopulation() = 0;
};
