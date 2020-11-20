/*!
 * \file
 * \brief Class module::Monitor_EXIT.
 */
#ifndef MONITOR_EXIT_HPP_
#define MONITOR_EXIT_HPP_

#include <vector>
#include <memory>
#include <cstdint>
#include <functional>

#include "Tools/Algo/Callback/Callback.hpp"
#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Monitor/Monitor.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Monitor_EXIT : public Monitor
{
public:
	inline Task&   operator[](const mnt::tsk                    t);
	inline Socket& operator[](const mnt::sck::check_mutual_info s);

protected:
	struct Attributes
	{
		unsigned long long n_trials; // Number of checked trials
		R                  I_A_sum;  // the mutual information

		Attributes();
		void reset();
		Attributes& operator+=(const Attributes& a);
	};

private:
	const int      N;            // Number of frame bits
	const unsigned max_n_trials; // max number of trials to check then n_trials_limit_achieved() returns true

	Attributes vals;

	tools::Callback<> callback_measure;

	std::vector<B> bits_buff;
	std::vector<R> llrs_e_buff;

public:
	Monitor_EXIT(const int size, const unsigned max_n_trials);

	virtual ~Monitor_EXIT() = default;

	virtual Monitor_EXIT<B,R>* clone() const;

	bool equivalent(const Monitor_EXIT<B,R>& m, bool do_throw = false) const; // check if this monitor and "m" have equivalent construction arguments
	                                                                          // and then can be merged by "collect" or "copy" methods
	/*!
	 * \brief Compares two messages and counts the number of frame errors and bit errors.
	 *
	 * Typically this method is called at the very end of a communication chain.
	 */
	template <class AB = std::allocator<B>, class AR = std::allocator<R>>
	void check_mutual_info(const std::vector<B,AB>& bits,
	                       const std::vector<R,AR>& llrs_a,
	                       const std::vector<R,AR>& llrs_e,
	                       const int frame_id = -1,
	                       const bool managed_memory = true);

	void check_mutual_info(const B *bits, const R *llrs_a, const R *llrs_e, const int frame_id = -1,
	                       const bool managed_memory = true);

	virtual bool is_done() const;
	bool n_trials_achieved() const;

	int                get_N           () const;
	unsigned           get_max_n_trials() const;
	unsigned long long get_n_trials    () const;
	R                  get_I_A         () const;
	R                  get_I_E         () const;

	virtual uint32_t record_callback_measure(std::function<void(void)> callback);
	virtual bool unrecord_callback_measure(const uint32_t id);

	virtual void reset();
	virtual void clear_callbacks();

	virtual void collect(const Monitor& m,           bool fully = false);
	virtual void collect(const Monitor_EXIT<B,R>& m, bool fully = false);
	virtual void collect(const Attributes& v);

	Monitor_EXIT<B,R>& operator+=(const Monitor_EXIT<B,R>& m); // not full "collect" call

	virtual void copy(const Monitor& m,           bool fully = false);
	virtual void copy(const Monitor_EXIT<B,R>& m, bool fully = false);
	virtual void copy(const Attributes& v);

	Monitor_EXIT<B,R>& operator=(const Monitor_EXIT<B,R>& m); // not full "copy" call

protected:
	const Attributes& get_attributes() const;

	virtual void _check_mutual_info(const B *bits, const R *llrs_a, const R *llrs_e, const size_t frame_id);
	virtual void _check_mutual_info_avg(const B *bits, const R *llrs_a, const size_t frame_id);
	virtual R _check_mutual_info_histo() const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Monitor/EXIT/Monitor_EXIT.hxx"
#endif

#endif /* MONITOR_EXIT_HPP_ */
