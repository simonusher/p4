#include "../../include/order-p3/optimizer/PopulationFactoryImpl.h"


PopulationFactoryImpl::PopulationFactoryImpl(Problem* problem, std::mt19937& randomGenerator) : problem(problem), randomGenerator(randomGenerator) { }

Population* PopulationFactoryImpl::getNewPopulation() const {
	return new Population(problem, randomGenerator);
}
