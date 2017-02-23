#ifndef MONITOR_REDUCTION_HPP_
#define MONITOR_REDUCTION_HPP_

#include <string>
#include <vector>

#include "Monitor_std.hpp"

template <typename B, typename R>
class Monitor_reduction : public Monitor_std<B,R>
{
private:
	std::vector<Monitor<B,R>*>& monitors;
	const float                 snr;
	const std::string           error_tracker_head_file_name;

public:
	Monitor_reduction(const int& K, const int& N, const int& max_fe,
	                  std::vector<Monitor<B,R>*>& monitors,
	                  const float snr,
	                  const int& n_frames = 1, const std::string name = "Monitor_reduction");
	virtual ~Monitor_reduction();

	unsigned long long get_n_analyzed_fra() const;
	int get_n_fe() const;
	int get_n_be() const;

	static bool check_file_name(const std::string& error_tracker_head_file_name); // return true if correct
	static void get_tracker_filenames(const std::string& error_tracker_head_filename, const float snr,
	                                  std::string& filename_src, std::string& filename_enc, std::string& filename_noise);
	void flush_wrong_frame(const std::string& error_tracker_head_file_name, const float snr);
};

#endif /* MONITOR_REDUCTION_HPP_ */
