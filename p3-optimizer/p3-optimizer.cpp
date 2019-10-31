#pragma once
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
#include "../order-p3/include/order-p3/problem/PermutationGOMEA.h"
#include "../order-p3/include/order-p3/optimizer/solution/RandomRescalingOptimalMixer.h"
#include "../order-p3/include/order-p3/problem/SortFunctionProblem.h"
#include "../order-p3/include/order-p3/local_optimizers/OptimalInversionHillClimber.h"

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

int main() {
	TtpProblem* problem = new TtpProblem();
	problem->initialize("medium_0.ttp", ItemSelectionPolicy::ProfitWeightRatio);
	// problem->initialize("hard_0.ttp", ItemSelectionPolicy::ProfitWeightRatio);
	// PermutationGOMEAProblems* problem = new PermutationGOMEAProblems();
	// problem->initializeProblem(37);
	// AbsoluteOrderingProblem* problem = new AbsoluteOrderingProblem(8);
	std::random_device device;
	std::mt19937 randomGenerator(device());

	NullOptimizer optimizer(problem);
	RandomKeyEncoder encoder(0, 1, problem->getProblemSize());
	RandomKeyDecoder decoder;
	SolutionFactory* solutionFactory = new SolutionFactoryImpl(encoder, decoder);
	SolutionMixer* mixer = new RandomRescalingOptimalMixer(problem, 0.1, 0, 1, randomGenerator);
	PopulationFactory* factory = new PopulationFactoryImpl(problem, mixer, randomGenerator);
	Pyramid pyramid(problem, solutionFactory, factory, &optimizer , false);

	double max_iter = 220712150;
	// for (int i = 0; problem->getFitnessFunctionEvaluations() < max_iter;i++) {
	for (int i = 0; pyramid.getBestFitness() != 1;i++) {
		pyramid.runSingleIteration();
		// std::cout << problem->getFitnessFunctionEvaluations() / max_iter << " : " << pyramid.getBestFitness() << std::endl;
		std::cout << problem->getFitnessFunctionEvaluations() << " : " << pyramid.getBestFitness() << std::endl;
	}
	
	// TtpProblem* problem = new TtpProblem();
	// problem->initialize("medium_0.ttp", ItemSelectionPolicy::ProfitWeightRatio);
	// problem->initialize("hard_0.ttp", ItemSelectionPolicy::ProfitWeightRatio);
	// AbsoluteOrderingProblem* problem = new AbsoluteOrderingProblem(8);
	// for(int j = 31; j < 41; j++) {
	// 	// PermutationGOMEAProblems* problem = new PermutationGOMEAProblems();
	// 	// problem->initializeProblem(32);
	// 	// Problem* problem = new SortFunctionProblem(10);
	// 	RandomKeyEncoder encoder(0, 1, problem->getProblemSize());
	// 	RandomKeyDecoder decoder;
	// 	SolutionFactory* solutionFactory = new SolutionFactoryImpl(encoder, decoder);
	//
	// 	// LocalOptimizer* localOptimizer = new SwapHillClimber(problem);
	// 	LocalOptimizer* localOptimizer = new NullOptimizer(problem);
	// 	// HcOptimizer* optimizer = new HcOptimizer(problem, solutionFactory, localOptimizer);
	// 	// LocalOptimizer* localOptimizer = new OptimalInversionHillClimber(problem);
	// 	std::random_device randomDevice;
	// 	std::mt19937 randomGenerator(randomDevice());
	// 	// SolutionMixer* mixer = new OptimalMixer(problem);
	// 	RandomRescalingOptimalMixer* mixer = new RandomRescalingOptimalMixer(problem, 0.1, 0, 1, randomGenerator);
	// 	PopulationFactory* populationFactory = new PopulationFactoryImpl(problem, mixer, randomGenerator);
	// 	Pyramid* pyramid = new Pyramid(problem, solutionFactory, populationFactory, localOptimizer);
	// 	// Pyramid* pyramid = new FeedbackPyramid(problem, solutionFactory, populationFactory, localOptimizer, 200);
	//
	// 	double max_iter = 220712150;
	// 	// for (int i = 0; problem->getFitnessFunctionEvaluations() < max_iter;i++) {
	// 	for (int i = 0; pyramid->getBestFitness() != 1 < max_iter;i++) {
	// 		// for(int i = 0; pyramid->getBestFitness() != 1 ;i++) {
	// 		pyramid->runSingleIteration();
	// 		// std::cout << problem->getFitnessFunctionEvaluations() / max_iter << " : " << pyramid->getBestFitness() << std::endl;
	// 		std::cout << problem->getFitnessFunctionEvaluations() << " : " << pyramid->getBestFitness() << std::endl;
	// 		// printSolution(pyramid->getBestSolutionPhenotype());
	// 		// std::cout << std::endl;
	// 	}
	// 	std::cout << "problem: " << j << " best fitness: " << pyramid->getBestFitness();;
	// 	delete pyramid;
	// 	delete populationFactory;
	// 	delete localOptimizer;
	// 	delete problem;
	// 	delete solutionFactory;
	// }
	
	return 0;
}
