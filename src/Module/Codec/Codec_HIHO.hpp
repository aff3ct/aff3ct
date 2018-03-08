#ifndef CODEC_HIHO_HPP_
#define CODEC_HIHO_HPP_

#include "Module/Decoder/Decoder_HIHO.hpp"

#include "Codec.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_HIHO : virtual public Codec<B,Q>
{
private:
	Decoder_HIHO<B>* decoder_hiho;

public:
	Codec_HIHO(const int K, const int N_cw, const int N, const int tail_length = 0, const int n_frames = 1)
	: Codec<B,Q>(K, N_cw, N, tail_length, n_frames), decoder_hiho(nullptr)
	{
		const std::string name = "Codec_HIHO";
		this->set_name(name);
	}

	virtual ~Codec_HIHO()
	{
		if (decoder_hiho != nullptr) { delete decoder_hiho; decoder_hiho = nullptr; }
	}

	Decoder_HIHO<B>* get_decoder_hiho()
	{
		if (this->decoder_hiho == nullptr)
		{
			std::stringstream message;
			message << "'decoder_hiho' is NULL.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		return this->decoder_hiho;
	}

	virtual void reset()
	{
		if (this->decoder_hiho == nullptr)
		{
			std::stringstream message;
			message << "'decoder_hiho' is NULL.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->decoder_hiho->reset();
	}

protected:
	void set_decoder_hiho(Decoder_HIHO<B>* dec)
	{
		this->decoder_hiho = dec;
	}
};
}
}

#endif /* CODEC_HIHO_HPP_ */
