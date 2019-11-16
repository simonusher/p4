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
#include "../order-p3/include/order-p3/optimizer/PyramidSizingP3.h"


template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
	os << "[ ";
	for (int i = 0; i < vec.size(); i++) {
		os << vec[i];
		if(i != vec.size() - 1) {
			os << ", ";
		}
	}
	os << " ],\n";
	return os;
}


void runTest(int problemIndex, Problem* problem, bool removeDuplicatesUpper, bool useLocalOptimizer, int experimentNumber,
	std::function<bool(Problem*, Pyramid*)> stopCondition, bool usePreprocessedLinkage)
{
	std::random_device device;
	unsigned int seed = device();
	std::mt19937 randomGenerator(seed);

	std::ofstream myfile;
	myfile.open("experiments/" + std::to_string(problemIndex) + "/p" + std::to_string(problemIndex) + "d" + std::to_string(removeDuplicatesUpper) + "plo" +
		std::to_string(useLocalOptimizer) + "linkage" + std::to_string(usePreprocessedLinkage) + "_" + std::to_string(experimentNumber) + ".csv");
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

	Pyramid finalPyramid(problem, &factoryImpl, &popFactoryImpl, localOptimizer, removeDuplicatesUpper, usePreprocessedLinkage);

	int i = 0;
	while (!stopCondition(problem, &finalPyramid)) {
		i++;
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

void runSizingTest(Problem* problem, std::function<bool(Problem*, PyramidSizingP3*)> stopCondition) {
	double best_fitness = std::numeric_limits<double>::lowest();
	int ffeFound = 0;
	PyramidSizingP3	finalPyramid(problem);
	int i = 0;
	while (!stopCondition(problem, &finalPyramid)) {
		i++;
		finalPyramid.runSingleIteration();
		if (best_fitness < finalPyramid.getBestFitness())
		{
			best_fitness = finalPyramid.getBestFitness();
			ffeFound = problem->getFitnessFunctionEvaluations();
			std::cout << ffeFound << " " << best_fitness << std::endl;
		}
	}
}

void run_tests() {
	int numberOfExperiments = 10;
	int budget = 220712150;
	// int budget = 283040000;
	// int budget = 260316750;
	// int budget = 256208100;
	// std::vector<int> valuesToReach{ -14033, -15151, -13301, -15447, -13529, -13123, -13548, -13948, -14295, -12943, -20911, -22440, -19833, -18710, -18641, -19245, -18363, -20241, -20330, -21320, -33623, -31587, -33920, -31661, -34557, -32564, -32922, -32412, -33600, -32262 };
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
			// runTest(i, &p, false, false, j, stop_condition, false);
			runTest(i, &p, false, false, j, stop_condition, false);
		}
	}
}

void test2() {
	std::random_device d;
	std::mt19937 randomGenerator(d());
	TtpProblem problem;
	problem.initialize("hard_0.ttp", ItemSelectionPolicy::ProfitWeightRatio);
	// // AbsoluteOrderingProblem problem(8);
	// // RelativeOrderingProblem problem(8);
	// RandomKeyEncoder encoder(0, 1, problem.getProblemSize(), randomGenerator);
	// // MaskedEncoder encoder = MaskedEncoder::get8FunctionLooseCoding(0, 1, problem.getProblemSize(), randomGenerator);
	// // MaskedEncoder encoder = MaskedEncoder::get8FunctionLooseCoding(0, 1, problem.getProblemSize(), randomGenerator);
	// RandomKeyDecoder decoder;
	// // MaskedDecoder decoder = MaskedDecoder::get8FunctionLooseCoding();
	// NullOptimizer optimizer(&problem);
	// // SwapHillClimber optimizer(&problem);
	// // OptimalMixer mixer(&problem);
	// RandomRescalingOptimalMixer mixer(&problem, 0.1, 0, 1, randomGenerator);
	// // ReencodingMixer<RandomRescalingOptimalMixer> mixer(&problem, 0.1, 0, 1, randomGenerator);
	// SolutionFactoryImpl solutionFactory(encoder, decoder);
	// PopulationFactoryImpl populationFactory(&problem, &mixer, randomGenerator);
	// Pyramid pyramid(&problem, &solutionFactory, &populationFactory, &optimizer, false, false);

	double best_fitness = std::numeric_limits<double>::lowest();
	int ffeFound = 0;

	PyramidSizingP3 pyramid(&problem);

	for (int i = 0; pyramid.getBestFitness() != 1; i++) {
		pyramid.runSingleIteration();
		if (best_fitness < pyramid.getBestFitness())
		{
			best_fitness = pyramid.getBestFitness();
			ffeFound = problem.getFitnessFunctionEvaluations();
			std::cout << ffeFound << " " << best_fitness << std::endl;
		}
	}
	std::cout << pyramid.getBestSolution()->getPhenotype();
}

auto test3() {
	int budget = 220712150;
	std::function<bool(Problem*, PyramidSizingP3*)> stop_condition = [&](Problem* problem, PyramidSizingP3* pyramid) { return problem->getFitnessFunctionEvaluations() >= budget;  };
	FlowshopSchedulingProblem p;
	p.initializeProblem(31);
	runSizingTest(&p, stop_condition);
}


int main() {
}
