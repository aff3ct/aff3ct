/*!
 * \file
 * \brief Quantizes floating-point data to fixed-point representation.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef QUANTIZER_HPP_
#define QUANTIZER_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

/*!
 * \class Quantizer_interface
 *
 * \brief Quantizes floating-point data to fixed-point representation.
 *
 * \tparam R: type of the reals (floating-point representation) in the Quantizer.
 * \tparam Q: type of the reals (floating-point or fixed-point representation) in the Quantizer.
 *
 * Please use Quantizer for inheritance (instead of Quantizer_interface).
 * If Q is a floating-point representation then the Quantizer does nothing more than a cast.
 */
template <typename R, typename Q>
class Quantizer_interface : public Module
{
protected:
	const int N; /*!< Size of one frame (= number of bits in one frame) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N:        size of one frame.
	 * \param n_frames: number of frames to process in the Decoder.
	 * \param name:     Quantizer's name.
	 */
	Quantizer_interface(const int N, const int n_frames = 1, const std::string name = "Quantizer_interface")
	: Module(n_frames, name), N(N)
	{
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Quantizer_interface()
	{
	}

	/*!
	 * \brief Quantizes the data if Q is a fixed-point representation, does nothing else.
	 *
	 * \param Y_N1: a vector of floating-point data.
	 * \param Y_N2: a vector of quantized data (fixed-point representation).
	 */
	virtual void process(const mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2) = 0;
};

#include "SC_Quantizer.hpp"

#endif /* QUANTIZER_HPP_ */
