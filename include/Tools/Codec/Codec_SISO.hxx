#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Codec/Codec_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B, typename Q>
Codec_SISO<B,Q>
::Codec_SISO(const int K, const int N_cw, const int N)
: Codec_SIHO<B,Q>(K, N_cw, N), decoder_siso(nullptr)
{
}

template <typename B, typename Q>
Codec_SISO<B,Q>* Codec_SISO<B,Q>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Codec_SISO<B,Q>
::deep_copy(const Codec_SISO<B,Q> &t)
{
	Codec_SIHO<B,Q>::deep_copy(t);
	if (this->decoder_siho != nullptr)
		this->decoder_siso = std::dynamic_pointer_cast<module::Decoder_SISO<B,Q>>(this->decoder_siho);
}

template <typename B, typename Q>
module::Decoder_SISO<B,Q>& Codec_SISO<B,Q>
::get_decoder_siso()
{
	if (this->decoder_siso == nullptr)
	{
		std::stringstream message;
		message << "'decoder_siso' is NULL.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->decoder_siso;
}

template <typename B, typename Q>
void Codec_SISO<B,Q>
::set_decoder_siso(module::Decoder_SISO<B,Q>* dec)
{
	this->set_decoder_siso(std::shared_ptr<module::Decoder_SISO<B,Q>>(dec));
}

template <typename B, typename Q>
void Codec_SISO<B,Q>
::set_decoder_siso(std::shared_ptr<module::Decoder_SISO<B,Q>> dec)
{
	this->decoder_siso = dec;
	this->set_decoder_siho(this->decoder_siso);
}

template <typename B, typename Q>
void Codec_SISO<B,Q>
::set_n_frames(const size_t n_frames)
{
	Codec_SIHO<B,Q>::set_n_frames(n_frames);
	if (this->decoder_siso != nullptr)
		this->decoder_siso->set_n_frames(n_frames);
}

}
}
