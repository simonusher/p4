#pragma once
#include "Pyramid.h"

class FeedbackPyramid: public Pyramid {
public:
	FeedbackPyramid(Problem* problem, SolutionFactory* solutionFactory, PopulationFactory* populationFactory,
		LocalOptimizer* localOptimizer, double feedbackProbability);

	void runSingleIteration() override;
private:
	void runFeedback();
	double feedbackProbability;
	std::mt19937 randomGenerator;
	std::bernoulli_distribution feedbackDistribution;
};