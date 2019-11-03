#include <utility>
#include "../../../include/order-p3/optimizer/encoding/MaskedDecoder.h"

MaskedDecoder::MaskedDecoder(std::mt19937& randomGenerator, int numberOfGenes) : orderingMask(numberOfGenes) {
	std::iota(orderingMask.begin(), orderingMask.end(), 0);
	std::shuffle(orderingMask.begin(), orderingMask.end(), randomGenerator);
}

MaskedDecoder::MaskedDecoder(std::vector<int> orderingMask) : orderingMask(std::move(orderingMask)) {
}

std::vector<int> MaskedDecoder::decode(const std::vector<double>& encodedGenotype) {
	std::vector<double> genotypeCopy(encodedGenotype.size());
	for (int i = 0; i < encodedGenotype.size(); i++) {
		genotypeCopy[i] = encodedGenotype[orderingMask[i]];
	}
	return RandomKeyDecoder::decode(genotypeCopy);
}
