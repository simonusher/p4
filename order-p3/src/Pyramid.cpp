//
// Created by Szymon on 24.11.2018.
//

#include "../include/order-p3/Pyramid.h"
#include "../include/order-p3/PairHasher.h"

Pyramid::Pyramid(vector<int> &geneDomain, CEvaluator& evaluator) {
    this->geneDomain = geneDomain;
	this->evaluator = &evaluator;
    this->generator = std::mt19937(randomDevice());
    this->geneDistribution = std::uniform_int_distribution<>(0, geneDomain.size() - 1);
	this->numberOfGenes = evaluator.iGetNumberOfBits();
    this->bestFitness = -1;
}

Pyramid::~Pyramid() {
    for(int i = 0; i < populations.size(); i++){
        delete populations[i];
    }
}

int Pyramid::nextRandomGene() {
    return geneDomain[geneDistribution(generator)];
}

bool Pyramid::addUnique(vector<int> &solution, int level, double fitness) {
    seenSolution = seen.find(solution);
    if(seenSolution != seen.end()){
        return false;
    } else {
        seen.insert(solution);
        if(populations.size() == level){
			populations.push_back(new Population(numberOfGenes, geneDomain));
        }
        populations[level]->addSolution(solution, generator);
        if(fitness > bestFitness){
            bestSolution = solution;
            bestFitness = fitness;
        }
        return true;
    }
}

void Pyramid::runSingleIteration() {
    vector<int> newSolution = nextRandomSolution();
	double fitness = evaluator->dEvaluate(newSolution);
    hillClimb(newSolution, fitness);
    climb(newSolution, fitness);
}

vector<int> Pyramid::nextRandomSolution() {
    vector<int> solution(numberOfGenes, 0);
    for(int i = 0; i < numberOfGenes; i++){
        solution[i] = nextRandomGene();
    }
    return solution;
}

void Pyramid::hillClimb(vector<int> &solution, double &fitness) {
	vector<std::pair<int, int>> optionPairs;
	optionPairs.reserve(solution.size() * geneDomain.size());
	for(int i = 0; i < solution.size(); i++){
		for(int j = 0; j < geneDomain.size(); j++)
		{
			optionPairs.push_back({ i, geneDomain.at(j) });
		}
	}

	float new_fitness;
	bool improvement;
	std::unordered_set<std::pair<int, int>, PairHasher> tried;
	do {
		improvement = false;
		std::shuffle(optionPairs.begin(), optionPairs.end(), generator);
		for (std::pair<int, int>& optionPair : optionPairs) {
			if (tried.count(optionPair) == 0 && solution.at(optionPair.first) != optionPair.second) {
				std::swap(solution[optionPair.first], optionPair.second);
				new_fitness = evaluator->dEvaluate(solution);
				if (fitness < new_fitness) {
					optionPair.second = solution[optionPair.first];
					fitness = new_fitness;
					improvement = true;
					tried.clear();
				}
				else {
					std::swap(solution[optionPair.first], optionPair.second);
				}
				tried.insert(optionPair);
			}
		}
	} while (improvement);
}

bool Pyramid::climb(vector<int> &solution, double &fitness) {
    return climb(solution, fitness, 0);
}

bool Pyramid::climb(vector<int> &solution, double &fitness, int level) {
    bool added = addUnique(solution, level, fitness);
    if(added){
        for (int lev = level; lev < populations.size(); lev++) {
            double prev = fitness;
            populations[lev]->improve(solution, fitness, *evaluator, generator);
            if (prev < fitness) {
                added = addUnique(solution, lev + 1, fitness) || added;
            }
        }
    }
    return added;
}

const vector<int> &Pyramid::getBestSolution() const {
    return bestSolution;
}

double Pyramid::getBestFitness() const {
    return bestFitness;
}
