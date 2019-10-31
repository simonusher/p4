#include "../../include/order-p3/optimizer/FeedbackPyramid.h"

FeedbackPyramid::FeedbackPyramid(Problem* problem, SolutionFactory* solutionFactory,
	PopulationFactory* populationFactory, LocalOptimizer* localOptimizer,
	int feedbackFrequency) : Pyramid(problem, solutionFactory, populationFactory, localOptimizer),
	feedbackFrequency(feedbackFrequency) {
	iterationsPassed = 0;
}

void FeedbackPyramid::runOptimizationFeedback() {
	std::cout << "RUNNING OPTIMIZATION FEEDBACK" << std::endl;
	Solution* solution = new Solution(*bestSolution);
	localOptimizer->optimizeLocally(solution);
	tryAddSolutionToPyramid(solution);
	std::cout << "bEST AFTER FEEDBACK: " << bestSolution->getFitness() << std::endl;
}

void FeedbackPyramid::runSingleIteration() {
	Pyramid::runSingleIteration();
	iterationsPassed++;
	if(iterationsPassed % feedbackFrequency == 0) {
		runFeedback();
		// runOptimizationFeedback();
	}
}

void FeedbackPyramid::runFeedback() {
	std::cout << "RUNNING FEEDBACK" << std::endl;
	tryToAddImprovedSolutions(bestSolution, 0);
	std::cout << "bEST AFTER FEEDBACK: " << bestSolution->getFitness() << std::endl;
}
