#include "pch.h"
#include "../order-p3/include/order-p3/optimizer/encoding/RandomKeyEncoder.h"

class RandomKeyEncoderTests : public ::testing::Test {
protected:
	void SetUp() override {
		lowerBound = 0;
		upperBound = 1;
		numberOfGenes = 6;
		encoder = new RandomKeyEncoder(lowerBound, upperBound, numberOfGenes);
	}

	void TearDown() override {
		delete encoder;
	}
	
	double lowerBound;
	double upperBound;
	int numberOfGenes;
	RandomKeyEncoder* encoder;
};
TEST_F(RandomKeyEncoderTests, GivenCorrectBoundsEveryKeyShouldBeInRange) {
	std::vector<double> encoding(encoder->getRandomEncoding(numberOfGenes));

	for(size_t i = 0; i < numberOfGenes; i++) {
		EXPECT_TRUE(encoding[i] >= lowerBound && encoding[i] <= upperBound);
	}
}

TEST_F(RandomKeyEncoderTests, GivenCorrectNumberOfGenesResultShouldHaveCorrectLength) {
	std::vector<double> encoding(encoder->getRandomEncoding(numberOfGenes));

	EXPECT_EQ(numberOfGenes, encoding.size());
}

TEST_F(RandomKeyEncoderTests, GivenSamplePhenotypeGetRandomEncodingShouldYieldCorrectResults) {
	std::vector<int> testPhenotype{ 2, 1, 3, 5, 4, 0 };
	std::vector<double> phenotypeEncoding(encoder->getEncodingForPhenotype(testPhenotype));
	
	EXPECT_LE(phenotypeEncoding[2], phenotypeEncoding[0]);
	EXPECT_LE(phenotypeEncoding[2], phenotypeEncoding[1]);
	EXPECT_LE(phenotypeEncoding[2], phenotypeEncoding[3]);
	EXPECT_LE(phenotypeEncoding[2], phenotypeEncoding[4]);
	EXPECT_LE(phenotypeEncoding[2], phenotypeEncoding[5]);


	EXPECT_LE(phenotypeEncoding[1], phenotypeEncoding[0]);
	EXPECT_LE(phenotypeEncoding[1], phenotypeEncoding[3]);
	EXPECT_LE(phenotypeEncoding[1], phenotypeEncoding[4]);
	EXPECT_LE(phenotypeEncoding[1], phenotypeEncoding[5]);


	EXPECT_LE(phenotypeEncoding[3], phenotypeEncoding[0]);
	EXPECT_LE(phenotypeEncoding[3], phenotypeEncoding[4]);
	EXPECT_LE(phenotypeEncoding[3], phenotypeEncoding[5]);

	EXPECT_LE(phenotypeEncoding[5], phenotypeEncoding[0]);
	EXPECT_LE(phenotypeEncoding[5], phenotypeEncoding[4]);
	
	EXPECT_LE(phenotypeEncoding[4], phenotypeEncoding[0]);
}