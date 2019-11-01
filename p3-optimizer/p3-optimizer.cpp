#include "../order-p3/include/order-p3/optimizer/encoding/RandomKeyEncoder.h"
#include "../order-p3/include/order-p3/optimizer/encoding/RandomKeyDecoder.h"
#include "../order-p3/include/order-p3/optimizer/solution/SolutionFactory.h"
#include "../order-p3/include/order-p3/optimizer/solution/SolutionFactoryImpl.h"
#include "../order-p3/include/order-p3/problem/travelling_thief/TTPProblem.h"
#include "../order-p3/include/order-p3/local_optimizers/LocalOptimizer.h"
#include "../order-p3/include/order-p3/local_optimizers/SwapHillClimber.h"
#include "../order-p3/include/order-p3/optimizer/PopulationFactory.h"
#include "../order-p3/include/order-p3/optimizer/PopulationFactoryImpl.h"
#include "../order-p3/include/order-p3/optimizer/Pyramid.h"
#include "../order-p3/include/order-p3/optimizer/solution/OptimalMixer.h"
#include "../order-p3/include/order-p3/problem/deceptive_ordering/AbsoluteOrderingProblem.h"
#include "../order-p3/include/order-p3/optimizer/FeedbackPyramid.h"
#include "../order-p3/include/order-p3/optimizer/HcOptimizer.h"
#include "../order-p3/include/order-p3/local_optimizers/NullOptimizer.h"
#include "../order-p3/include/order-p3/optimizer/solution/RandomRescalingOptimalMixer.h"
#include "../order-p3/include/order-p3/problem/SortFunctionProblem.h"
#include "../order-p3/include/order-p3/local_optimizers/OptimalInversionHillClimber.h"
#include "../order-p3/include/order-p3/problem/FlowshopSchedulingProblem.h"

void printSolution(const std::vector<int>& solution) {
	std::cout << "[ ";
	for (int i = 0; i < solution.size(); i++) {
		std::cout << solution[i];
		if(i != solution.size() - 1) {
			std::cout << ", ";
		}
	}
	std::cout << " ]";
}


void runFlowshopTests(int problemIndex, bool removeDuplicatesUpper, int maxIter, bool useP3AsLocalOptimizer, int experimentNumber)
{
	FlowshopSchedulingProblem fsp;
	fsp.initializeProblem(problemIndex);
	Problem* problem = &fsp;
	std::random_device device;
	unsigned int seed = device();
	std::mt19937 randomGenerator(seed);

	ofstream myfile;
	myfile.open("p" + std::to_string(problemIndex) + "d" + std::to_string(removeDuplicatesUpper) + "p3" + 
		std::to_string(useP3AsLocalOptimizer) + "_" + std::to_string(experimentNumber) + ".csv");
	myfile << "FFE found;Fitness" << std::endl;

	double best_fitness = std::numeric_limits<double>::lowest();
	int ffeFound = 0;

	NullOptimizer optimizer(problem);
	RandomKeyEncoder encoder(0, 1, problem->getProblemSize());
	RandomKeyDecoder decoder;
	SolutionFactoryImpl factoryImpl(encoder, decoder);
	SolutionFactory* solutionFactory = &factoryImpl;
	RandomRescalingOptimalMixer mixerImpl(problem, 0.1, 0, 1, randomGenerator);
	SolutionMixer* mixer = &mixerImpl;
	PopulationFactoryImpl popFactoryImpl(problem, mixer, randomGenerator);
	PopulationFactory* populationFactory = &popFactoryImpl;

	LocalOptimizer* localOptimizer;
	if(useP3AsLocalOptimizer)
	{
		localOptimizer = new Pyramid(problem, solutionFactory, populationFactory, &optimizer, false);

	} else
	{
		localOptimizer = &optimizer;
	}

	Pyramid finalPyramid(problem, solutionFactory, populationFactory, localOptimizer, removeDuplicatesUpper);

	
	for (int i = 0; problem->getFitnessFunctionEvaluations() < maxIter; i++) {
		finalPyramid.runSingleIteration();
		if(problem->getFitnessFunctionEvaluations() <= maxIter && best_fitness < finalPyramid.getBestFitness())
		{
			best_fitness = finalPyramid.getBestFitness();
			ffeFound = problem->getFitnessFunctionEvaluations();
			myfile << ffeFound << ";" << best_fitness << std::endl;
		}
	}
}


int main() {
	int numberOfExperiments = 20;
	int budget = 220712150;
	for(int i = 31; i < 41; i++)
	{
		for(int j = 0; j < numberOfExperiments; j++)
		{
			runFlowshopTests(i, false, budget, true, j);
			runFlowshopTests(i, false, budget, false, j);
		}
	}
	return 0;
}
