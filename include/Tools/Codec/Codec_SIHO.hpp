/*!
 * \file
 * \brief Class tools::Codec_SIHO.
 */
#ifndef CODEC_SIHO_HPP_
#define CODEC_SIHO_HPP_

#include <memory>

#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Tools/Codec/Codec.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_SIHO : virtual public Codec<B,Q>
{
private:
	std::shared_ptr<module::Decoder_SIHO<B,Q>> decoder_siho;

public:
	Codec_SIHO(const int K, const int N_cw, const int N, const int tail_length = 0, const int n_frames = 1);

	virtual ~Codec_SIHO() = default;

	module::Decoder_SIHO<B,Q>& get_decoder_siho();

protected:
	virtual void set_decoder_siho(std::shared_ptr<module::Decoder_SIHO<B,Q>> dec);
	virtual void set_decoder_siho(module::Decoder_SIHO<B,Q>* dec);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Codec/Codec_SIHO.hxx"
#endif

#endif /* CODEC_SIHO_HPP_ */
