/*!
 * \file
 * \brief Adds/builds and checks a Cyclic Redundancy Check (CRC) for a set of information bits.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef CRC_HPP_
#define CRC_HPP_

#include <string>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class CRC_i
 *
 * \brief Adds/builds and checks a Cyclic Redundancy Check (CRC) for a set of information bits.
 *
 * \tparam B: type of the bits in the CRC.
 *
 * Please use CRC for inheritance (instead of CRC_i).
 */
template <typename B = int>
class CRC_i : public Module
{
protected:
	const int K; /*!< Number of information bits (the CRC bits are included in K) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:        number of information bits (the CRC bits are included in K).
	 * \param n_frames: number of frames to process in the CRC.
	 * \param name:     CRC's name.
	 */
	CRC_i(const int K, const int n_frames = 1, const std::string name = "CRC_i")
	: Module(n_frames, name), K(K)
	{
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~CRC_i()
	{
	}

	/*!
	 * \brief Gets the size of the CRC (the number of bits for the CRC signature).
	 *
	 * \return the size of the CRC.
	 */
	virtual int size() const = 0;

	/*!
	 * \brief Computes and adds the CRC in the vector of information bits (the CRC bits are often put at the end of the
	 *        vector).
	 *
	 * \param U_K: a vector (size = K - CRC<B>::size()) containing the information bits, adds "CRC<B>::size()" bits in
	 *             U_K.
	 */
	virtual void build(mipp::vector<B>& U_K) = 0;

	/*!
	 * \brief Checks if the CRC is verified or not.
	 *
	 * \param V_K:      a vector containing information bits plus the CRC bits.
	 * \param n_frames: you should not use this parameter unless you know what you are doing, this parameter
	 *                  redefine the number of frames to check specifically in this method.
	 *
	 * \return true if the CRC is verified, false otherwise.
	 */
	virtual bool check(const mipp::vector<B>& V_K, const int n_frames = -1) = 0;
};
}
}

#include "SC_CRC.hpp"

#endif
