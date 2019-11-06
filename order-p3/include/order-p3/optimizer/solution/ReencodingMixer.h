#pragma once
#include "Solution.h"

template <class TMixer>
class ReencodingMixer : public TMixer {
public:
	template<typename ...Args>
	ReencodingMixer(Args ...args) : TMixer(args...) {
	}
	bool mix(Solution* destination, Solution* source, std::vector<int>* cluster) override;
};

template <class TMixer>
bool ReencodingMixer<TMixer>::mix(Solution* destination, Solution* source, std::vector<int>* cluster) {
	bool changed = TMixer::mix(destination, source, cluster);
	destination->reEncode();
	return changed;
}
