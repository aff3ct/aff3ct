#ifndef CODEC_SISO_HPP_
#define CODEC_SISO_HPP_

#include "Module/Decoder/Decoder_SISO.hpp"

#include "Codec.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_SISO : virtual public Codec<B,Q>
{
private:
	Decoder_SISO<Q> *decoder_siso;

public:
	Codec_SISO(const int K, const int N_cw, const int N, const int tail_length = 0, const int n_frames = 1)
	: Codec<B,Q>(K, N_cw, N, tail_length, n_frames), decoder_siso(nullptr)
	{
		const std::string name = "Codec_SISO";
		this->set_name(name);
	}

	virtual ~Codec_SISO()
	{
		if (decoder_siso != nullptr) { delete decoder_siso; decoder_siso = nullptr; }
	}

	Decoder_SISO<Q>* get_decoder_siso()
	{
		if (this->decoder_siso == nullptr)
		{
			std::stringstream message;
			message << "'decoder_siso' is NULL.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		return this->decoder_siso;
	}

	virtual void reset()
	{
		if (this->decoder_siso == nullptr)
		{
			std::stringstream message;
			message << "'decoder_siso' is NULL.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->decoder_siso->reset();
	}

protected:
	void set_decoder_siso(Decoder_SISO<Q>* dec)
	{
		this->decoder_siso = dec;
	}
};
}
}

#endif /* CODEC_SISO_HPP_ */
