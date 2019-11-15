/*!
 * \file
 * \brief Class tools::Codec.
 */
#ifndef CODEC_HPP_
#define CODEC_HPP_

#include <cstdint>
#include <cstddef>
#include <vector>
#include <memory>

#include "Tools/Interleaver/Interleaver_core.hpp"
#include "Tools/Noise/Noise.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Puncturer/Puncturer.hpp"
#include "Module/Extractor/Extractor.hpp"

namespace aff3ct
{
namespace tools
{

template <typename B = int, typename Q = float>
class Codec
{
private:
	std::unique_ptr<Interleaver_core   < >> interleaver_core;
	std::unique_ptr<module::Interleaver<B>> interleaver_bit;
	std::unique_ptr<module::Interleaver<Q>> interleaver_llr;

	std::unique_ptr<module::Encoder  <B  >> encoder;
	std::unique_ptr<module::Puncturer<B,Q>> puncturer;
	std::unique_ptr<module::Extractor<B,Q>> extractor;

protected :
	const int K;
	const int N_cw;
	const int N;
	const Noise<> *noise;

public:
	Codec(const int K, const int N_cw, const int N, const int n_frames = 1);

	virtual ~Codec() = default;

	Interleaver_core<>& get_interleaver();
	module::Encoder<B>& get_encoder();
	module::Puncturer<B,Q>& get_puncturer();
	module::Extractor<B,Q>& get_extractor();

	const Noise<>& get_noise() const;

	virtual void set_noise(const Noise<>& noise);

	virtual void noise_changed();

protected:
	virtual void check_noise();

	virtual void set_interleaver(std::unique_ptr<Interleaver_core<>> &&itl);
	virtual void set_encoder    (std::unique_ptr<module::Encoder<B>> &&enc);
	virtual void set_puncturer  (std::unique_ptr<module::Puncturer<B,Q>> &&pct);
	virtual void set_extractor  (std::unique_ptr<module::Extractor<B,Q>> &&pct);

	virtual void set_interleaver(Interleaver_core<>* itl);
	virtual void set_encoder    (module::Encoder<B>* enc);
	virtual void set_puncturer  (module::Puncturer<B,Q>* pct);
	virtual void set_extractor  (module::Extractor<B,Q>* pct);

	virtual const module::Interleaver<B>& get_interleaver_bit();
	virtual const module::Interleaver<Q>& get_interleaver_llr();
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Codec/Codec.hxx"
#endif

#endif /* CODEC_HPP_ */
