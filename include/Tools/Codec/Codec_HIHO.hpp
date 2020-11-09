/*!
 * \file
 * \brief Class tools::Codec_HIHO.
 */
#ifndef CODEC_HIHO_HPP_
#define CODEC_HIHO_HPP_

#include <memory>

#include "Module/Decoder/Decoder_HIHO.hpp"
#include "Tools/Codec/Codec.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_HIHO : public Codec<B,Q>
{
protected:
	std::shared_ptr<module::Decoder_HIHO<B>> decoder_hiho;

public:
	Codec_HIHO(const int K, const int N_cw, const int N);

	virtual ~Codec_HIHO() = default;

	virtual Codec_HIHO<B,Q>* clone() const;

	virtual module::Decoder_HIHO<B>& get_decoder_hiho();

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual void deep_copy(const Codec_HIHO<B,Q> &t);
	void set_decoder_hiho(module::Decoder_HIHO<B>* dec);
	void set_decoder_hiho(std::shared_ptr<module::Decoder_HIHO<B>> dec);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Codec/Codec_HIHO.hxx"
#endif

#endif /* CODEC_HIHO_HPP_ */
