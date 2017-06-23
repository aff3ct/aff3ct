/*!
 * \file
 * \brief Performs the coset approach (flips the bits or the signs depending on the initial codeword).
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef COSET_HPP_
#define COSET_HPP_

#include <vector>
#include <string>
#include <sstream>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Coset_i
 *
 * \brief Performs the coset approach (flips the bits or the signs depending on the initial codeword).
 *
 * \tparam B: type of the bits in the ref data.
 * \tparam D: type of the input/output data.
 *
 * Please use Coset for inheritance (instead of Coset_i).
 */
template <typename B, typename D>
class Coset_i : public Module
{
protected:
	const int size; /*!< Size of the input data */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param size:     size of the input data.
	 * \param n_frames: number of frames to process in the Source.
	 * \param name:     Source's name.
	 */
	Coset_i(const int size, const int n_frames = 1, const std::string name = "Coset_i")
	: Module(n_frames, name), size(size)
	{
		if (size <= 0)
		{
			std::stringstream message;
			message << "'size' has to be greater than 0. ('size' = " << size << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Coset_i()
	{
	}

	int get_size() const
	{
		return this->size;
	}

	/*!
	 * \brief Applies the coset approach on data using the reference.
	 *
	 * Flips the bits or the signs depending on the reference (flips if bit ref = 1, do nothing else).
	 *
	 * \param ref:      the ref codeword.
	 * \param in_data:  the input data to apply the coset on.
	 * \param out_data: the output data after the coset application.
	 */
	void apply(const mipp::vector<B>& ref, const mipp::vector<D> &in_data, mipp::vector<D> &out_data)
	{
		if (ref.size() != in_data.size() || in_data.size() != out_data.size())
		{
			std::stringstream message;
			message << "'ref.size()' has to be equal to 'in_data.size()' and 'out_data.size()' ('ref.size()' = "
			        << ref.size() << ", 'in_data.size()' = " << in_data.size() << ", 'out_data.size()' = "
			        << out_data.size() << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->size * this->n_frames != (int)ref.size())
		{
			std::stringstream message;
			message << "'ref.size()' has to be equal to 'size' * 'n_frames' ('ref.size()' = " << ref.size()
			        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->size * this->n_frames != (int)in_data.size())
		{
			std::stringstream message;
			message << "'in_data.size()' has to be equal to 'size' * 'n_frames' ('in_data.size()' = " << in_data.size()
			        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->size * this->n_frames != (int)out_data.size())
		{
			std::stringstream message;
			message << "'out_data.size()' has to be equal to 'size' * 'n_frames' ('out_data.size()' = " << out_data.size()
			        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->apply(ref.data(), in_data.data(), out_data.data());
	}

	virtual void apply(const B *ref, const D *in_data, D *out_data)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_apply(ref      + f * this->size,
			             in_data  + f * this->size,
			             out_data + f * this->size,
			             f);
	}

protected:
	virtual void _apply(const B *ref, const D *in_data, D *out_data, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}
};
}
}

#include "SC_Coset.hpp"

#endif /* COSET_HPP_ */
