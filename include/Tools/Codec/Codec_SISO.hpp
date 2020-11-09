/*!
 * \file
 * \brief Class tools::Codec_SISO.
 */
#ifndef CODEC_SISO_HPP_
#define CODEC_SISO_HPP_

#include <memory>

#include "Module/Decoder/Decoder_SISO.hpp"
#include "Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
class Codec_SISO : public Codec_SIHO<B,Q>
{
protected:
	std::shared_ptr<module::Decoder_SISO<B,Q>> decoder_siso;

public:
	Codec_SISO(const int K, const int N_cw, const int N);

	virtual ~Codec_SISO() = default;

	virtual Codec_SISO<B,Q>* clone() const;

	module::Decoder_SISO<B,Q>& get_decoder_siso();

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual void deep_copy(const Codec_SISO<B,Q> &t);
	void set_decoder_siso(module::Decoder_SISO<B,Q>* dec);
	void set_decoder_siso(std::shared_ptr<module::Decoder_SISO<B,Q>> dec);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Codec/Codec_SISO.hxx"
#endif

#endif /* CODEC_SISO_HPP_ */
