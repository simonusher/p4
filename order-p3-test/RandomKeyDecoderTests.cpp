#include "pch.h"
#include "../order-p3/include/order-p3/optimizer/encoding/RandomKeyDecoder.h"

TEST(DecodingTests, GivenCorrectVectorsPhenotypeShouldBeDecodedCorrectly) {
	RandomKeyDecoder decoder;
	std::vector<double> testGenotype { 0.57, 0.93, 0.12, 0.43 };
	std::vector<int> expectedPhenotype { 2, 3, 0, 1 };
	std::vector<int> decodingResult(decoder.decode(testGenotype));
	for(size_t i = 0; i < decodingResult.size(); i++) {
		EXPECT_EQ(decodingResult[i], expectedPhenotype[i]);
	}
}

TEST(DecodingTests, RkgaTestVectorsShouldBeDecodedCorrectly) {
	RandomKeyDecoder decoder;
	std::vector<double> testGenotype{ 0.46, 0.91, 0.33, 0.75, 0.51 };
	std::vector<int> expectedPhenotype{ 2, 0, 4, 3, 1 };
	std::vector<int> decodingResult(decoder.decode(testGenotype));
	for (size_t i = 0; i < decodingResult.size(); i++) {
		EXPECT_EQ(decodingResult[i], expectedPhenotype[i]);
	}
}