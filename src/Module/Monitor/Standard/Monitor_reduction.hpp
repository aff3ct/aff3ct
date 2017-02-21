#ifndef MONITOR_REDUCTION_HPP_
#define MONITOR_REDUCTION_HPP_

#include <string>
#include <vector>
#include <fstream>

#include "Monitor_std.hpp"

template <typename B, typename R>
class Monitor_reduction : public Monitor_std<B,R>
{
private:
	std::vector<Monitor<B,R>*>& monitors;
	const float                 snr;
	const std::string           error_tracker_head_file_name;

public:
	Monitor_reduction(const int& K, const int& N, const int& Y_size, const int& max_fe,
	                  std::vector<Monitor<B,R>*>& monitors,
	                  const float snr,
	                  const bool& error_tracker_enable = false,
	                  const std::string& error_tracker_head_file_name = "error_tracker",
	                  const int& n_frames = 1, const std::string name = "Monitor_reduction");
	virtual ~Monitor_reduction();

	unsigned long long get_n_analyzed_fra() const;
	int get_n_fe() const;
	int get_n_be() const;

private:
	void flush_erroneous_frame();
};

#endif /* MONITOR_REDUCTION_HPP_ */
