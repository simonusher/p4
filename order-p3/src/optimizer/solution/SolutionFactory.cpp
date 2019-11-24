#include "../../../include/order-p3/optimizer/solution/SolutionFactory.h"

SolutionFactory::SolutionFactory(RandomKeyEncoder& encoder, RandomKeyDecoder& decoder) : encoder(encoder), decoder(decoder) { }

Solution* SolutionFactory::nextSolution() {
	return new Solution(encoder, decoder);
}
