#ifndef MONITOR_STD_HPP_
#define MONITOR_STD_HPP_

#include <chrono>
#include <vector>

#include "Tools/Perf/MIPP/mipp.h"

#include "../Monitor.hpp"

template <typename B, typename R>
class Monitor_std : public Monitor<B,R>
{
protected:
	const int max_fe;

	int n_bit_errors;
	int n_frame_errors;
	unsigned long long n_analyzed_frames;

	std::vector<mipp::vector<B>> buff_src;
	std::vector<mipp::vector<B>> buff_enc;
	std::vector<mipp::vector<R>> buff_noise;

public:
	Monitor_std(const int& K, const int& N, const int& max_fe, const int& n_frames = 1,
	            const std::string name = "Monitor_std");
	virtual ~Monitor_std(){};

	virtual void check_errors(const mipp::vector<B>& U, const mipp::vector<B>& V);

	virtual void check_and_track_errors(const mipp::vector<B>& U,
	                                    const mipp::vector<B>& V,
	                                    const mipp::vector<B>& X,
	                                    const mipp::vector<R>& X_mod,
	                                    const mipp::vector<R>& Y);

	virtual bool fe_limit_achieved();
	int get_fe_limit() const;

	virtual unsigned long long get_n_analyzed_fra() const;
	virtual int get_n_fe() const;
	virtual int get_n_be() const;

	float get_fer() const;
	float get_ber() const;

	void dump_bad_frames(const std::string& base_path, const float snr);

	const std::vector<mipp::vector<B>> get_buff_src  () const;
	const std::vector<mipp::vector<B>> get_buff_enc  () const;
	const std::vector<mipp::vector<R>> get_buff_noise() const;

private:
	void update_n_analyzed_frames();
	void copy_bad_frame(const B* U, const B* X, const R* X_mod, const R* Y, const int Y_size);
	inline bool check_errors(const B* U, const B* V, const int length);
};

#endif /* MONITOR_STD_HPP_ */
