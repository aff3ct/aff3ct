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
#include <sstream>
#include <iostream>

#include "Tools/Exception/exception.hpp"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Source_i
 *
 * \brief Generates a message.
 *
 * \tparam B: type of the bits in the Source.
 *
 * Please use Source for inheritance (instead of Source_i).
 */
template <typename B = int>
class Source_i : public Module
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
	Source_i(const int K, const int n_frames = 1, const std::string name = "Source_i")
	: Module(n_frames, name), K(K)
	{
		if (K <= 0)
		{
			std::stringstream message;
			message << "'K' has to be greater than 0 ('K' = " << K << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		auto &p = this->create_process("generate");
		this->template create_socket_out<B>(p, "U_K", this->K * this->n_frames);
		this->create_codelet(p, [&]()
		{
			this->generate(static_cast<B*>(p["U_K"].get_dataptr()));
		});
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Source_i()
	{
	}

	virtual int get_K() const
	{
		return K;
	}

	/*!
	 * \brief Fulfills a vector with bits.
	 *
	 * \param U_K: a vector of bits to fill.
	 */
	template <class A = std::allocator<B>>
	void generate(std::vector<B,A>& U_K)
	{
		if (this->K * this->n_frames != (int)U_K.size())
		{
			std::stringstream message;
			message << "'U_K.size()' has to be equal to 'K' * 'n_frames' ('U_K.size()' = " << U_K.size()
			        << ", 'K' = " << this->K << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->generate(U_K.data());
	}

	virtual void generate(B *U_K)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_generate(U_K + f * this->K, f);
	}

protected:
	virtual void _generate(B *U_K, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}
};
}
}

#include "SC_Source.hpp"

#endif /* SOURCE_HPP_ */
