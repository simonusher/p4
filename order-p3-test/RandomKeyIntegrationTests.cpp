#include "pch.h"
#include "../order-p3/include/order-p3/optimizer/encoding/RandomKeyDecoder.h"
#include "../order-p3/include/order-p3/optimizer/encoding/RandomKeyEncoder.h"

TEST(RandomKeyIntegrationTests, GivenPhenotypeWhenFirstEncodedThenDecodedItShouldYieldSamePhenotype) {
	std::vector<int> testPhenotype{ 2, 0, 4, 3, 1 };
	RandomKeyDecoder decoder;
	RandomKeyEncoder encoder(0, 1, testPhenotype.size());
	std::vector<double> encoding(encoder.getEncodingForPhenotype(testPhenotype));
	std::vector<int> decoded(decoder.decode(encoding));
	for (int i = 0; i < testPhenotype.size(); i++) {
		EXPECT_EQ(testPhenotype[i], decoded[i]);
	}
}