/*!
 * \file
 * \brief Modulates, filters and/or demodulates a signal.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef MODULATOR_HPP_
#define MODULATOR_HPP_

#include <string>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

/*!
 * \class Modulator_i
 *
 * \brief Modulates, filters and/or demodulates a signal.
 *
 * \tparam B: type of the bits or symbols to modulate.
 * \tparam R: type of the reals (floating-point representation) in the modulation and in the filtering process.
 * \tparam Q: type of the reals (floating-point or fixed-point representation) in the demodulation.
 *
 * Please use Modulator for inheritance (instead of Modulator_i)
 */
template <typename B, typename R, typename Q>
class Modulator_i : public Module
{
protected:
	const int N;     /*!< Size of one frame (= number of bits in one frame) */
	const int N_mod; /*!< Number of transmitted elements after the modulation (could be smaller, bigger or equal to N) */
	const int N_fil; /*!< Number of transmitted elements after the filtering process */

	const mipp::vector<R> &H; /*!< Channel gains (by default empty which means a constant gain equal to 1) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N:        size of one frame (= number of bits in one frame).
	 * \param N_mod:    number of transmitted elements after the modulation (could be smaller, bigger or equal to N).
	 * \param N_fil:    number of transmitted elements after the filtering process.
	 * \param H:        channel gains (by default empty which means a constant gain equal to 1)
	 * \param n_frames: number of frames to process in the Modulator.
	 * \param name:     Modulator's name.
	 */
	Modulator_i(const int N, const int N_mod, const int N_fil, const mipp::vector<R> &H, const int n_frames = 1,
	            const std::string name = "Modulator_i")
	: Module(n_frames, name), N(N), N_mod(N_mod), N_fil(N_fil), H(H)
	{
	}

	/*!
	 * \brief Constructor (assumes that nothing is done in the filtering process).
	 *
	 * \param N:        size of one frame (= number of bits in one frame).
	 * \param N_mod:    number of transmitted elements after the modulation (could be smaller, bigger or equal to N).
	 * \param H:        channel gains (by default empty which means a constant gain equal to 1)
	 * \param n_frames: number of frames to process in the Modulator.
	 * \param name:     Modulator's name.
	 */
	Modulator_i(const int N, const int N_mod, const mipp::vector<R> &H, const int n_frames = 1,
	            const std::string name = "Modulator_i")
	: Module(n_frames, name), N(N), N_mod(N_mod), N_fil(get_buffer_size_after_filtering(N_mod)), H(H)
	{
	}

	/*!
	 * \brief Constructor (assumes that nothing is done in the filtering process).
	 *
	 * \param N:        size of one frame (= number of bits in one frame).
	 * \param n_frames: number of frames to process in the Modulator.
	 * \param H:        channel gains (by default empty which means a constant gain equal to 1)
	 * \param name:     Modulator's name.
	 */
	Modulator_i(const int N, const mipp::vector<R> &H, const int n_frames = 1,
	            const std::string name = "Modulator_i")
	: Module(n_frames, name), N(N), N_mod(get_buffer_size_after_modulation(N)),
	  N_fil(get_buffer_size_after_filtering(N)), H(H)
	{
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Modulator_i()
	{
	}

	/*!
	 * \brief Modulates a vector of bits or symbols.
	 *
	 * \param X_N1: a vector of bits or symbols.
	 * \param X_N2: a vector of modulated bits or symbols.
	 */
	virtual void modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2) = 0;

	/*!
	 * \brief Filters a vector of noised and modulated bits/symbols.
	 *
	 * By default this method does nothing.
	 *
	 * \param Y_N1: a vector of noised and modulated bits/symbols.
	 * \param Y_N2: a filtered vector.
	 */
	virtual void filter(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2)
	{
		assert(Y_N1.size() == Y_N2.size());
		Y_N2 = Y_N1;
	}

	/*!
	 * \brief Demodulates a vector of noised and modulated bits/symbols (after the filtering process if required).
	 *
	 * \param Y_N1: a vector of noised and modulated bits/symbols.
	 * \param Y_N2: a demodulated vector.
	 */
	virtual void demodulate(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2) = 0;

	/*!
	 * \brief Demodulates a vector of noised and modulated bits/symbols (after the filtering process if required).
	 *
	 * Used for the iterative turbo demodulation technique, this type of demodulation takes the decoder information
	 * into account.
	 *
	 * \param Y_N1: a vector of noised and modulated bits/symbols.
	 * \param Y_N2: a vector of extrinsic information coming from a SISO decoder (used in the iterative turbo
	 *              demodulation technique).
	 * \param Y_N3: a demodulated vector.
	 */
	virtual void demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3)
	{
		demodulate(Y_N1, Y_N3);
	}

	/*!
	 * \brief Gets the vector size after the modulation (considering a given frame size).
	 *
	 * \param N: a frame size.
	 *
	 * \return the vector size after the modulation.
	 */
	virtual int get_buffer_size_after_modulation(const int N)
	{
		return N;
	}

	/*!
	 * \brief Gets the vector size after the filtering process (considering a given frame size).
	 *
	 * \param N: a frame size.
	 *
	 * \return the vector size after the filtering process.
	 */
	virtual int get_buffer_size_after_filtering (const int N)
	{
		return get_buffer_size_after_modulation(N);
	}
};

#include "SC_Modulator.hpp"

#endif /* MODULATOR_HPP_ */
