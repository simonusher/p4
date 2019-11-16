#include "pch.h"
#include "../order-p3/include/order-p3/optimizer/encoding/RandomKeyDecoder.h"
#include "../order-p3/include/order-p3/optimizer/encoding/RandomKeyEncoder.h"

TEST(RandomKeyIntegrationTests, GivenPhenotypeWhenFirstEncodedThenDecodedItShouldYieldSamePhenotype) {
	std::random_device d;
	std::mt19937 randomGenerator(d());
	std::vector<int> testPhenotype{ 2, 0, 4, 3, 1 };
	RandomKeyDecoder decoder;
	RandomKeyEncoder encoder(0, 1, testPhenotype.size(), randomGenerator);
	std::vector<double> encoding(encoder.getEncodingForPhenotype(testPhenotype));
	std::vector<int> decoded(decoder.decode(encoding));
	for (int i = 0; i < testPhenotype.size(); i++) {
		EXPECT_EQ(testPhenotype[i], decoded[i]);
	}
}