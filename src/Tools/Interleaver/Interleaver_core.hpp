#ifndef INTERLEAVER_CORE_HPP_
#define INTERLEAVER_CORE_HPP_

#include <cstdint>
#include <string>
#include <vector>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = uint32_t>
class Interleaver_core
{
protected:
	const int size;
	const std::string name;
	const int n_frames;
	      bool uniform;
	      bool initialized;
	std::vector<T> pi;     /*!< Lookup table for the interleaving process */
	std::vector<T> pi_inv; /*!< Lookup table for the deinterleaving process */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param size:     number of the data to interleave or to deinterleave.
	 * \param n_frames: number of frames to process in the Interleaver.
	 * \param name:     Interleaver's name.
	 */
	Interleaver_core(const int size, const std::string &name, const bool uniform = false, const int n_frames = 1)
	: size(size), name(name), n_frames(n_frames), uniform(uniform), initialized(false),
	  pi(size * n_frames, 0), pi_inv(size * n_frames, 0)
	{
		if (size <= 0)
		{
			std::stringstream message;
			message << "'size' has to be greater than 0 ('size' = " << size << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (n_frames <= 0)
		{
			std::stringstream message;
			message << "'n_frames' has to be greater than 0 ('n_frames' = " << size << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (name.empty())
		{
			std::stringstream message;
			message << "'name' cannot be empty.";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	virtual ~Interleaver_core() = default;

	const std::vector<T>& get_lut() const
	{
		return pi;
	}

	const std::vector<T>& get_lut_inv() const
	{
		return pi_inv;
	}

	int get_size() const
	{
		return size;
	}

	int get_n_frames() const
	{
		return n_frames;
	}

	bool is_uniform() const
	{
		return uniform;
	}

	bool is_initialized() const
	{
		return initialized;
	}

	std::string get_name() const
	{
		return name;
	}

	void init()
	{
		this->refresh();
		this->initialized = true;
	}

	void refresh()
	{
		this->gen_lut(this->pi.data(), 0);
		for (auto i = 0; i < (int)this->get_size(); i++)
			this->pi_inv[this->pi[i]] = i;

		if (uniform)
		{
			for (auto f = 1; f < this->n_frames; f++)
			{
				const auto off = f * this->size;

				this->gen_lut(this->pi.data() + off, f);

				for (auto i = 0; i < this->get_size(); i++)
					this->pi_inv[off + this->pi[off +i]] = i;
			}
		}
		else
		{
			for (auto f = 1; f < this->n_frames; f++)
			{
				std::copy(pi    .data(), pi    .data() + size, pi    .data() + f * size);
				std::copy(pi_inv.data(), pi_inv.data() + size, pi_inv.data() + f * size);
			}
		}
	}

protected:
	virtual void gen_lut(T *lut, const int frame_id) = 0;
};
}
}

#endif /* INTERLEAVER_CORE_HPP_ */
