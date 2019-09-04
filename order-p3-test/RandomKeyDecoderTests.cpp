#include "pch.h"
#include "../order-p3/include/order-p3/RandomKeyDecoder.h"

TEST(DecodingTests, GivenCorrectVectorsPhenotypeShouldBeDecodedCorrectly) {
	RandomKeyDecoder decoder;
	std::vector<double> testGenotype { 0.57, 0.93, 0.12, 0.43 };
	std::vector<int> expectedPhenotype { 2, 3, 0, 1 };
	std::vector<int> decodingResult(decoder.decode(testGenotype));
	for(size_t i = 0; i < decodingResult.size(); i++) {
		EXPECT_EQ(decodingResult[i], expectedPhenotype[i]);
	}
}