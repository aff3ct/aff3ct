#ifndef CODEC_SIHO_HPP_
#define CODEC_SIHO_HPP_

#include "Module/Decoder/Decoder_SIHO.hpp"

#include "Codec.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_SIHO : virtual public Codec<B,Q>
{
private:
	Decoder_SIHO<B,Q>* decoder_siho;

public:
	Codec_SIHO(const int K, const int N_cw, const int N, const int tail_length = 0, const int n_frames = 1,
	           const std::string name = "Codec_SIHO")
	: Codec<B,Q>(K, N_cw, N, tail_length, n_frames, name), decoder_siho(nullptr)
	{
	}

	virtual ~Codec_SIHO()
	{
		if (decoder_siho != nullptr) { delete decoder_siho; decoder_siho = nullptr; }
	}

	Decoder_SIHO<B,Q>* get_decoder_siho()
	{
		if (this->decoder_siho == nullptr)
		{
			std::stringstream message;
			message << "'decoder_siho' is NULL.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		return this->decoder_siho;
	}

protected:
	void set_decoder_siho(Decoder_SIHO<B,Q>* dec)
	{
		this->decoder_siho = dec;
	}
};
}
}

#endif /* CODEC_SIHO_HPP_ */
