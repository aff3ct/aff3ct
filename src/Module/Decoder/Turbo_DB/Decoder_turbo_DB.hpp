#ifndef DECODER_TURBO_DB_HPP
#define DECODER_TURBO_DB_HPP

#include <functional>
#include <vector>

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
	const int                 n_ite; // number of iterations

	const Interleaver  <R>   &pi;
	Decoder_RSC_DB_BCJR<B,R> &siso_n;
	Decoder_RSC_DB_BCJR<B,R> &siso_i;

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

	std::vector<std::function<bool(const int ite,
	                               const mipp::vector<R>& sys,
	                                     mipp::vector<R>& ext,
	                                     mipp::vector<B>& s)>> callbacks_siso_n;
	std::vector<std::function<bool(const int ite,
	                               const mipp::vector<R>& sys,
	                                     mipp::vector<R>& ext)>> callbacks_siso_i;
	std::vector<std::function<void(const int n_ite)>> callbacks_end;

public:
	Decoder_turbo_DB(const int& K,
	                 const int& N,
	                 const int& n_ite,
	                 const Interleaver<R> &pi,
	                 Decoder_RSC_DB_BCJR<B,R> &siso_n,
	                 Decoder_RSC_DB_BCJR<B,R> &siso_i);
	virtual ~Decoder_turbo_DB() = default;

	void add_handler_siso_n(std::function<bool(const int,
	                                           const mipp::vector<R>&,
	                                                 mipp::vector<R>&,
	                                                 mipp::vector<B>&)> callback);
	void add_handler_siso_i(std::function<bool(const int,
	                                           const mipp::vector<R>&,
	                                                 mipp::vector<R>&)> callback);
	void add_handler_end(std::function<void(const int)> callback);

protected:
	virtual void _decode_siho(const R *Y_N, B *V_K, const int frame_id);
	virtual void _load       (const R *Y_N                            );
	virtual void _store      (              B *V_K                    ) const;
};
}
}

#endif /* DECODER_TURBO_DB_HPP */
