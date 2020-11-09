/*!
 * \file
 * \brief Class tools::Codec_SIHO.
 */
#ifndef CODEC_SIHO_HPP_
#define CODEC_SIHO_HPP_

#include <memory>

#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Tools/Codec/Codec_HIHO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_SIHO : public Codec_HIHO<B,Q>
{
protected:
	std::shared_ptr<module::Decoder_SIHO<B,Q>> decoder_siho;

public:
	Codec_SIHO(const int K, const int N_cw, const int N);

	virtual ~Codec_SIHO() = default;

	virtual Codec_SIHO<B,Q>* clone() const;

	module::Decoder_SIHO<B,Q>& get_decoder_siho();

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual void deep_copy(const Codec_SIHO<B,Q> &t);
	void set_decoder_siho(module::Decoder_SIHO<B,Q>* dec);
	void set_decoder_siho(std::shared_ptr<module::Decoder_SIHO<B,Q>> dec);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Codec/Codec_SIHO.hxx"
#endif

#endif /* CODEC_SIHO_HPP_ */
