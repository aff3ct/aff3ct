#include <algorithm>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Interleaver/Interleaver_core.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T>
Interleaver_core<T>
::Interleaver_core(const int size, const std::string &name, const bool uniform)
: size(size), name(name), n_frames(1), uniform(uniform), initialized(false),
  pi(size * this->n_frames, 0), pi_inv(size * this->n_frames, 0)
{
	if (size <= 0)
	{
		std::stringstream message;
		message << "'size' has to be greater than 0 ('size' = " << size << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (name.empty())
	{
		std::stringstream message;
		message << "'name' cannot be empty.";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename T>
const std::vector<T>& Interleaver_core<T>
::get_lut() const
{
	if (!this->is_initialized())
	{
		std::string message = "The 'init()' method has to be called before trying to get the lookup table.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message);
	}

	return pi;
}

template <typename T>
const std::vector<T>& Interleaver_core<T>
::get_lut_inv() const
{
	if (!this->is_initialized())
	{
		std::string message = "The 'init()' method has to be called before trying to get the inverse lookup table.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message);
	}

	return pi_inv;
}

template <typename T>
int Interleaver_core<T>
::get_size() const
{
	return size;
}

template <typename T>
size_t Interleaver_core<T>
::get_n_frames() const
{
	return n_frames;
}

template <typename T>
void Interleaver_core<T>
::set_n_frames(const size_t n_frames)
{
	if (n_frames <= 0)
	{
		std::stringstream message;
		message << "'n_frames' has to be greater than 0 ('n_frames' = " << size << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		this->initialized = false;
		this->n_frames = n_frames;

		const auto old_pi_size = this->pi.size();
		const auto new_pi_size = (old_pi_size / old_n_frames) * n_frames;
		this->pi.resize(new_pi_size, 0);

		const auto old_pi_inv_size = this->pi_inv.size();
		const auto new_pi_inv_size = (old_pi_inv_size / old_n_frames) * n_frames;
		this->pi_inv.resize(new_pi_inv_size, 0);

		this->init();
	}
}

template <typename T>
bool Interleaver_core<T>
::is_uniform() const
{
	return uniform;
}

template <typename T>
bool Interleaver_core<T>
::is_initialized() const
{
	return initialized;
}

template <typename T>
std::string Interleaver_core<T>
::get_name() const
{
	return name;
}

template <typename T>
void Interleaver_core<T>
::init()
{
	this->refresh();
	this->initialized = true;
}

template <typename T>
void Interleaver_core<T>
::refresh()
{
	this->gen_lut(this->pi.data(), 0);
	for (auto i = 0; i < (int)this->get_size(); i++)
		this->pi_inv[this->pi[i]] = i;

	if (uniform)
	{
		for (size_t f = 1; f < this->n_frames; f++)
		{
			const auto off = f * this->size;

			this->gen_lut(this->pi.data() + off, f);

			for (auto i = 0; i < this->get_size(); i++)
				this->pi_inv[off + this->pi[off +i]] = i;
		}
	}
	else
	{
		for (size_t f = 1; f < this->n_frames; f++)
		{
			std::copy(pi    .data(), pi    .data() + size, pi    .data() + f * size);
			std::copy(pi_inv.data(), pi_inv.data() + size, pi_inv.data() + f * size);
		}
	}
}

template <typename T>
void Interleaver_core<T>
::set_seed(const int seed)
{
	// do nothing in the general case, this method has to be overrided
}

template <typename T>
void Interleaver_core<T>
::reinitialize()
{
}

}
}
