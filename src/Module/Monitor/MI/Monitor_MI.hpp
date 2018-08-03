#ifndef MONITOR_MI_HPP_
#define MONITOR_MI_HPP_

#include "../Monitor.hpp"
#include "Tools/Algo/Histogram.hpp"

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
		const int      N;            // Number of frame bits
		const int      n_frames;     // Number of frames
		const unsigned max_n_trials; // max number of trials to check then n_trials_limit_achieved() returns true

		unsigned long long n_trials; // Number of checked trials
		R                  MI;       // the mutual information
		R                  MI_max;   // the maximum obtained MI
		R                  MI_min;   // the minimum obtained MI
	};

private:
	Attributes vals;
	tools::Histogram<R> mutinfo_hist; // the MI histogram record

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
	Monitor_MI(const Attributes& v); // init the monitor with the given values

	virtual ~Monitor_MI() = default;

	bool equivalent(const Monitor_MI<B,R>& m, bool do_throw = false) const; // check if this monitor and "m" have equivalent construction arguments
	                                                                        // and then can be merged by "collect" or "copy" methods


	template <class AB = std::allocator<B>, class AR = std::allocator<R>>
	R get_mutual_info(const std::vector<B,AB>& X, const std::vector<R,AR>& Y, const int frame_id = -1)
	{
		if ((int)X.K() != this->K * this->n_frames)
		{
			std::stringstream message;
			message << "'X.K()' has to be equal to 'K' * 'n_frames' ('X.K()' = " << X.K()
			        << ", 'K' = " << this->K << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if ((int)Y.K() != this->K * this->n_frames)
		{
			std::stringstream message;
			message << "'Y.K()' has to be equal to 'K' * 'n_frames' ('Y.K()' = " << Y.K()
			        << ", 'K' = " << this->K << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (frame_id != -1 && frame_id >= this->n_frames)
		{
			std::stringstream message;
			message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
			        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		return this->check_errors(X.data(), Y.data(), frame_id);
	}

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


	virtual void add_handler_check                  (std::function<void(void)> callback);
	virtual void add_handler_n_trials_limit_achieved(std::function<void(void)> callback);

	virtual void reset();
	virtual void clear_callbacks();

	virtual void collect(const Monitor& m,         bool fully = false);
	virtual void collect(const Monitor_MI<B,R>& m, bool fully = false);
	virtual void collect(const Attributes& v);

	Monitor_MI<B,R>& operator+=(const Monitor_MI<B,R>& m); // "collect" call with fully = false
	Monitor_MI<B,R>& operator+=(const Attributes&      v); // "collect" call


	virtual void copy(const Monitor& m,         bool fully = false);
	virtual void copy(const Monitor_MI<B,R>& m, bool fully = false);
	virtual void copy(const Attributes& v);


	Monitor_MI<B,R>& operator=(const Monitor_MI<B,R>& m); // "copy" call  with fully = false
	Monitor_MI<B,R>& operator=(const Attributes&      v); // "copy" call


protected:
	virtual R _get_mutual_info(const B *X, const R *Y, const int frame_id);

	void add_MI_value(const R mi);
};
}
}

#endif /* MONITOR_MI_HPP_ */
