#ifndef CODEC_HIHO_HPP_
#define CODEC_HIHO_HPP_

#include <memory>

#include "Module/Decoder/Decoder_HIHO.hpp"
#include "Module/Codec/Codec.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_HIHO : virtual public Codec<B,Q>
{
private:
	std::shared_ptr<Decoder_HIHO<B>> decoder_hiho;

public:
	Codec_HIHO(const int K, const int N_cw, const int N, const int tail_length = 0, const int n_frames = 1);

	virtual ~Codec_HIHO() = default;

	const std::shared_ptr<Decoder_HIHO<B>>& get_decoder_hiho();

	virtual void reset();

protected:
	void set_decoder_hiho(std::shared_ptr<Decoder_HIHO<B>> dec);
	void set_decoder_hiho(Decoder_HIHO<B>* dec);
};
}
}

#include "Module/Codec/Codec_HIHO.hxx"

#endif /* CODEC_HIHO_HPP_ */
