/*!
 * \file
 * \brief Class module::Codec.
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
#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace cdc
	{
		enum class tsk : size_t { extract_sys_llr, extract_sys_bit, extract_sys_par, add_sys_ext, SIZE };

		namespace sck
		{
			enum class extract_sys_llr : size_t { Y_N, Y_K     , SIZE };
			enum class extract_sys_bit : size_t { Y_N, V_K     , SIZE };
			enum class extract_sys_par : size_t { Y_N, sys, par, SIZE };
			enum class add_sys_ext     : size_t { ext, Y_N     , SIZE };
		}
	}

template <typename B = int, typename Q = float>
class Codec : public Module
{
public:
	inline Task&   operator[](const cdc::tsk                  t);
	inline Socket& operator[](const cdc::sck::extract_sys_llr s);
	inline Socket& operator[](const cdc::sck::extract_sys_bit s);
	inline Socket& operator[](const cdc::sck::extract_sys_par s);
	inline Socket& operator[](const cdc::sck::add_sys_ext     s);

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

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Codec/Codec.hxx"
#endif

#endif /* CODEC_HPP_ */
