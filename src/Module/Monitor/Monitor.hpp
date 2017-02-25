/*!
 * \file
 * \brief Monitors the simulated frames, tells if there is a frame errors and counts the number of bit errors.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef MONITOR_HPP_
#define MONITOR_HPP_

#include <csignal>
#include <chrono>
#include <vector>
#include <string>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Monitor_i
 *
 * \brief Monitors the simulated frames, tells if there is a frame errors and counts the number of bit errors.
 *
 * \tparam B: type of the bits in the frames to compare.
 * \tparam R: type of the samples in the channel frame.
 *
 * Please use Monitor for inheritance (instead of Monitor_i).
 */
template <typename B, typename R>
class Monitor_i : public Module
{
protected:
	static bool interrupt;                                                                                /*!< True if there is a SIGINT signal (ctrl+C). */
	static bool first_interrupt;                                                                          /*!< True if this is the first time that SIGIN is called. */
	static bool over;                                                                                     /*!< True if SIGINT is called twice in the Monitor_i::d_delta_interrupt time */
	static std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_last_interrupt; /*!< Time point of the last call to SIGINT */
	static std::chrono::nanoseconds d_delta_interrupt;                                                    /*!< Delta time. */

	const int K; /*!< Number of information bits in one frame */
	const int N; /*!< Size of one encoded frame (= number of bits in one frame) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * Registers the SIGINT (signal interrupt or ctrl+C) interruption.
	 *
	 * \param K:        number of information bits in the frame.
	 * \param N:        size of one frame.
	 * \param n_frames: number of frames to process in the Monitor.
	 * \param name:     Monitor's name.
	 */
	Monitor_i(const int& K, const int& N, const int& n_frames = 1, const std::string name = "Monitor_i")
	: Module(n_frames, name), K(K), N(N)
	{
		Monitor_i<B,R>::interrupt = false;
		Monitor_i<B,R>::d_delta_interrupt = std::chrono::nanoseconds(0);

#ifndef ENABLE_MPI
		// Install a signal handler
		std::signal(SIGINT, Monitor_i<B,R>::signal_interrupt_handler);
#endif
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Monitor_i()
	{
	}

	/*!
	 * \brief Gets the frame size.
	 *
	 * \return the frame size.
	 */
	int get_N() const
	{
		return N;
	}

	/*!
	 * \brief Gets the number of information bits in a frame.
	 *
	 * \return the number of information bits.
	 */
	int get_K() const
	{
		return K;
	}

	/*!
	 * \brief Gets the number of bit errors.
	 *
	 * \return the number of bit errors.
	 */
	virtual int get_n_be() const = 0;

	/*!
	 * \brief Gets the number of frame errors.
	 *
	 * \return the number of frame errors.
	 */
	virtual int get_n_fe() const = 0;

	/*!
	 * \brief Gets the bit error rate.
	 *
	 * \return the bit error rate.
	 */
	virtual float get_ber() const = 0;

	/*!
	 * \brief Gets the frame error rate.
	 *
	 * \return the frame error rate.
	 */
	virtual float get_fer() const = 0;

	/*!
	 * \brief Gets the number of analyzed frames (analyzed in the Monitor_i::check_errors method).
	 *
	 * \return the number of analyzed frames.
	 */
	virtual unsigned long long get_n_analyzed_fra() const = 0;

	/*!
	 * \brief Gets the frame errors limit (maximal number of frame errors to simulate).
	 *
	 * \return the frame errors limit.
	 */
	virtual int get_fe_limit() const = 0;

	/*!
	 * \brief Tells if the frame errors limit is achieved (in this case the current computations should stop).
	 *
	 * \return true if the frame errors limit is achieved.
	 */
	virtual bool fe_limit_achieved() = 0;

	/*!
	 * \brief Compares two messages and counts the number of frame errors and bit errors.
	 *
	 * Typically this method is called at the very end of a communication chain.
	 *
	 * \param U: the original message (from the Source or the CRC).
	 * \param V: the decoded message (from the Decoder).
	 */
	virtual void check_errors(const mipp::vector<B>& U, const mipp::vector<B>& V) = 0;

	/*!
	 * \brief Tells if the user asked for stopping the current computations.
	 *
	 * \return true if the SIGINT (ctrl+c) is called.
	 */
	static bool is_interrupt()
	{
		return Monitor_i<B,R>::interrupt;
	}

	/*!
	 * \brief Tells if the user asked for stopping the whole simulation.
	 *
	 * \return true if the SIGINT (ctrl+c) is called twice.
	 */
	static bool is_over()
	{
		return Monitor_i<B,R>::over;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// The following public methods are specific to catch the bad frames and to dump them into files. //
	// The proposed default implementation do nothing.                                                //
	////////////////////////////////////////////////////////////////////////////////////////////////////

	/*!
	 * \brief Compares two messages and counts the number of frame errors and bit errors.
	 *
	 * Typically this method is called at the very end of a communication chain.
	 *
	 * \param U:     the original message (from the Source or the CRC).
	 * \param V:     the decoded message (from the Decoder).
	 * \param X:     the encoded message (from the Encoder).
	 * \param X_mod: the modulated message (from the Modulator, the input of the Channel).
	 * \param Y:     the noised message (the output of the Channel).
	 */
	virtual void check_and_track_errors(const mipp::vector<B>& U,
	                                    const mipp::vector<B>& V,
	                                    const mipp::vector<B>& X,
	                                    const mipp::vector<R>& X_mod,
	                                    const mipp::vector<R>& Y)
	{
		check_errors(U, V);
	}

	/*!
	 * \brief Write the bad frames into files.
	 *
	 * \param base_path: base path for files to write the bad frames.
	 * \param snr:       the current SNR.
	 */
	virtual void dump_bad_frames(const std::string& base_path, const float snr)
	{
	}

	/*!
	 * \brief get the buffer recording the wrong frames from the source
	 *
	 * \return a reference to the buffer recording the wrong frames from the source
	 */
	virtual const std::vector<mipp::vector<B>> get_buff_src() const
	{
		return std::vector<mipp::vector<B>>(0);
	}

	/*!
	 * \brief get the buffer recording the wrong frames from the encoder
	 *
	 * \return a reference to the buffer recording the wrong frames from the encoder
	 */
	virtual const std::vector<mipp::vector<B>> get_buff_enc() const
	{
		return std::vector<mipp::vector<B>>(0);
	}

	/*!
	 * \brief get the buffer recording the noise added to the wrong frames in the channel
	 *
	 * \return a reference to the buffer recording the noise added to the wrong frames in the channel
	 */
	virtual const std::vector<mipp::vector<R>> get_buff_noise() const
	{
		return std::vector<mipp::vector<R>>(0);
	}

private:
	static void signal_interrupt_handler(int signal)
	{
		auto t_now = std::chrono::steady_clock::now();
		if (!Monitor_i<B,R>::first_interrupt)
		{
			Monitor_i<B,R>::d_delta_interrupt = t_now - Monitor_i<B,R>::t_last_interrupt;
			if (Monitor_i<B,R>::d_delta_interrupt < std::chrono::milliseconds(500))
				Monitor_i<B,R>::over = true;
		}
		Monitor_i<B,R>::t_last_interrupt  = t_now;

		Monitor_i<B,R>::first_interrupt = false;
		Monitor_i<B,R>::interrupt       = true;
	}
};

template <typename B, typename R>
bool Monitor_i<B,R>::interrupt = false;

template <typename B, typename R>
bool Monitor_i<B,R>::first_interrupt = true;

template <typename B, typename R>
bool Monitor_i<B,R>::over = false;

template <typename B, typename R>
std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> Monitor_i<B,R>::t_last_interrupt;

template <typename B, typename R>
std::chrono::nanoseconds Monitor_i<B,R>::d_delta_interrupt = std::chrono::nanoseconds(0);
}
}

#include "SC_Monitor.hpp"

#endif /* MONITOR_HPP_ */
