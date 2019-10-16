#ifndef MONITOR_MI_HPP_
#define MONITOR_MI_HPP_

#include <vector>
#include <memory>
#include <functional>

#include "Tools/Algo/Histogram.hpp"
#include "Module/Monitor/Monitor.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int, typename R = float>
class Monitor_MI : public Monitor
{
public:
	inline Task&   operator[](const mnt::tsk                  t) { return Module::operator[]((int)t);                                 }
	inline Socket& operator[](const mnt::sck::get_mutual_info s) { return Module::operator[]((int)mnt::tsk::get_mutual_info)[(int)s]; }

protected:
	struct Attributes
	{
		unsigned long long n_trials; // Number of checked trials
		R                  MI;       // the mutual information
		R                  MI_max;   // the maximum obtained MI
		R                  MI_min;   // the minimum obtained MI

		Attributes();
		void reset();
		Attributes& operator+=(const Attributes&);
	};

private:
	const int      N;            // Number of frame bits
	const unsigned max_n_trials; // max number of trials to check then n_trials_limit_achieved() returns true

	Attributes vals;
	tools::Histogram<R> mutinfo_hist; // the MI histogram record
	bool mutinfo_hist_activated;

	std::vector<std::function<void(void)>> callbacks_check;
	std::vector<std::function<void(void)>> callbacks_n_trials_limit_achieved;

public:
	/*
	 * 'max_n_cf' is the max number of frames to checked after what the simulation shall stop
	 */
	Monitor_MI(const int N, const unsigned max_n_trials, const int n_frames = 1);
	Monitor_MI(const Monitor_MI<B,R>& m, const int n_frames = -1); // construct with the same parameters than "m"
	                                                               // if n_frames != -1 then set it has "n_frames" value
	Monitor_MI(); // construct with null and default parameters.

	virtual ~Monitor_MI() = default;

	bool equivalent(const Monitor_MI<B,R>& m, bool do_throw = false) const; // check if this monitor and "m" have equivalent construction arguments
	                                                                        // and then can be merged by "collect" or "copy" methods
	template <class AB = std::allocator<B>, class AR = std::allocator<R>>
	R get_mutual_info(const std::vector<B,AB>& X, const std::vector<R,AR>& Y, const int frame_id = -1);

	virtual R get_mutual_info(const B *X, const R *Y, const int frame_id = -1);

	bool n_trials_limit_achieved() const;
	virtual bool is_done() const;


	const Attributes&   get_attributes  () const;
	int                 get_N           () const;
	unsigned            get_max_n_trials() const;
	unsigned long long  get_n_trials    () const;
	R                   get_MI          () const;
	R                   get_MI_min      () const;
	R                   get_MI_max      () const;

	tools::Histogram<R> get_mutinfo_hist() const;
	void activate_mutinfo_histogram(bool val);

	virtual void add_handler_check                  (std::function<void(void)> callback);
	virtual void add_handler_n_trials_limit_achieved(std::function<void(void)> callback);

	virtual void reset();
	virtual void clear_callbacks();

	virtual void collect(const Monitor& m,         bool fully = false);
	virtual void collect(const Monitor_MI<B,R>& m, bool fully = false);
	virtual void collect(const Attributes& v);

	Monitor_MI<B,R>& operator+=(const Monitor_MI<B,R>& m); // "collect" call with fully = false


	virtual void copy(const Monitor& m,         bool fully = false);
	virtual void copy(const Monitor_MI<B,R>& m, bool fully = false);
	virtual void copy(const Attributes& v);

	Monitor_MI<B,R>& operator=(const Monitor_MI<B,R>& m); // "copy" call  with fully = false

protected:
	virtual R _get_mutual_info(const B *X, const R *Y, const int frame_id);

	void add_MI_value(const R mi);
};
}
}

#include "Module/Monitor/MI/Monitor_MI.hxx"

#endif /* MONITOR_MI_HPP_ */
