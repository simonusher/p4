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
#include "../order-p3/include/order-p3/local_optimizers/NullOptimizer.h"
#include "../order-p3/include/order-p3/optimizer/solution/RandomRescalingOptimalMixer.h"
#include "../order-p3/include/order-p3/problem/SortFunctionProblem.h"
#include "../order-p3/include/order-p3/local_optimizers/OptimalInversionHillClimber.h"
#include "../order-p3/include/order-p3/problem/FlowshopSchedulingProblem.h"
#include <functional>
#include "../order-p3/include/order-p3/optimizer/encoding/MaskedDecoder.h"
#include "../order-p3/include/order-p3/problem/deceptive_ordering/RelativeOrderingProblem.h"
#include <filesystem>
#include "../order-p3/include/order-p3/optimizer/solution/ReencodingMixer.h"
#include "../order-p3/include/order-p3/optimizer/encoding/MaskedEncoder.h"


template <typename T>
void printSolution(const std::vector<T>& solution) {
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

	std::ofstream myfile;
	myfile.open("experiments/" + std::to_string(problemIndex) + "/p" + std::to_string(problemIndex) + "d" + std::to_string(removeDuplicatesUpper) + "plo" +
		std::to_string(useLocalOptimizer) + "_" + std::to_string(experimentNumber) + ".csv");
	myfile << "Seed: " << std::to_string(seed) << std::endl;
	myfile << "FFE found;Fitness" << std::endl;

	double best_fitness = std::numeric_limits<double>::lowest();
	int ffeFound = 0;

	NullOptimizer optimizer(problem);
	RandomKeyEncoder encoder(0, 1, problem->getProblemSize(), randomGenerator);
	RandomKeyDecoder decoder;
	SolutionFactoryImpl factoryImpl(encoder, decoder);
	// ReencodingMixer<RandomRescalingOptimalMixer> mixerImpl(problem, 0.1, 0, 1, randomGenerator);
	RandomRescalingOptimalMixer mixerImpl(problem, 0.1, 0, 1, randomGenerator);
	// OptimalMixer mixerImpl(problem);
	PopulationFactoryImpl popFactoryImpl(problem, &mixerImpl, randomGenerator);

	LocalOptimizer* localOptimizer;
	if (useLocalOptimizer)
	{
		localOptimizer = new SwapHillClimber(problem);
		// localOptimizer = new Pyramid(problem, &factoryImpl, &popFactoryImpl, &optimizer, removeDuplicatesUpper);
	}
	else {
		localOptimizer = new NullOptimizer(problem);
		// localOptimizer = new SwapHillClimber(problem);
	}

	Pyramid finalPyramid(problem, &factoryImpl, &popFactoryImpl, localOptimizer, removeDuplicatesUpper);

	
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

void run_tests() {
	int numberOfExperiments = 20;
	int budget = 220712150;
	// int budget = 283040000;
	std::function<bool(Problem*, Pyramid*)> stop_condition = [&](Problem* problem, Pyramid* pyramid) { return problem->getFitnessFunctionEvaluations() >= budget;  };

	for (int i = 31; i < 41; i++)
	{
		if (!std::filesystem::exists("experiments/" + std::to_string(i))) {
			std::filesystem::create_directory("experiments/" + std::to_string(i));
		}
		for (int j = 0; j < numberOfExperiments; j++)
		{
			std::cout << j << std::endl;
			FlowshopSchedulingProblem p;
			p.initializeProblem(i);
			runTest(i, &p, false, false, j, stop_condition);
		}
	}
}

void test2() {
	std::random_device d;
	std::mt19937 randomGenerator(d());
	// TtpProblem problem;
	// problem.initialize("medium_0.ttp", ItemSelectionPolicy::ProfitWeightRatio);
	AbsoluteOrderingProblem problem(8);
	// AbsoluteOrderingProblem problem(8);
	// RandomKeyEncoder encoder(0, 1, problem.getProblemSize(), randomGenerator);
	// MaskedEncoder encoder = MaskedEncoder::get8FunctionLooseCoding(0, 1, problem.getProblemSize(), randomGenerator);
	MaskedEncoder encoder = MaskedEncoder::get8FunctionDeflen6Coding(0, 1, problem.getProblemSize(), randomGenerator);
	// RandomKeyDecoder decoder;
	MaskedDecoder decoder = MaskedDecoder::get8FunctionDeflen6Coding();
	// NullOptimizer optimizer(&problem);
	SwapHillClimber optimizer(&problem);
	// OptimalMixer mixer(&problem);
	RandomRescalingOptimalMixer mixer(&problem, 0.1, 0, 1, randomGenerator);
	// ReencodingMixer<RandomRescalingOptimalMixer> mixer(&problem, 0.1, 0, 1, randomGenerator);
	SolutionFactoryImpl solutionFactory(encoder, decoder);
	PopulationFactoryImpl populationFactory(&problem, &mixer, randomGenerator);

	Pyramid pyramid(&problem, &solutionFactory, &populationFactory, &optimizer, false);

	double best_fitness = std::numeric_limits<double>::lowest();
	int ffeFound = 0;

	for (int i = 0; pyramid.getBestFitness() != 1; i++) {
		pyramid.runSingleIteration();
		if (best_fitness < pyramid.getBestFitness())
		{
			best_fitness = pyramid.getBestFitness();
			ffeFound = problem.getFitnessFunctionEvaluations();
			std::cout << ffeFound << " " << best_fitness << std::endl;
		}
	}
	printSolution(pyramid.getBestSolution()->getPhenotype());
}


int main() {
	run_tests();
	return 0;
}
