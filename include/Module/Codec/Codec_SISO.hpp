#ifndef CODEC_SISO_HPP_
#define CODEC_SISO_HPP_

#include <memory>

#include "Module/Decoder/Decoder_SISO.hpp"
#include "Module/Codec/Codec.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_SISO : virtual public Codec<B,Q>
{
private:
	std::shared_ptr<Decoder_SISO<Q>> decoder_siso;

public:
	Codec_SISO(const int K, const int N_cw, const int N, const int tail_length = 0, const int n_frames = 1);

	virtual ~Codec_SISO() = default;

	const std::shared_ptr<Decoder_SISO<Q>>& get_decoder_siso();

	virtual void reset();

protected:
	void set_decoder_siso(std::shared_ptr<Decoder_SISO<Q>> dec);
	void set_decoder_siso(Decoder_SISO<Q>* dec);
};
}
}
#include "Module/Codec/Codec_SISO.hxx"

#endif /* CODEC_SISO_HPP_ */
