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
::Interleaver_core(const int size, const std::string &name, const bool uniform, const int n_frames)
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

template <typename T>
const std::vector<T>& Interleaver_core<T>
::get_lut() const
{
	return pi;
}

template <typename T>
const std::vector<T>& Interleaver_core<T>
::get_lut_inv() const
{
	return pi_inv;
}

template <typename T>
int Interleaver_core<T>
::get_size() const
{
	return size;
}

template <typename T>
int Interleaver_core<T>
::get_n_frames() const
{
	return n_frames;
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
}
}
