/*!
 * \file
 * \brief Class module::Monitor.
 */
#ifndef MONITOR_HPP_
#define MONITOR_HPP_

#include <cstdint>
#include <cstddef>

#include "Tools/Interface/Interface_reset.hpp"
#include "Tools/Interface/Interface_is_done.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace mnt
	{
		enum class tsk : size_t { check_errors, check_errors2, get_mutual_info, check_mutual_info, SIZE };

		namespace sck
		{
			enum class check_errors      : size_t { U, V, status };
			enum class check_errors2     : size_t { U, V, FRA, BE, FE, BER, FER, status };
			enum class get_mutual_info   : size_t { X, Y, status };
			enum class check_mutual_info : size_t { bits, llrs_a, llrs_e, status };
		}
	}

/*!
 * \class Monitor
 *
 * \brief Monitors the simulated frames, tells if there is a frame errors and counts the number of bit errors.
 *
 * Please use Monitor for inheritance (instead of Monitor).
 */
class Monitor : public Module, public tools::Interface_reset, public tools::Interface_is_done
{
public:

	/*!
	 * \brief Constructor.
	 *
	 * Registers the SIGINT (signal interrupt or ctrl+C) interruption.
	 */
	explicit Monitor();

	/*!
	 * \brief Destructor.
	 */
	virtual ~Monitor() = default;

	virtual Monitor* clone() const;

	virtual void reset();

	virtual void clear_callbacks();

	/*
	 * \brief return true if enough data have been processed and respect check conditions
	 */
	virtual bool is_done() const = 0;
};
}
}

#endif /* MONITOR_HPP_ */
