#include <utility>
#include "../../../include/order-p3/optimizer/encoding/MaskedDecoder.h"

MaskedDecoder::MaskedDecoder(std::mt19937& randomGenerator, int numberOfGenes) : orderingMask(numberOfGenes) {
	std::iota(orderingMask.begin(), orderingMask.end(), 0);
	std::shuffle(orderingMask.begin(), orderingMask.end(), randomGenerator);
}

MaskedDecoder::MaskedDecoder(std::vector<int> orderingMask) : orderingMask(std::move(orderingMask)) {
}

MaskedDecoder MaskedDecoder::get8FunctionDeflen6Coding() {
	return { { 0, 2, 4, 6, 1, 3, 5, 7, 8, 10, 12, 14, 9, 11, 13, 15, 16, 18, 20, 22, 17, 19, 21, 23, 24, 26, 28, 30, 25, 27, 29, 31 } };
}

MaskedDecoder MaskedDecoder::get8FunctionLooseCoding() {
	return { { 0, 8, 16, 24, 1, 9, 17, 25, 2, 10, 18, 26, 3, 11, 19, 27, 4, 12, 20, 28, 4, 13, 21, 29, 6, 14, 22, 30, 7, 15, 23, 31 } };
}

std::vector<int> MaskedDecoder::decode(const std::vector<double>& encodedGenotype) {
	std::vector<double> genotypeCopy(encodedGenotype.size());
	for (int i = 0; i < encodedGenotype.size(); i++) {
		genotypeCopy[i] = encodedGenotype[orderingMask[i]];
	}
	return RandomKeyDecoder::decode(genotypeCopy);
}
