/*!
 * \file
 * \brief Monitors the simulated frames, tells if there is a frame errors and counts the number of bit errors.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef MONITOR_HPP_
#define MONITOR_HPP_

#include <stdexcept>
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
template <typename B = int, typename R = float>
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
	const int N_mod;

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
	Monitor_i(const int& K, const int& N, const int& N_mod, const int& n_frames = 1,
	          const std::string name = "Monitor_i")
	: Module(n_frames, name), K(K), N(N), N_mod(N_mod)
	{
		if (this->K <= 0)
			throw std::invalid_argument("aff3ct::module::Monitor: \"K\" has to be greater than 0.");
		if (this->N <= 0)
			throw std::invalid_argument("aff3ct::module::Monitor: \"N\" has to be greater than 0.");
		if (this->N_mod <= 0)
			throw std::invalid_argument("aff3ct::module::Monitor: \"N_mod\" has to be greater than 0.");
		if (this->K > this->N)
			throw std::invalid_argument("aff3ct::module::Monitor: \"K\" has to be smaller than \"N\".");

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

	int get_N_mod() const
	{
		return N_mod;
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
	virtual unsigned long long get_n_be() const = 0;

	/*!
	 * \brief Gets the number of frame errors.
	 *
	 * \return the number of frame errors.
	 */
	virtual unsigned long long get_n_fe() const = 0;

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
	virtual unsigned get_fe_limit() const = 0;

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
	void check_errors(const mipp::vector<B>& U_K, const mipp::vector<B>& V_K)
	{
		if ((int)U_K.size() != this->K * this->n_frames)
			throw std::length_error("aff3ct::module::Monitor: \"U_K.size()\" has to be equal to \"K * n_frames\".");

		if ((int)V_K.size() != this->K * this->n_frames)
			throw std::length_error("aff3ct::module::Monitor: \"V_K.size()\" has to be equal to \"K * n_frames\".");

		this->check_errors(U_K.data(), V_K.data());
	}

	virtual void check_errors(const B *U_K, const B *V_K)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_check_errors(U_K + f * this->K,
			                    V_K + f * this->K);
	}

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

	/*!
	 * \brief Put Monitor_i<B,R>::interrupt and Monitor_i<B,R>::over to true.
	 */
	static void stop()
	{
		Monitor_i<B,R>::interrupt = true;
		Monitor_i<B,R>::over      = true;
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
	 * \param U:      the original message (from the Source or the CRC).
	 * \param V:      the decoded message (from the Decoder).
	 * \param X:      the encoded message (from the Encoder).
	 * \param X_mod:  the modulated message (from the Modulator, the input of the Channel).
	 * \param Y:      the noised message (the output of the Channel).
	 */
	void check_and_track_errors(const mipp::vector<B>& U_K,
	                            const mipp::vector<B>& V_K,
	                            const mipp::vector<B>& X_N,
	                            const mipp::vector<R>& Y_N_mod)
	{
		if ((int)U_K.size() != this->K * this->n_frames)
			throw std::length_error("aff3ct::module::Monitor: \"U_K.size()\" has to be equal to \"K * n_frames\".");

		if ((int)V_K.size() != this->K * this->n_frames)
			throw std::length_error("aff3ct::module::Monitor: \"V_K.size()\" has to be equal to \"K * n_frames\".");

		if ((int)X_N.size() != this->N * this->n_frames)
			throw std::length_error("aff3ct::module::Monitor: \"X_N.size()\" has to be equal to \"N * n_frames\".");

		if ((int)Y_N_mod.size() != this->N_mod * this->n_frames)
			throw std::length_error("aff3ct::module::Monitor: \"Y_N_mod.size()\" has to be equal to "
			                        "\"N_mod * n_frames\".");

		this->check_and_track_errors(U_K.data(), V_K.data(), X_N.data(), Y_N_mod.data());
	}

	virtual void check_and_track_errors(const B *U_K, const B *V_K, const B *X_N, const R *Y_N_mod)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_check_and_track_errors(U_K     + f * this->K,
			                              V_K     + f * this->K,
			                              X_N     + f * this->N,
			                              Y_N_mod + f * this->N_mod);
	}

	/*!
	 * \brief Write the bad frames into files.
	 *
	 * \param base_path: base path for files to write the bad frames.
	 * \param snr:       the current SNR.
	 */
	virtual void dump_bad_frames(const std::string& base_path, const float snr, const mipp::vector<int>& itl_pi = mipp::vector<int>(0))
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

protected:
	virtual void _check_errors(const B *U, const B *V)
	{
		throw std::runtime_error("aff3ct::module::Monitor: \"_check_errors\" is unimplemented.");
	}

	void _check_and_track_errors(const B *U_K, const B *V_K, const B *X_N, const R *Y_N_mod)
	{
		throw std::runtime_error("aff3ct::module::Monitor: \"_check_and_track_errors\" is unimplemented.");
	}

private:
	static void signal_interrupt_handler(int signal)
	{
		auto t_now = std::chrono::steady_clock::now();
		if (!Monitor_i<B,R>::first_interrupt)
		{
			Monitor_i<B,R>::d_delta_interrupt = t_now - Monitor_i<B,R>::t_last_interrupt;
			if (Monitor_i<B,R>::d_delta_interrupt < std::chrono::milliseconds(500))
				Monitor_i<B,R>::stop();
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
