#ifndef MONITOR_MI_HPP_
#define MONITOR_MI_HPP_

#ifdef ENABLE_MPI
#include <mpi.h>
#endif

#include "../Monitor.hpp"

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
		static constexpr unsigned n_attributes = 2;
		unsigned long long n_fra;
		R MI_sum;

		Values_t& operator+=(const Values_t& o)
		{
			n_fra  += o.n_fra;
			MI_sum += o.MI_sum;
			return *this;
		}

		void reset()
		{
			MI_sum = 0.;
			n_fra  = 0;
		}

	#ifdef ENABLE_MPI
		using MPI_R_type = typename std::conditional<std::is_same<R,double>::value, MPI_DOUBLE, MPI_FLOAT>;

		static void create_MPI_struct(int          blen         [n_attributes],
		                              MPI_Aint     displacements[n_attributes],
		                              MPI_Datatype oldtypes     [n_attributes])
		{
			blen[0] = 1; displacements[0] = offsetof(Values_t, n_fra ); oldtypes[0] = MPI_UNSIGNED_LONG_LONG;
			blen[1] = 1; displacements[1] = offsetof(Values_t, MI_sum); oldtypes[1] = MPI_R_type;
		}
	#endif
	};

protected:
	const unsigned max_n_trials;
	Values_t vals;

	std::vector<std::function<void(void)>> callbacks_check;
	std::vector<std::function<void(void)>> callbacks_n_trials_limit_achieved;

public:
	/*
	 * 'max_n_cf' is the max number of frames to checked after what the simulation shall stop
	 */
	Monitor_MI(const int K, const int N, const unsigned max_n_trials, const int n_frames = 1);
	virtual ~Monitor_MI() = default;


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

	virtual unsigned long long get_n_analyzed_fra() const;

	virtual R get_MI    () const;
	virtual R get_MI_sum() const;

	virtual void add_handler_check                  (std::function<void(void)> callback);
	virtual void add_handler_n_trials_limit_achieved(std::function<void(void)> callback);

	virtual void reset();
	virtual void clear_callbacks();

	virtual void collect(const Monitor_MI<B,R>& m);

protected:
	virtual R _get_mutual_info(const B *X, const R *Y, const int frame_id);
};
}
}

#endif /* MONITOR_MI_HPP_ */
