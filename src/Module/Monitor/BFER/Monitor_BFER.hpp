#ifndef MONITOR_STD_HPP_
#define MONITOR_STD_HPP_

#include <chrono>
#include <vector>
#include <functional>

#include "../Monitor.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Monitor_BFER : public Monitor
{
protected:
	const unsigned max_fe;

	unsigned long long n_bit_errors;
	unsigned long long n_frame_errors;
	unsigned long long n_analyzed_frames;

	std::vector<std::function<void(int )>> callbacks_fe;
	std::vector<std::function<void(void)>> callbacks_check;
	std::vector<std::function<void(void)>> callbacks_fe_limit_achieved;

public:
	Monitor_BFER(const int size, const unsigned max_fe, const int n_frames = 1, const std::string name = "Monitor_BFER");
	virtual ~Monitor_BFER(){};

	/*!
	 * \brief Compares two messages and counts the number of frame errors and bit errors.
	 *
	 * Typically this method is called at the very end of a communication chain.
	 *
	 * \param U: the original message (from the Source or the CRC).
	 * \param V: the decoded message (from the Decoder).
	 */
	template <class A = std::allocator<B>>
	int check_errors(const std::vector<B,A>& U, const std::vector<B,A>& V)
	{
		if ((int)U.size() != this->size * this->n_frames)
		{
			std::stringstream message;
			message << "'U.size()' has to be equal to 'size' * 'n_frames' ('U.size()' = " << U.size()
			        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if ((int)V.size() != this->size * this->n_frames)
		{
			std::stringstream message;
			message << "'V.size()' has to be equal to 'size' * 'n_frames' ('V.size()' = " << V.size()
			        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		return this->check_errors(U.data(), V.data());
	}

	virtual int check_errors(const B *U, const B *V);

	virtual bool fe_limit_achieved();
	unsigned get_fe_limit() const;

	virtual unsigned long long get_n_analyzed_fra() const;
	virtual unsigned long long get_n_fe          () const;
	virtual unsigned long long get_n_be          () const;

	float get_fer() const;
	float get_ber() const;

	virtual void add_handler_fe               (std::function<void(int )> callback);
	virtual void add_handler_check            (std::function<void(void)> callback);
	virtual void add_handler_fe_limit_achieved(std::function<void(void)> callback);

	virtual void reset();
	virtual void clear_callbacks();

protected:
	virtual int _check_errors(const B *U, const B *V, const int frame_id);
};
}
}

#endif /* MONITOR_STD_HPP_ */
