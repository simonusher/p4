#include "../../../include/order-p3/optimizer/solution/SolutionFactoryImpl.h"

SolutionFactoryImpl::SolutionFactoryImpl(RandomKeyEncoder& encoder, RandomKeyDecoder& decoder) : encoder(encoder), decoder(decoder) { }

Solution* SolutionFactoryImpl::nextSolution() {
	return new Solution(encoder, decoder);
}
