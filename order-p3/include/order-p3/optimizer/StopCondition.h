#pragma once
#include <chrono>
#include <functional>
#include <utility>

class StopCondition {
public:
	virtual bool finished() = 0;
	virtual ~StopCondition() = default;
	bool operator()() {
		return finished();
	}
};

class TimePassedStopCondition : public StopCondition {
public:
	explicit TimePassedStopCondition(const std::chrono::steady_clock::time_point& endTime)
		: endTime(endTime) {
	}

	bool finished() override {
		return std::chrono::steady_clock::now() > endTime;
	}
private:
	std::chrono::steady_clock::time_point endTime;
};

class FfeStopCondition : public StopCondition {
public:
	FfeStopCondition(std::function<int()> getElapsedFfe, int ffeThreshold)
		: getElapsedFfe(std::move(getElapsedFfe)),
		  ffeThreshold(ffeThreshold) {
	}


	bool finished() override {
		return getElapsedFfe() > ffeThreshold;
	}
private:
	std::function<int()> getElapsedFfe;
	int ffeThreshold;
};