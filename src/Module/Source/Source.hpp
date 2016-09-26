/*!
 * \file
 * \brief Generates a message.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef SOURCE_HPP_
#define SOURCE_HPP_

#include <vector>
#include <string>

#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

/*!
 * \class Source_interface
 *
 * \brief Generates a message.
 *
 * \tparam B: type of the bits in the Decoder.
 *
 * Please use Source for inheritance (instead of Source_interface).
 */
template <typename B>
class Source_interface : public Module
{
protected:
	const int K; /*!< Number of information bits in one frame */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:        number of information bits in the frame.
	 * \param n_frames: number of frames to process in the Source.
	 * \param name:     Source's name.
	 */
	Source_interface(const int K, const int n_frames = 1, const std::string name = "Source_interface") 
	: Module(n_frames, name), K(K)
	{
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Source_interface()
	{
	}

	/*!
	 * \brief Fulfills a vector with bits.
	 *
	 * \param U_K: a vector of bits to fill.
	 */
	virtual void generate(mipp::vector<B>& U_K) = 0;
};

#include "SC_Source.hpp"

#endif /* SOURCE_HPP_ */
