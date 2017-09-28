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
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Quantizer_i
 *
 * \brief Quantizes floating-point data to fixed-point representation.
 *
 * \tparam R: type of the reals (floating-point representation) in the Quantizer.
 * \tparam Q: type of the reals (floating-point or fixed-point representation) in the Quantizer.
 *
 * Please use Quantizer for inheritance (instead of Quantizer_i).
 * If Q is a floating-point representation then the Quantizer does nothing more than a cast.
 */
template <typename R = float, typename Q = int>
class Quantizer_i : public Module
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
	Quantizer_i(const int N, const int n_frames = 1, const std::string name = "Quantizer_i")
	: Module(n_frames, name, "Quantizer"), N(N)
	{
		if (N <= 0)
		{
			std::stringstream message;
			message << "'N' has to be greater than 0 ('N' = " << N << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		auto &p = this->create_task("process");
		this->template create_socket_in <R>(p, "Y_N1", this->N * this->n_frames);
		this->template create_socket_out<Q>(p, "Y_N2", this->N * this->n_frames);
		this->create_codelet(p, [&]() -> int
		{
			this->process(static_cast<R*>(p["Y_N1"].get_dataptr()),
			              static_cast<Q*>(p["Y_N2"].get_dataptr()));

			return 0;
		});
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Quantizer_i()
	{
	}

	int get_N() const
	{
		return N;
	}

	/*!
	 * \brief Quantizes the data if Q is a fixed-point representation, does nothing else.
	 *
	 * \param Y_N1: a vector of floating-point data.
	 * \param Y_N2: a vector of quantized data (fixed-point representation).
	 */
	template <class AR = std::allocator<R>, class AQ = std::allocator<Q>>
	void process(const std::vector<R,AR>& Y_N1, std::vector<Q,AQ>& Y_N2)
	{
		if (this->N * this->n_frames != (int)Y_N1.size())
		{
			std::stringstream message;
			message << "'Y_N1.size()' has to be equal to 'N' * 'n_frames' ('Y_N1.size()' = " << Y_N1.size()
			        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N * this->n_frames != (int)Y_N2.size())
		{
			std::stringstream message;
			message << "'Y_N2.size()' has to be equal to 'N' * 'n_frames' ('Y_N2.size()' = " << Y_N2.size()
			        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->process(Y_N1.data(), Y_N2.data());
	}

	virtual void process(const R *Y_N1, Q *Y_N2)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_process(Y_N1 + f * this->N,
			               Y_N2 + f * this->N,
			               f);
	}

protected:
	virtual void _process(const R *Y_N1, Q *Y_N2, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}
};
}
}

#include "SC_Quantizer.hpp"

#endif /* QUANTIZER_HPP_ */
