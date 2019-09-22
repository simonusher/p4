#pragma once
#include "Pyramid.h"

class FeedbackPyramid: public Pyramid {
public:
	FeedbackPyramid(Problem* problem, SolutionFactory* solutionFactory, PopulationFactory* populationFactory,
		LocalOptimizer* localOptimizer, int feedbackFrequency);

	void runSingleIteration() override;
private:
	void runFeedback();
	int iterationsPassed;
	int feedbackFrequency;
};