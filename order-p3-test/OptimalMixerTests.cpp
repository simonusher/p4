#include "pch.h"
#include "../order-p3/include/order-p3/problem/Problem.h"
#include "../order-p3/include/order-p3/optimizer/solution/OptimalMixer.h"
#include "gmock/gmock.h"

class ProblemMock: public Problem {
public:
	MOCK_METHOD1(evaluate, double(std::vector<int>& solution));
	MOCK_METHOD0(getProblemSize, int());
	MOCK_METHOD0(getFitnessFunctionEvaluations, int());
};

class OptimalMixerTests : public ::testing::Test {
protected:
	void setUpSource() {
		sourceSolution = new Solution(SOURCE_GENOTYPE, SOURCE_PHENOTYPE, BASE_FITNESS, *encoder, *decoder);
	}

	void setUpDestination() {
		destinationSolution = new Solution(DESTINATION_GENOTYPE, DESTINATION_PHENOTYPE, BASE_FITNESS, *encoder, *decoder);
	}

	void SetUp() override {
		decoder = new RandomKeyDecoder();
		problem = new ProblemMock();
		encoder = new RandomKeyEncoder(0.0, 1.0, SOURCE_GENOTYPE.size());
		optimalMixer = new OptimalMixer(problem);
		setUpSource();
		setUpDestination();
	}

	void TearDown() override {
		delete optimalMixer;
		delete problem;
		delete decoder;
		delete destinationSolution;
		delete sourceSolution;
		delete encoder;
	}

	template<typename T>
	void expectVectorsEqual(std::vector<T>* first, std::vector<T>* second) {
		EXPECT_EQ(first->size(), second->size());
		for(int i = 0; i < first->size(); i++) {
			EXPECT_EQ(first->at(i), second->at(i));
		}
	}

	template<typename T>
	void expectVectorsEqualInCluster(std::vector<T>* first, std::vector<T>* second, std::vector<int>& cluster) {
		for(const int& index : cluster) {
			EXPECT_EQ(first->at(index), second->at(index));
		}
	}

	static const int BASE_FITNESS = 5;
	const std::vector<int> SOURCE_PHENOTYPE { 1, 0, 2, 3 };
	const std::vector<double> SOURCE_GENOTYPE { 0.3, 0.1, 0.4, 0.5 };

	const std::vector<int> DESTINATION_PHENOTYPE{ 0, 3, 2, 1 };
	const std::vector<double> DESTINATION_GENOTYPE{ 0.1, 0.5, 0.3, 0.2 };

	std::vector<int> cluster { 0, 3 };	
	
	Solution* destinationSolution;
	Solution* sourceSolution;
	ProblemMock* problem;
	RandomKeyDecoder* decoder;
	RandomKeyEncoder* encoder;
	OptimalMixer* optimalMixer;
};

TEST_F(OptimalMixerTests, GivenTwoSolutionsWhenMixingImprovesFitnessThenSourceShouldBeUnchanged) {
	using testing::Return;
	EXPECT_CALL(*problem, evaluate)
		.WillOnce(Return(BASE_FITNESS + 1));

	std::vector<double> oldGenotype(sourceSolution->getGenotype());
	std::vector<int> oldPhenotype(sourceSolution->getPhenotype());
	double oldFitness = sourceSolution->getFitness();
	optimalMixer->mix(destinationSolution, sourceSolution, &cluster);
	EXPECT_EQ(sourceSolution->getFitness(), oldFitness);
	expectVectorsEqual(&oldGenotype, sourceSolution->getGenotypePtr());
	expectVectorsEqual(&oldPhenotype, sourceSolution->getPhenotypePtr());
}

TEST_F(OptimalMixerTests, GivenTwoSolutionsWhenMixingImprovesFitnessThenDestinationShouldBeChanged) {
	using testing::Return;
	EXPECT_CALL(*problem, evaluate)
		.WillOnce(Return(BASE_FITNESS + 1));

	
	optimalMixer->mix(destinationSolution, sourceSolution, &cluster);
	expectVectorsEqualInCluster(destinationSolution->getGenotypePtr(), sourceSolution->getGenotypePtr(), cluster);
	EXPECT_EQ(destinationSolution->getFitness(), BASE_FITNESS + 1);
}

TEST_F(OptimalMixerTests, GivenTwoSolutionsWhenMixingDecreasesFitnessThenDestinationShouldBeUnchanged) {
	using testing::Return;
	EXPECT_CALL(*problem, evaluate)
		.WillOnce(Return(BASE_FITNESS - 1));

	std::vector<double> oldGenotype(destinationSolution->getGenotype());
	std::vector<int> oldPhenotype(destinationSolution->getPhenotype());
	double oldFitness = destinationSolution->getFitness();

	optimalMixer->mix(destinationSolution, sourceSolution, &cluster);
	expectVectorsEqual(&oldGenotype, destinationSolution->getGenotypePtr());
	expectVectorsEqual(&oldPhenotype, destinationSolution->getPhenotypePtr());
	EXPECT_EQ(oldFitness, destinationSolution->getFitness());
}

TEST_F(OptimalMixerTests, GivenTwoSolutionsWhenMixingDecreasesFitnessThenSourceShouldBeUnchanged) {
	using testing::Return;
	EXPECT_CALL(*problem, evaluate)
		.WillOnce(Return(BASE_FITNESS - 1));

	std::vector<double> oldGenotype(sourceSolution->getGenotype());
	std::vector<int> oldPhenotype(sourceSolution->getPhenotype());
	double oldFitness = sourceSolution->getFitness();
	optimalMixer->mix(destinationSolution, sourceSolution, &cluster);
	EXPECT_EQ(sourceSolution->getFitness(), oldFitness);
	expectVectorsEqual(&oldGenotype, sourceSolution->getGenotypePtr());
	expectVectorsEqual(&oldPhenotype, sourceSolution->getPhenotypePtr());
}