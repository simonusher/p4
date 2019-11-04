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
#include <functional>
#include "../order-p3/include/order-p3/optimizer/encoding/MaskedDecoder.h"
#include "../order-p3/include/order-p3/problem/deceptive_ordering/RelativeOrderingProblem.h"
#include <filesystem>


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


void runTest(int problemIndex, Problem* problem, bool removeDuplicatesUpper, bool useLocalOptimizer, int experimentNumber,
	std::function<bool(Problem*, Pyramid*)> stopCondition)
{
	std::random_device device;
	unsigned int seed = device();
	std::mt19937 randomGenerator(seed);

	ofstream myfile;
	myfile.open("experiments/" + to_string(problemIndex) + "/p" + std::to_string(problemIndex) + "d" + std::to_string(removeDuplicatesUpper) + "plo" +
		std::to_string(useLocalOptimizer) + "_" + std::to_string(experimentNumber) + ".csv");
	myfile << "Seed: " << std::to_string(seed) << std::endl;
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
	if(useLocalOptimizer)
	{
		// localOptimizer = new Pyramid(problem, solutionFactory, populationFactory, &optimizer, false);
		localOptimizer = new SwapHillClimber(problem);

	} else
	{
		localOptimizer = new NullOptimizer(problem);
		// localOptimizer = new SwapHillClimber(problem);
	}

	Pyramid finalPyramid(problem, solutionFactory, populationFactory, localOptimizer, removeDuplicatesUpper);

	
	while (!stopCondition(problem, &finalPyramid)) {
		finalPyramid.runSingleIteration();
		if(best_fitness < finalPyramid.getBestFitness())
		{
			best_fitness = finalPyramid.getBestFitness();
			std::cout << ffeFound << " " << best_fitness << std::endl;
			ffeFound = problem->getFitnessFunctionEvaluations();
			myfile << ffeFound << ";" << best_fitness << std::endl;
		} 
	}
	delete localOptimizer;
}


int main() {
	int numberOfExperiments = 20;
	int budget = 220712150;
	std::function<bool(Problem*, Pyramid*)> stop_condition = [&](Problem* problem, Pyramid* pyramid) { return problem->getFitnessFunctionEvaluations() >= budget;  };

	for(int i = 31; i < 41; i++)
	{
		if(!std::filesystem::exists("experiments/" + to_string(i))) {
			std::filesystem::create_directory("experiments/" + to_string(i));
		}
		for(int j = 0; j < numberOfExperiments; j++)
		{
			// std::cout << j << std::endl;
			FlowshopSchedulingProblem p;
			p.initializeProblem(i);
			runTest(i, &p, false, false, j, stop_condition);
			
			p = FlowshopSchedulingProblem();
			p.initializeProblem(i);
			runTest(i, &p, false, true, j, stop_condition);
		}
	}
	return 0;
}
