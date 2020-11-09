/*!
 * \file
 * \brief Class module::Decoder_turbo.
 */
#ifndef DECODER_TURBO_HPP_
#define DECODER_TURBO_HPP_

#include <vector>
#include <memory>
#include <mipp.h>

#include "Tools/Code/Turbo/Post_processing_SISO/Post_processing_SISO.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_turbo : public Decoder_SIHO<B,R>
{
protected:
	const int  n_ite; // number of iterations
	const bool buffered_encoding;

	std::shared_ptr<Interleaver<R>> pi;
	std::shared_ptr<Decoder_SISO<B,R>> siso_n;
	std::shared_ptr<Decoder_SISO<B,R>> siso_i;

	mipp::vector<R> l_sn;  // systematic LLRs                  in the natural     domain
	mipp::vector<R> l_si;  // systematic LLRs                  in the interleaved domain
	mipp::vector<R> l_sen; // systematic LLRs + extrinsic LLRs in the natural     domain
	mipp::vector<R> l_sei; // systematic LLRs + extrinsic LLRs in the interleaved domain
	mipp::vector<R> l_pn;  // parity     LLRs                  in the natural     domain
	mipp::vector<R> l_pi;  // parity     LLRs                  in the interleaved domain
	mipp::vector<R> l_e1n; // extrinsic  LLRs                  in the natural     domain
	mipp::vector<R> l_e2n; // extrinsic  LLRs                  in the natural     domain
	mipp::vector<R> l_e1i; // extrinsic  LLRs                  in the interleaved domain
	mipp::vector<R> l_e2i; // extrinsic  LLRs                  in the interleaved domain
	mipp::vector<B> s;     // bit decision

	std::vector<std::shared_ptr<tools::Post_processing_SISO<B,R>>> post_processings;

public:
	Decoder_turbo(const int& K,
	              const int& N,
	              const int& n_ite,
	              const Decoder_SISO<B,R> &siso_n,
	              const Decoder_SISO<B,R> &siso_i,
	              const Interleaver<R> &pi,
	              const bool buffered_encoding = true);

	virtual ~Decoder_turbo() = default;

	virtual Decoder_turbo<B,R>* clone() const;

	void add_post_processing(const tools::Post_processing_SISO<B,R> &post_processing);

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual void deep_copy(const Decoder_turbo<B,R> &m);
	virtual void _load (const R *Y_N, const size_t frame_id);
	virtual void _store(      B *V_K                       ) const;

private:
	void buffered_load(const R *Y_N, const size_t frame_id);
	void standard_load(const R *Y_N, const size_t frame_id);
};
}
}

#endif /* DECODER_TURBO_HPP_ */
