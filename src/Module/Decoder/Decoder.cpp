#include <string>
#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/Decoder.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Decoder
::Decoder(const int K, const int N, const int simd_inter_frame_level)
: Module(),
  n_inter_frame_rest(this->n_frames % simd_inter_frame_level),
  K(K),
  N(N),
  simd_inter_frame_level(simd_inter_frame_level),
  n_dec_waves((int)std::ceil((float)this->n_frames / (float)simd_inter_frame_level)),
  auto_reset(true),
  mask(std::numeric_limits<int>::max())
{
	const std::string name = "Decoder";
	this->set_name(name);
	this->set_short_name(name);

	if (K <= 0)
	{
		std::stringstream message;
		message << "'K' has to be greater than 0 ('K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (simd_inter_frame_level <= 0)
	{
		std::stringstream message;
		message << "'simd_inter_frame_level' has to be greater than 0 ('simd_inter_frame_level' = "
		        << simd_inter_frame_level << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (K > N)
	{
		std::stringstream message;
		message << "'K' has to be smaller or equal to 'N' ('K' = " << K << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->tasks_with_nullptr.resize((size_t)dec::tsk::SIZE);
}

Decoder* Decoder
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

int Decoder
::get_K() const
{
	return this->K;
}

int Decoder
::get_N() const
{
	return this->N;
}

int Decoder
::get_simd_inter_frame_level() const
{
	return this->simd_inter_frame_level;
}

int Decoder
::get_n_dec_waves() const
{
	return this->n_dec_waves;
}

bool Decoder
::is_auto_reset() const
{
	return this->auto_reset;
}

void Decoder
::set_auto_reset(const bool auto_reset)
{
	this->auto_reset = auto_reset;
}

void Decoder
::reset()
{
	this->reset(-1);
}

void Decoder
::reset(const int frame_id)
{
	if (frame_id < 0)
	{
		for (auto w = 0; w < this->get_n_dec_waves(); w++)
		{
			auto fid = w * this->get_simd_inter_frame_level();
			this->_reset(fid);
		}
	}
	else if (frame_id < this->get_n_frames())
	{
		this->_reset(frame_id);
	}
	else
	{
		std::stringstream message;
		message << "'frame_id' has to be smaller than 'get_n_frames()' ('frame_id' = " << frame_id
		        << ", 'get_n_frames()' = " << this->get_n_frames() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

void Decoder
::_reset(const int frame_id)
{
}

void Decoder
::set_seed(const int seed)
{
	// do nothing in the general case, this method has to be overrided
}

void Decoder
::set_n_frames(const int n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Module::set_n_frames(n_frames);

		this->n_inter_frame_rest = n_frames % this->simd_inter_frame_level;
		this->n_dec_waves = (int)std::ceil((float)this->n_frames / (float)simd_inter_frame_level);
	}
}