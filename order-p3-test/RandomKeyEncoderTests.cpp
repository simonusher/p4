#include "pch.h"
#include "../order-p3/include/order-p3/RandomKeyEncoder.h"

TEST(RandomKeyGenerationTests, GivenCorrectBoundsEveryKeyShouldBeInRange) {
	double lowerBound = 0;
	double upperBound = 1;
	int numberOfGenes = 10;
	RandomKeyEncoder encoder(lowerBound, upperBound, numberOfGenes);
	std::vector<double> encoding(encoder.getRandomEncoding(numberOfGenes));

	for(size_t i = 0; i < numberOfGenes; i++) {
		EXPECT_TRUE(encoding[i] >= lowerBound && encoding[i] <= upperBound);
	}
}

TEST(RandomKeyGenerationTests, GivenCorrectNumberOfGenesResultShouldHaveCorrectLength) {
	double lowerBound = 0;
	double upperBound = 1;
	int numberOfGenes = 10;
	RandomKeyEncoder encoder(lowerBound, upperBound, numberOfGenes);
	std::vector<double> encoding(encoder.getRandomEncoding(numberOfGenes));

	EXPECT_EQ(numberOfGenes, encoding.size());
}