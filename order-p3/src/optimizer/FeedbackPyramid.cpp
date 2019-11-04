#include "../../include/order-p3/optimizer/FeedbackPyramid.h"

FeedbackPyramid::FeedbackPyramid(Problem* problem, SolutionFactory* solutionFactory,
	PopulationFactory* populationFactory, LocalOptimizer* localOptimizer,
	double feedbackProbability) : Pyramid(problem, solutionFactory, populationFactory, localOptimizer),
	feedbackProbability(feedbackProbability), feedbackDistribution(feedbackProbability) {
}

void FeedbackPyramid::runSingleIteration() {
	Pyramid::runSingleIteration();
	if(feedbackDistribution(randomGenerator)) {
		runFeedback();
	}
}

void FeedbackPyramid::runFeedback() {
	tryToAddImprovedSolutions(bestSolution, 0);
}
