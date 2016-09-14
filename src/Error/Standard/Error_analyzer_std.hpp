#ifndef ERROR_ANALYZER_STD_HPP_
#define ERROR_ANALYZER_STD_HPP_

#include <chrono>
#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../Error_analyzer.hpp"

template <typename B>
class Error_analyzer_std : public Error_analyzer<B>
{
protected:
	const int max_fe;

	int n_bit_errors;
	int n_frame_errors;
	unsigned long long n_analyzed_frames;

    bool last_frame_error;

public:
	Error_analyzer_std(const int& K, const int& N, const int& max_fe, const int& n_frames = 1, 
	                   const std::string name = "Error_analyzer_std");
	virtual ~Error_analyzer_std(){};

	virtual void check_errors(const mipp::vector<B>& U, const mipp::vector<B>& V);

	bool fe_limit_achieved() const;
	int get_fe_limit() const;

	unsigned long long get_n_analyzed_frames() const;
	int get_n_fe() const;
	int get_n_be() const;

	float get_fer_value() const;
	float get_ber_value() const;

	bool is_last_frame_error() { return last_frame_error; };

private:
	void update_n_analyzed_frames();
};

#endif /* ERROR_ANALYZER_STD_HPP_ */
