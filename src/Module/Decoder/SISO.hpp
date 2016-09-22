/*!
 * \file
 * \brief A SISO (Soft Input Soft Output) is a type of decoder which take a soft input and return a soft output.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef SISO_HPP_
#define SISO_HPP_

#include <string>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

/*!
 * \class SISO_interface
 *
 * \brief A SISO (Soft Input Soft Output) is a type of decoder which take a soft input and return a soft output.
 *
 * \tparam R: type of the reals (floating-point or fixed-point representation) in the SISO.
 *
 * Please use SISO for inheritance (instead of SISO_interface).
 */
template <typename R>
class SISO_interface : public Module
{
protected:
	const int K_siso; /*!< Number of information bits in one frame */
	const int N_siso; /*!< Size of one frame (= number of bits in one frame) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K       : number of information bits in the frame.
	 * \param N       : size of one frame.
	 * \param n_frames: number of frames to process in the SISO.
	 * \param name    : decoder name.
	 */
	SISO_interface(const int K, const int N, const int n_frames = 1, std::string name = "SISO_interface") 
	: Module(n_frames, name), K_siso(K), N_siso(N) {}

	/*!
	 * \brief Destructor.
	 */
	virtual ~SISO_interface() {};

	/*!
	 * \brief Decode a given noisy codeword. This prototype supposes that the encoded frame is systematic, can't be
	 *        used otherwise.
	 *
	 * \param sys: the systematic LLRs (original noised information bits) from the channel.
	 * \param par: the parity LLRs from the channel.
	 * \param ext: the extrinsic information about the systematic bits.
	 */
	virtual void decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext) = 0;

	/*!
	 * \brief Decode a given noisy codeword.
	 *
	 * \param Y_N1: the complete noisy codeword from the channel.
	 * \param Y_N2: the extrinsic information about all the bits in the frame.
	 */
	virtual void decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2) = 0;
	
	/*!
	 * \brief Get the number of tail bits.
	 *
	 * \return the number of tail bits.
	 */
	virtual int tail_length() const { return 0; }
};

#include "SC_SISO.hpp"

#endif /* SISO_HPP_ */
