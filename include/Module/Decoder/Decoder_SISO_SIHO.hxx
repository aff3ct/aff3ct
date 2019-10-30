#include <string>

#include "Module/Module.hpp"
#include "Module/Decoder/Decoder_SISO_SIHO.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename R>
Task& Decoder_SISO_SIHO<B,R>
::operator[](const dec::tsk t)
{
	return Module::operator[]((int)t);
}

template <typename B, typename R>
Socket& Decoder_SISO_SIHO<B,R>
::operator[](const dec::sck::decode_siso s)
{
	return Module::operator[]((int)dec::tsk::decode_siso )[(int)s];
}

template <typename B, typename R>
Socket& Decoder_SISO_SIHO<B,R>
::operator[](const dec::sck::decode_siho s)
{
	return Module::operator[]((int)dec::tsk::decode_siho)[(int)s];
}

template <typename B, typename R>
Socket& Decoder_SISO_SIHO<B,R>
::operator[](const dec::sck::decode_siho_cw s)
{
	return Module::operator[]((int)dec::tsk::decode_siho_cw)[(int)s];
}

template <typename B, typename R>
Decoder_SISO_SIHO<B,R>
::Decoder_SISO_SIHO(const int K, const int N, const int n_frames, const int simd_inter_frame_level)
: Decoder          (K, N, n_frames, simd_inter_frame_level),
  Decoder_SIHO<B,R>(K, N, n_frames, simd_inter_frame_level),
  Decoder_SISO<  R>(K, N, n_frames, simd_inter_frame_level)
{
	const std::string name = "Decoder_SISO_SIHO";
	this->set_name(name);
}

}
}
