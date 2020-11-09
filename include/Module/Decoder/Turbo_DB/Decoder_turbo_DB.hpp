/*!
 * \file
 * \brief Class module::Decoder_turbo_DB.
 */
#ifndef DECODER_TURBO_DB_HPP
#define DECODER_TURBO_DB_HPP

#include <memory>
#include <vector>
#include <mipp.h>

#include "Tools/Code/Turbo/Post_processing_SISO/Post_processing_SISO.hpp"
#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Decoder/RSC_DB/BCJR/Decoder_RSC_DB_BCJR.hpp"

#include "Module/Decoder/Decoder_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_turbo_DB : public Decoder_SIHO<B,R>
{
protected:
	const int n_ite; // number of iterations

	std::shared_ptr<Interleaver<R>> pi;
	std::shared_ptr<Decoder_RSC_DB_BCJR<B,R>> siso_n;
	std::shared_ptr<Decoder_RSC_DB_BCJR<B,R>> siso_i;

	mipp::vector<R> l_cpy;
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

	virtual void set_n_frames(const size_t n_frames);

public:
	Decoder_turbo_DB(const int& K,
	                 const int& N,
	                 const int& n_ite,
	                 const Decoder_RSC_DB_BCJR<B,R> &siso_n,
	                 const Decoder_RSC_DB_BCJR<B,R> &siso_i,
	                 const Interleaver<R> &pi);
	virtual ~Decoder_turbo_DB() = default;
	virtual Decoder_turbo_DB<B,R>* clone() const;

	void add_post_processing(const tools::Post_processing_SISO<B,R> &post_processing);

protected:
	virtual void deep_copy(const Decoder_turbo_DB<B,R> &m);
	virtual int  _decode_siho(const R *Y_N, B *V_K, const size_t frame_id);
	virtual void _load       (const R *Y_N                               );
	virtual void _store      (              B *V_K                       ) const;
};
}
}

#endif /* DECODER_TURBO_DB_HPP */
