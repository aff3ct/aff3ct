/*!
 * \file
 * \brief Monitors the simulated frames, tells if there is a frame errors and counts the number of bit errors.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef MONITOR_HPP_
#define MONITOR_HPP_

#include <cstdint>

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace mnt
	{
		enum class tsk : uint8_t { check_errors, get_mutual_info, check_mutual_info, SIZE };

		namespace sck
		{
			enum class check_errors      : uint8_t { U, V, SIZE };
			enum class get_mutual_info   : uint8_t { X, Y, SIZE };
			enum class check_mutual_info : uint8_t { bits, llrs_a, llrs_e, SIZE };
		}
	}

/*!
 * \class Monitor
 *
 * \brief Monitors the simulated frames, tells if there is a frame errors and counts the number of bit errors.
 *
 * Please use Monitor for inheritance (instead of Monitor).
 */
class Monitor : public Module
{
public:

	/*!
	 * \brief Constructor.
	 *
	 * Registers the SIGINT (signal interrupt or ctrl+C) interruption.
	 *
	 * \param K: number of bits of information
	 * \param N: number of coded bits
	 */
	explicit Monitor(int n_frames = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Monitor() = default;

	virtual void reset();

	virtual void clear_callbacks();

	/*!
	 * \brief collect data from 'm' monitor into this monitor.
	 * 'fully' flag indicates if only "Value_t" struct vals must be merged
	 * or also any other attributes of the class.
	 */
	virtual void collect(const Monitor& m, bool fully = false) = 0;

	/*
	 * \brief return true if enough data have been processed and respect check conditions
	 */
	virtual bool is_done() const = 0;

	/*!
	 * \brief completely copy values of the monitor 'm' into this monitor
	 */
	Monitor& operator=(const Monitor& m);
};
}
}

#endif /* MONITOR_HPP_ */
