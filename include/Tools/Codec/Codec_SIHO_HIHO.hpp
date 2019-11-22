/*!
 * \file
 * \brief Class tools::Codec_SIHO_HIHO.
 */
#ifndef CODEC_SIHO_HIHO_HPP_
#define CODEC_SIHO_HIHO_HPP_

#include <memory>

#include "Module/Decoder/Decoder_SIHO_HIHO.hpp"
#include "Tools/Codec/Codec_HIHO.hpp"
#include "Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_SIHO_HIHO : public Codec_SIHO<B,Q>, public Codec_HIHO<B,Q>
{
public:
	Codec_SIHO_HIHO(const int K, const int N_cw, const int N, const int n_frames = 1);

	virtual ~Codec_SIHO_HIHO() = default;

	virtual Codec_SIHO_HIHO<B,Q>* clone() const;

	virtual void set_decoder_siho_hiho(std::shared_ptr<module::Decoder_SIHO_HIHO<B,Q>> dec);
	virtual void set_decoder_siho_hiho(module::Decoder_SIHO_HIHO<B,Q>* dec);

protected:
	virtual void deep_copy(const Codec_SIHO_HIHO<B,Q> &t);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Codec/Codec_SIHO_HIHO.hxx"
#endif

#endif /* CODEC_SIHO_HIHO_HPP_ */
