#ifndef CODEC_HPP_
#define CODEC_HPP_

#include "Tools/Interleaver/Interleaver_core.hpp"
#include "Tools/Noise.hpp"

#include "Factory/Module/Interleaver/Interleaver.hpp"

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Puncturer/Puncturer.hpp"

#include "../Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace cdc
	{
		namespace tsk
		{
			enum list { extract_sys_llr, extract_sys_bit, extract_sys_par, add_sys_ext, SIZE };
		}

		namespace sck
		{
			namespace extract_sys_llr { enum list { Y_N, Y_K     , SIZE }; }
			namespace extract_sys_bit { enum list { Y_N, V_K     , SIZE }; }
			namespace extract_sys_par { enum list { Y_N, sys, par, SIZE }; }
			namespace add_sys_ext     { enum list { ext, Y_N     , SIZE }; }
		}
	}

template <typename B = int, typename Q = float>
class Codec : public Module
{
private:
	tools::Interleaver_core< > *interleaver_core;
	       Interleaver     <B> *interleaver_bit;
	       Interleaver     <Q> *interleaver_llr;

	Encoder  <B  > *encoder;
	Puncturer<B,Q> *puncturer;

protected :
	const int K;
	const int N_cw;
	const int N;
	const int tail_length;
	// tools::Noise<float> n;

public:
	Codec(const int K, const int N_cw, const int N, const int tail_length = 0, const int n_frames = 1);

	virtual ~Codec();

	virtual tools::Interleaver_core<>* get_interleaver();

	virtual Encoder<B>* get_encoder();

	virtual Puncturer<B,Q>* get_puncturer();

	const tools::Noise<float>& get_noise();

	virtual void set_noise(const tools::Noise<float>& noise);
	void set_noise(const tools::Noise<double>& noise);

	template <class A = std::allocator<Q>>
	void extract_sys_llr(const std::vector<Q,A> &Y_N, std::vector<Q,A> &Y_K, const int frame_id = -1);

	virtual void extract_sys_llr(const Q *Y_N, Q *Y_K, const int frame_id = -1);

	template <class AQ = std::allocator<Q>, class AB = std::allocator<B>>
	void extract_sys_bit(const std::vector<Q,AQ> &Y_N, std::vector<B,AB> &V_K, const int frame_id = -1);

	virtual void extract_sys_bit(const Q *Y_N, B *V_K, const int frame_id = -1);

	template <class A = std::allocator<Q>>
	void extract_sys_par(const std::vector<Q,A> &Y_N, std::vector<Q,A> &sys, std::vector<Q,A> &par,
	                     const int frame_id = -1);

	virtual void extract_sys_par(const Q *Y_N, Q *sys, Q *par, const int frame_id = -1);

	template <class A = std::allocator<Q>>
	void add_sys_ext(const std::vector<Q,A> &ext, std::vector<Q,A> &Y_N, const int frame_id = -1);

	virtual void add_sys_ext(const Q *ext, Q *Y_N, const int frame_id = -1);

	virtual void reset();

protected:
	virtual void _extract_sys_llr(const Q *Y_N, Q *Y_K, const int frame_id);

	virtual void _extract_sys_bit(const Q *Y_N, B *V_K, const int frame_id);

	virtual void _extract_sys_par(const Q *Y_N, Q *sys, Q *par, const int frame_id);

	virtual void _add_sys_ext(const Q *ext, Q *Y_N, const int frame_id);

	virtual void set_interleaver(tools::Interleaver_core<>* itl);

	virtual void set_encoder(Encoder<B>* enc);

	virtual void set_puncturer(Puncturer<B,Q>* pct);

	virtual const Interleaver<B>& get_interleaver_bit();

	virtual const Interleaver<Q>& get_interleaver_llr();
};
}
}

#include "Codec.hxx"

#endif /* CODEC_HPP_ */
