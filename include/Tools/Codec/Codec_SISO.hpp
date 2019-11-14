/*!
 * \file
 * \brief Class tools::Codec_SISO.
 */
#ifndef CODEC_SISO_HPP_
#define CODEC_SISO_HPP_

#include <memory>

#include "Module/Decoder/Decoder_SISO.hpp"
#include "Tools/Codec/Codec.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_SISO : virtual public Codec<B,Q>
{
private:
	std::shared_ptr<module::Decoder_SISO<Q>> decoder_siso;

public:
	Codec_SISO(const int K, const int N_cw, const int N, const int tail_length = 0, const int n_frames = 1);

	virtual ~Codec_SISO() = default;

	module::Decoder_SISO<Q>& get_decoder_siso();

protected:
	void set_decoder_siso(std::shared_ptr<module::Decoder_SISO<Q>> dec);
	void set_decoder_siso(module::Decoder_SISO<Q>* dec);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Codec/Codec_SISO.hxx"
#endif

#endif /* CODEC_SISO_HPP_ */
