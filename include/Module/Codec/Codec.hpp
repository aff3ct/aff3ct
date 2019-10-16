#ifndef CODEC_HPP_
#define CODEC_HPP_

#include <cstdint>
#include <vector>
#include <memory>

#include "Tools/Interleaver/Interleaver_core.hpp"
#include "Tools/Noise/Noise.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Puncturer/Puncturer.hpp"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace cdc
	{
		enum class tsk : uint8_t { extract_sys_llr, extract_sys_bit, extract_sys_par, add_sys_ext, SIZE };

		namespace sck
		{
			enum class extract_sys_llr : uint8_t { Y_N, Y_K     , SIZE };
			enum class extract_sys_bit : uint8_t { Y_N, V_K     , SIZE };
			enum class extract_sys_par : uint8_t { Y_N, sys, par, SIZE };
			enum class add_sys_ext     : uint8_t { ext, Y_N     , SIZE };
		}
	}

template <typename B = int, typename Q = float>
class Codec : public Module
{
public:
	inline Task&   operator[](const cdc::tsk                  t) { return Module::operator[]((int)t);                                 }
	inline Socket& operator[](const cdc::sck::extract_sys_llr s) { return Module::operator[]((int)cdc::tsk::extract_sys_llr)[(int)s]; }
	inline Socket& operator[](const cdc::sck::extract_sys_bit s) { return Module::operator[]((int)cdc::tsk::extract_sys_bit)[(int)s]; }
	inline Socket& operator[](const cdc::sck::extract_sys_par s) { return Module::operator[]((int)cdc::tsk::extract_sys_par)[(int)s]; }
	inline Socket& operator[](const cdc::sck::add_sys_ext     s) { return Module::operator[]((int)cdc::tsk::add_sys_ext    )[(int)s]; }

private:
	std::unique_ptr<tools::Interleaver_core< >> interleaver_core;
	std::unique_ptr<       Interleaver     <B>> interleaver_bit;
	std::unique_ptr<       Interleaver     <Q>> interleaver_llr;

	std::unique_ptr<Encoder  <B  >> encoder;
	std::unique_ptr<Puncturer<B,Q>> puncturer;

protected :
	const int K;
	const int N_cw;
	const int N;
	const int tail_length;
	std::unique_ptr<tools::Noise<float>> n;

public:
	Codec(const int K, const int N_cw, const int N, const int tail_length = 0, const int n_frames = 1);

	virtual ~Codec() = default;

	std::unique_ptr<tools::Interleaver_core<>>& get_interleaver();

	std::unique_ptr<Encoder<B>>& get_encoder();

	std::unique_ptr<Puncturer<B,Q>>& get_puncturer();

	const tools::Noise<float>& current_noise() const;

	virtual void set_noise(const tools::Noise<float>& noise);
	virtual void set_noise(const tools::Noise<double>& noise);

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
	virtual void _extract_sys_llr(const Q *Y_N, Q *Y_K,         const int frame_id);
	virtual void _extract_sys_bit(const Q *Y_N, B *V_K,         const int frame_id);
	virtual void _extract_sys_par(const Q *Y_N, Q *sys, Q *par, const int frame_id);
	virtual void _add_sys_ext    (const Q *ext, Q *Y_N,         const int frame_id);

	virtual void set_interleaver(std::unique_ptr<tools::Interleaver_core<>> &&itl);
	virtual void set_encoder    (std::unique_ptr<Encoder<B>>                &&enc);
	virtual void set_puncturer  (std::unique_ptr<Puncturer<B,Q>>            &&pct);

	virtual void set_interleaver(tools::Interleaver_core<>* itl);
	virtual void set_encoder    (Encoder<B>*                enc);
	virtual void set_puncturer  (Puncturer<B,Q>*            pct);

	virtual const Interleaver<B>& get_interleaver_bit();
	virtual const Interleaver<Q>& get_interleaver_llr();
};
}
}

#include "Module/Codec/Codec.hxx"

#endif /* CODEC_HPP_ */
