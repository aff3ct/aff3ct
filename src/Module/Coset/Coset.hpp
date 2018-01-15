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

#include "Tools/Exception/exception.hpp"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace cst
	{
		namespace tsk
		{
			enum list { apply, SIZE };
		}

		namespace sck
		{
			namespace apply { enum list { ref, in, out, SIZE }; }
		}
	}

/*!
 * \class Coset
 *
 * \brief Performs the coset approach (flips the bits or the signs depending on the initial codeword).
 *
 * \tparam B: type of the bits in the ref data.
 * \tparam D: type of the input/output data.
 *
 * Please use Coset for inheritance (instead of Coset).
 */
template <typename B, typename D>
class Coset : public Module
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
	Coset(const int size, const int n_frames = 1)
	: Module(n_frames), size(size)
	{
		const std::string name = "Coset";
		this->set_name(name);
		this->set_short_name(name);

		if (size <= 0)
		{
			std::stringstream message;
			message << "'size' has to be greater than 0. ('size' = " << size << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		auto &p = this->create_task("apply");
		auto &ps_ref = this->template create_socket_in <B>(p, "ref", this->size * this->n_frames);
		auto &ps_in  = this->template create_socket_in <D>(p, "in",  this->size * this->n_frames);
		auto &ps_out = this->template create_socket_out<D>(p, "out", this->size * this->n_frames);
		this->create_codelet(p, [this, &ps_ref, &ps_in, &ps_out]() -> int
		{
			this->apply(static_cast<B*>(ps_ref.get_dataptr()),
			            static_cast<D*>(ps_in .get_dataptr()),
			            static_cast<D*>(ps_out.get_dataptr()));

			return 0;
		});
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Coset()
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
	template <class AB = std::allocator<B>, class AD = std::allocator<D>>
	void apply(const std::vector<B,AB>& ref, const std::vector<D,AD> &in, std::vector<D,AD> &out)
	{
		if (ref.size() != in.size() || in.size() != out.size())
		{
			std::stringstream message;
			message << "'ref.size()' has to be equal to 'in_data.size()' and 'out_data.size()' ('ref.size()' = "
			        << ref.size() << ", 'in_data.size()' = " << in.size() << ", 'out_data.size()' = "
			        << out.size() << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->size * this->n_frames != (int)ref.size())
		{
			std::stringstream message;
			message << "'ref.size()' has to be equal to 'size' * 'n_frames' ('ref.size()' = " << ref.size()
			        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->size * this->n_frames != (int)in.size())
		{
			std::stringstream message;
			message << "'in_data.size()' has to be equal to 'size' * 'n_frames' ('in_data.size()' = " << in.size()
			        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->size * this->n_frames != (int)out.size())
		{
			std::stringstream message;
			message << "'out_data.size()' has to be equal to 'size' * 'n_frames' ('out_data.size()' = " << out.size()
			        << ", 'size' = " << this->size << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->apply(ref.data(), in.data(), out.data());
	}

	virtual void apply(const B *ref, const D *in, D *out)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_apply(ref + f * this->size,
			             in  + f * this->size,
			             out + f * this->size,
			             f);
	}

protected:
	virtual void _apply(const B *ref, const D *in, D *out, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}
};
}
}

#endif /* COSET_HPP_ */
