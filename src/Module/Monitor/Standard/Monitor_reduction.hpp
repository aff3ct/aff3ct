#ifndef MONITOR_REDUCTION_HPP_
#define MONITOR_REDUCTION_HPP_

#include <vector>
#include <mutex>

#include "Monitor_std.hpp"

template <typename B>
class Monitor_reduction : public Monitor_std<B>
{
private:
	std::vector<Monitor<B>*>& error_analyzers;
	std::vector<int> prev_n_be;
	std::vector<int> prev_n_fe;
	std::vector<unsigned long long> prev_n_analyzed_frames;

	std::mutex mutex_total_frame_errors;

public:
	Monitor_reduction(const int& K, const int& N, const int& max_fe,
	                  std::vector<Monitor<B>*>& error_analyzers, const int& n_frames = 1,
	                  const std::string name = "Monitor_reduction");
	virtual ~Monitor_reduction();

	void reduce();
	void increment_frame_errors(const int n_frames);
};

#endif /* MONITOR_REDUCTION_HPP_ */
