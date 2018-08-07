#ifndef MONITOR_EXIT_HPP_
#define MONITOR_EXIT_HPP_

#include <chrono>
#include <vector>
#include <functional>

#include "../Monitor.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Monitor_EXIT : public Monitor
{
public:
	inline Task&   operator[](const mnt::tsk                    t) { return Module::operator[]((int)t);                                   }
	inline Socket& operator[](const mnt::sck::check_mutual_info s) { return Module::operator[]((int)mnt::tsk::check_mutual_info)[(int)s]; }

protected:
	struct Attributes
	{
		unsigned long long n_trials; // Number of checked trials
		R                  I_A_sum;  // the mutual information

		Attributes();
		void reset();
		Attributes& operator+=(const Attributes&);
	};

private:
	const int      N;            // Number of frame bits
	const unsigned max_n_trials; // max number of trials to check then n_trials_limit_achieved() returns true

	Attributes vals;

	std::vector<std::function<void(void)>> callbacks_measure;

	std::vector<B> bits_buff;
	std::vector<R> llrs_e_buff;

public:
	Monitor_EXIT(const int size, const unsigned max_n_trials, const int n_frames = 1);
	Monitor_EXIT(const Monitor_EXIT<B,R>& m, const int n_frames = -1); // construct with the same parameters than "m"
	                                                                   // if n_frames != -1 then set it has "n_frames" value
	Monitor_EXIT(); // construct with null and default parameters.

	virtual ~Monitor_EXIT() = default;

	bool equivalent(const Monitor_EXIT<B,R>& m, bool do_throw = false) const; // check if this monitor and "m" have equivalent construction arguments
	                                                                          // and then can be merged by "collect" or "copy" methods

	/*!
	 * \brief Compares two messages and counts the number of frame errors and bit errors.
	 *
	 * Typically this method is called at the very end of a communication chain.
	 *
	 * \param U: the original message (from the Source or the CRC).
	 * \param V: the decoded message (from the Decoder).
	 */
	template <class AB = std::allocator<B>, class AR = std::allocator<R>>
	void check_mutual_info(const std::vector<B,AB>& bits,
	                       const std::vector<R,AR>& llrs_a,
	                       const std::vector<R,AR>& llrs_e,
	                       const int frame_id = -1)
	{
		if ((int)bits.size() != this->size * this->n_frames)
		{
			std::stringstream message;
			message << "'bits.size()' has to be equal to 'size' * 'n_frames' ('bits.size()' = " << bits.size()
			        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if ((int)llrs_a.size() != this->size * this->n_frames)
		{
			std::stringstream message;
			message << "'llrs_a.size()' has to be equal to 'size' * 'n_frames' ('llrs_a.size()' = " << llrs_a.size()
			        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if ((int)llrs_e.size() != this->size * this->n_frames)
		{
			std::stringstream message;
			message << "'llrs_e.size()' has to be equal to 'size' * 'n_frames' ('llrs_e.size()' = " << llrs_e.size()
			        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (frame_id != -1 && frame_id >= this->n_frames)
		{
			std::stringstream message;
			message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
			        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		return this->check_mutual_info(bits.data(), llrs_a.data(), llrs_e.data(), frame_id);
	}

	virtual void check_mutual_info(const B *bits, const R *llrs_a, const R *llrs_e, const int frame_id = -1);


	virtual bool is_done() const;
	bool n_trials_achieved() const;


	const Attributes&  get_attributes  () const;
	int                get_N           () const;
	unsigned           get_max_n_trials() const;
	unsigned long long get_n_trials    () const;
	R                  get_I_A         () const;
	R                  get_I_E         () const;


	virtual void add_handler_measure(std::function<void(void)> callback);

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
	virtual void _check_mutual_info_avg  (const B *bits, const R *llrs_a, const int frame_id);
	virtual R _check_mutual_info_histo() const;
};
}
}

#endif /* MONITOR_EXIT_HPP_ */
