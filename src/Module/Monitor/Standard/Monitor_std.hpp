#ifndef MONITOR_STD_HPP_
#define MONITOR_STD_HPP_

#include <chrono>
#include <vector>

#include "Tools/Perf/MIPP/mipp.h"

#include "../Monitor.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Monitor_std : public Monitor<B,R>
{
protected:
	const unsigned max_fe;

	unsigned long long n_bit_errors;
	unsigned long long n_frame_errors;
	unsigned long long n_analyzed_frames;

	std::vector<mipp::vector<B>> buff_src;
	std::vector<mipp::vector<B>> buff_enc;
	std::vector<mipp::vector<R>> buff_noise;

public:
	Monitor_std(const int& K, const int& N, const int& N_mod, const unsigned& max_fe, const int& n_frames = 1,
	            const std::string name = "Monitor_std");
	virtual ~Monitor_std(){};

	virtual bool fe_limit_achieved();
	unsigned get_fe_limit() const;

	virtual unsigned long long get_n_analyzed_fra() const;
	virtual unsigned long long get_n_fe          () const;
	virtual unsigned long long get_n_be          () const;

	float get_fer() const;
	float get_ber() const;

	void dump_bad_frames(const std::string& base_path, const float snr, mipp::vector<int> itl_pi = mipp::vector<int>(0));

	const std::vector<mipp::vector<B>> get_buff_src  () const;
	const std::vector<mipp::vector<B>> get_buff_enc  () const;
	const std::vector<mipp::vector<R>> get_buff_noise() const;

protected:
	virtual void _check_errors          (const B *U_K, const B *V_K);
	virtual void _check_and_track_errors(const B *U_K, const B *V_K, const B *X_N, const R *Y_N_mod);

private:
	void copy_bad_frame(const B* U_K, const B* X_N, const R* Y_N_mod);
	inline bool __check_errors(const B* U, const B* V, const int length);
};
}
}

#endif /* MONITOR_STD_HPP_ */
