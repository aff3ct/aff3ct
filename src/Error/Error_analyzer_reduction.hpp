#ifndef ERROR_ANALYZER_REDUCTION_HPP_
#define ERROR_ANALYZER_REDUCTION_HPP_

#include <vector>
#include <mutex>
/*#ifdef _WIN32
// see https://github.com/meganz/mingw-std-threads
#include <mingw.mutex.h>
#endif*/

#include "Error_analyzer_std.hpp"

template <typename B>
class Error_analyzer_reduction : public Error_analyzer_std<B>
{
private:
	std::vector<Error_analyzer<B>*>& error_analyzers;
	std::vector<int> prev_n_be;
	std::vector<int> prev_n_fe;
	std::vector<unsigned long long> prev_n_analyzed_frames;

	std::mutex mutex_total_frame_errors;

public:
	Error_analyzer_reduction(const int& K, const int& N, const int& max_fe, 
	                         std::vector<Error_analyzer<B>*>& error_analyzers, const int& n_frames = 1, 
	                         const std::string name = "Error_analyzer_reduction");
	virtual ~Error_analyzer_reduction();

	void reduce();
	void increment_frame_errors(const int n_frames);
};

#endif /* ERROR_ANALYZER_REDUCTION_HPP_ */
