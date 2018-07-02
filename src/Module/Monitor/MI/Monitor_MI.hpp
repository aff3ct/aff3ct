#ifndef MONITOR_MI_HPP_
#define MONITOR_MI_HPP_

#ifdef ENABLE_MPI
#include <mpi.h>
#endif

#include "../Monitor.hpp"
#include "Tools/Algo/Histogram.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int, typename R = float>
class Monitor_MI : virtual public Monitor
{
public:
	inline Task&   operator[](const mnt::tsk                  t) { return Module::operator[]((int)t);                                 }
	inline Socket& operator[](const mnt::sck::get_mutual_info s) { return Module::operator[]((int)mnt::tsk::get_mutual_info)[(int)s]; }

	struct Values_t
	{
		static constexpr unsigned n_attributes = 4;
		unsigned long long n_trials;
		R MI;
		R MI_max;
		R MI_min;

		Values_t() { reset(); }

		Values_t& operator+=(const Values_t& o)
		{
			n_trials += o.n_trials;
			MI += (o.MI - MI) / n_trials;
			MI_max = std::max(MI_max, o.MI_max);
			MI_min = std::min(MI_min, o.MI_min);
			return *this;
		}

		void reset()
		{
			MI     = 0.;
			MI_max = 0.;
			MI_min = 1.;
			n_trials  = 0;
		}

	#ifdef ENABLE_MPI
		using MPI_R_type = typename std::conditional<std::is_same<R,double>::value, MPI_DOUBLE, MPI_FLOAT>;

		static void create_MPI_struct(int          blen         [n_attributes],
		                              MPI_Aint     displacements[n_attributes],
		                              MPI_Datatype oldtypes     [n_attributes])
		{
			blen[0] = 1; displacements[0] = offsetof(Values_t, n_trials ); oldtypes[0] = MPI_UNSIGNED_LONG_LONG;
			blen[1] = 1; displacements[1] = offsetof(Values_t, MI    ); oldtypes[1] = MPI_R_type;
			blen[2] = 1; displacements[2] = offsetof(Values_t, MI_min); oldtypes[2] = MPI_R_type;
			blen[3] = 1; displacements[3] = offsetof(Values_t, MI_max); oldtypes[3] = MPI_R_type;
		}
	#endif
	};

protected:
	const int N; /*!< Number of code bits*/

	const unsigned max_n_trials;

	Values_t vals;
	tools::Histogram<R> mutinfo_hist;


	std::vector<std::function<void(void)>> callbacks_check;
	std::vector<std::function<void(void)>> callbacks_n_trials_limit_achieved;

public:
	/*
	 * 'max_n_cf' is the max number of frames to checked after what the simulation shall stop
	 */
	Monitor_MI(const int N, const unsigned max_n_trials, const int n_frames = 1);

	Monitor_MI() = default;
	Monitor_MI(const Monitor_MI<B,R>& mon, const int n_frames = -1);

	virtual ~Monitor_MI() = default;

	const Values_t& get_vals() const;

	int get_N() const;

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

	virtual bool n_trials_limit_achieved();
	unsigned get_n_trials_limit() const;

	virtual unsigned long long get_n_trials_fra() const;

	virtual R get_MI    () const;
	virtual R get_MI_min() const;
	virtual R get_MI_max() const;
	tools::Histogram<R> get_mutinfo_hist() const;

	virtual void add_handler_check                  (std::function<void(void)> callback);
	virtual void add_handler_n_trials_limit_achieved(std::function<void(void)> callback);

	virtual void reset();
	virtual void clear_callbacks();

	virtual void collect(const Monitor& m, bool fully = false);
	virtual void collect(const Monitor_MI<B,R>& m, bool fully = false);

	Monitor_MI<B,R>& operator=(const Monitor_MI<B,R>& m);

protected:
	virtual R _get_mutual_info(const B *X, const R *Y, const int frame_id);

	void add_MI_value(const R mi);

	Monitor_MI(const bool create_task, const int N, const unsigned max_n_trials, const int n_frames);

};
}
}

#endif /* MONITOR_MI_HPP_ */
