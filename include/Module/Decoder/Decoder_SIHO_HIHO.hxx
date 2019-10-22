#include <string>

#include "Module/Module.hpp"
#include "Module/Decoder/Decoder_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename R>
Task& Decoder_SIHO_HIHO<B,R>
::operator[](const dec::tsk t)
{
	return Module::operator[]((int)t);
}

template <typename B, typename R>
Socket& Decoder_SIHO_HIHO<B,R>
::operator[](const dec::sck::decode_siho s)
{
	return Module::operator[]((int)dec::tsk::decode_siho)[(int)s];
}

template <typename B, typename R>
Socket& Decoder_SIHO_HIHO<B,R>
::operator[](const dec::sck::decode_siho_cw s)
{
	return Module::operator[]((int)dec::tsk::decode_siho_cw)[(int)s];
}

template <typename B, typename R>
Socket& Decoder_SIHO_HIHO<B,R>
::operator[](const dec::sck::decode_hiho s)
{
	return Module::operator[]((int)dec::tsk::decode_hiho)[(int)s];
}

template <typename B, typename R>
Socket& Decoder_SIHO_HIHO<B,R>
::operator[](const dec::sck::decode_hiho_cw s)
{
	return Module::operator[]((int)dec::tsk::decode_hiho_cw)[(int)s];
}

template <typename B, typename R>
Decoder_SIHO_HIHO<B,R>
::Decoder_SIHO_HIHO(const int K, const int N, const int n_frames, const int simd_inter_frame_level)
: Decoder          (K, N, n_frames, simd_inter_frame_level),
  Decoder_SIHO<B,R>(K, N, n_frames, simd_inter_frame_level),
  Decoder_HIHO<B  >(K, N, n_frames, simd_inter_frame_level)
{
	const std::string name = "Decoder_SIHO_HIHO";
	this->set_name(name);
}

}
}
