#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Decoder.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Decoder::
Decoder(const int K, const int N, const int n_frames, const int simd_inter_frame_level)
: Module(n_frames),
  n_inter_frame_rest(this->n_frames % simd_inter_frame_level),
  K(K),
  N(N),
  simd_inter_frame_level(simd_inter_frame_level),
  n_dec_waves((int)std::ceil((float)this->n_frames / (float)simd_inter_frame_level))
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

	this->tasks_with_nullptr.resize((size_t)dec::tsk::SIZE, nullptr);
}

Decoder::
~Decoder()
{
}

int Decoder::
get_K() const
{
	return this->K;
}

int Decoder::
get_N() const
{
	return this->N;
}

int Decoder::
get_simd_inter_frame_level() const
{
	return this->simd_inter_frame_level;
}

int Decoder::
get_n_dec_waves() const
{
	return this->n_dec_waves;
}

void Decoder::
reset()
{

}
