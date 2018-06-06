#ifndef MONITOR_BFER_HPP_
#define MONITOR_BFER_HPP_

#ifdef ENABLE_MPI
#include <mpi.h>
#endif

#include "../Monitor.hpp"
#include "Tools/Algo/Histogram.hpp"

namespace aff3ct
{
namespace module
{
	namespace mnt_er
	{
		enum class tsk : uint8_t { check_errors, SIZE };

		namespace sck
		{
			enum class check_errors : uint8_t { U, V, SIZE };
		}
	}

template <typename B = int>
class Monitor_BFER : virtual public Monitor
{
public:
	inline Task&   operator[](const mnt_er::tsk               t) { return Module::operator[]((int)t);                                 }
	inline Socket& operator[](const mnt_er::sck::check_errors s) { return Module::operator[]((int)mnt_er::tsk::check_errors)[(int)s]; }

	struct Values_t
	{
		static constexpr unsigned n_attributes = 3;
		unsigned long long n_be;
		unsigned long long n_fe;
		unsigned long long n_fra;

		Values_t() { reset(); }

		Values_t& operator+=(const Values_t& o)
		{
			n_be  += o.n_be;
			n_fe  += o.n_fe;
			n_fra += o.n_fra;
			return *this;
		}

		void reset()
		{
			n_be  = 0;
			n_fe  = 0;
			n_fra = 0;
		}

	#ifdef ENABLE_MPI
		static void create_MPI_struct(int          blen         [n_attributes],
		                              MPI_Aint     displacements[n_attributes],
		                              MPI_Datatype oldtypes     [n_attributes])
		{
			blen[0] = 1; displacements[0] = offsetof(Values_t, n_be ); oldtypes[0] = MPI_UNSIGNED_LONG_LONG;
			blen[1] = 1; displacements[1] = offsetof(Values_t, n_fe ); oldtypes[1] = MPI_UNSIGNED_LONG_LONG;
			blen[2] = 1; displacements[2] = offsetof(Values_t, n_fra); oldtypes[2] = MPI_UNSIGNED_LONG_LONG;
		}
	#endif
	};

protected:
	const int K; /*!< Number of source bits*/

	const unsigned max_fe;
	const bool count_unknown_values;

	Values_t vals;
	tools::Histogram<int> err_hist;

	std::vector<std::function<void(unsigned, int )>> callbacks_fe;
	std::vector<std::function<void(          void)>> callbacks_check;
	std::vector<std::function<void(          void)>> callbacks_fe_limit_achieved;

public:
	Monitor_BFER(const int K, const unsigned max_fe, const bool count_unknown_values = false, const int n_frames = 1);

	Monitor_BFER() = default;
	Monitor_BFER(const Monitor_BFER<B>& mon, const int n_frames = -1);

	virtual ~Monitor_BFER() = default;

	int get_K() const;

	/*!
	 * \brief Compares two messages and counts the number of frame errors and bit errors.
	 *
	 * Typically this method is called at the very end of a communication chain.
	 *
	 * \param U: the original message (from the Source or the CRC).
	 * \param Y: the decoded message (from the Decoder).
	 */
	template <class A = std::allocator<B>>
	int check_errors(const std::vector<B,A>& U, const std::vector<B,A>& Y, const int frame_id = -1)
	{
		if ((int)U.K() != this->K * this->n_frames)
		{
			std::stringstream message;
			message << "'U.K()' has to be equal to 'K' * 'n_frames' ('U.K()' = " << U.K()
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

		return this->check_errors(U.data(), Y.data(), frame_id);
	}

	virtual int check_errors(const B *U, const B *Y, const int frame_id = -1);

	virtual bool fe_limit_achieved();
	unsigned get_fe_limit() const;

	bool get_count_unknown_values() const;

	virtual unsigned long long    get_n_analyzed_fra() const;
	virtual unsigned long long    get_n_fe          () const;
	virtual unsigned long long    get_n_be          () const;
	virtual tools::Histogram<int> get_err_hist      () const;

	float get_fer() const;
	float get_ber() const;

	virtual void add_handler_fe               (std::function<void(unsigned, int )> callback);
	virtual void add_handler_check            (std::function<void(          void)> callback);
	virtual void add_handler_fe_limit_achieved(std::function<void(          void)> callback);

	virtual void reset();
	virtual void clear_callbacks();

	virtual void collect(const Monitor& m);
	virtual void collect(const Monitor_BFER<B>& m);

protected:
	virtual int _check_errors(const B *U, const B *Y, const int frame_id);
};
}
}

#endif /* MONITOR_BFER_HPP_ */
