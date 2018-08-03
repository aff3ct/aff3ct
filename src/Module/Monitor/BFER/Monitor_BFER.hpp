#ifndef MONITOR_BFER_HPP_
#define MONITOR_BFER_HPP_

#include "../Monitor.hpp"
#include "Tools/Algo/Histogram.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int>
class Monitor_BFER : public Monitor
{
public:
	inline Task&   operator[](const mnt::tsk               t) { return Module::operator[]((int)t);                              }
	inline Socket& operator[](const mnt::sck::check_errors s) { return Module::operator[]((int)mnt::tsk::check_errors)[(int)s]; }

protected:
	struct Attributes
	{
		unsigned long long n_fra;           // the number of checked frames
		unsigned long long n_be;            // the number of wrong bits
		unsigned long long n_fe;            // the number of wrong frames

		Attributes& operator+=(const Attributes&);
	};

private:
	const int      K;                    // Number of source bits
	const unsigned max_fe;               // max number of wrong frames to get then fe_limit_achieved() returns true else if 0
	const unsigned max_n_frames;         // max number of frames to check then frame_limit_achieved() returns true else if 0
	const bool     count_unknown_values; // take into account or not the unknown values as wrong values in the checked frames

	Attributes vals;
	tools::Histogram<int> err_hist; // the error histogram record

	std::vector<std::function<void(unsigned, int )>> callbacks_fe;
	std::vector<std::function<void(          void)>> callbacks_check;
	std::vector<std::function<void(          void)>> callbacks_fe_limit_achieved;

public:
	Monitor_BFER(const int K, const unsigned max_fe, const unsigned max_n_frames = 0, const bool count_unknown_values = false, const int n_frames = 1);
	Monitor_BFER(const Monitor_BFER<B>& m, const int n_frames = -1); // construct with the same parameters than "m"
	                                                                 // if n_frames != -1 then set it has "n_frames" value
	Monitor_BFER(); // construct with null and default parameters.

	virtual ~Monitor_BFER() = default;


	bool equivalent(const Monitor_BFER<B>& m, bool do_throw = false) const; // check if this monitor and "m" have equivalent construction arguments
	                                                                        // and then can be merged by "collect" or "copy" methods

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
		if ((int)U.size() != this->K * this->n_frames)
		{
			std::stringstream message;
			message << "'U.size()' has to be equal to 'K' * 'n_frames' ('U.size()' = " << U.size()
			        << ", 'K' = " << this->K << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if ((int)Y.size() != this->K * this->n_frames)
		{
			std::stringstream message;
			message << "'Y.size()' has to be equal to 'K' * 'n_frames' ('Y.size()' = " << Y.size()
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

	bool    fe_limit_achieved() const;
	bool frame_limit_achieved() const;
	virtual bool is_done() const;

	const Attributes&     get_attributes          () const;
	int                   get_K                   () const;
	bool                  get_count_unknown_values() const;
	unsigned              get_max_fe              () const;
	unsigned              get_max_n_frames        () const;
	unsigned long long    get_n_analyzed_fra      () const;
	unsigned long long    get_n_fe                () const;
	unsigned long long    get_n_be                () const;
	float                 get_fer                 () const;
	float                 get_ber                 () const;
	tools::Histogram<int> get_err_hist            () const;

	virtual void add_handler_fe               (std::function<void(unsigned, int )> callback);
	virtual void add_handler_check            (std::function<void(          void)> callback);
	virtual void add_handler_fe_limit_achieved(std::function<void(          void)> callback);

	virtual void reset();
	virtual void clear_callbacks();

	virtual void collect(const Monitor& m,         bool fully = false);
	virtual void collect(const Monitor_BFER<B>& m, bool fully = false);
	virtual void collect(const Attributes& v);

	Monitor_BFER<B>& operator+=(const Monitor_BFER<B>& m); // not full "collect" call


	virtual void copy(const Monitor& m,         bool fully = false);
	virtual void copy(const Monitor_BFER<B>& m, bool fully = false);
	virtual void copy(const Attributes& v);

	Monitor_BFER<B>& operator=(const Monitor_BFER<B>& m); // not full "copy" call



protected:
	virtual int _check_errors(const B *U, const B *Y, const int frame_id);

};
}
}

#endif /* MONITOR_BFER_HPP_ */
