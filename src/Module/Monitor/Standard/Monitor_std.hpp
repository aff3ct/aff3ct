#ifndef MONITOR_STD_HPP_
#define MONITOR_STD_HPP_

#include <chrono>
#include <vector>

#include "Tools/Perf/MIPP/mipp.h"

#include "../Monitor.hpp"

template <typename B>
class Monitor_std : public Monitor<B>
{
protected:
	const int max_fe;

	int n_bit_errors;
	int n_frame_errors;
	unsigned long long n_analyzed_frames;

public:
	Monitor_std(const int& K, const int& N, const int& max_fe, const int& n_frames = 1,
	            const std::string name = "Monitor_std");
	virtual ~Monitor_std(){};

	virtual void check_errors(const mipp::vector<B>& U, const mipp::vector<B>& V);

	virtual bool fe_limit_achieved();
	int get_fe_limit() const;

	virtual unsigned long long get_n_analyzed_fra() const;
	virtual int get_n_fe() const;
	virtual int get_n_be() const;

	float get_fer() const;
	float get_ber() const;

private:
	void update_n_analyzed_frames();
};

#endif /* MONITOR_STD_HPP_ */
