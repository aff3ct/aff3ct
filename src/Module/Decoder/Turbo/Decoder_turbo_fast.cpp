#include <vector>
#include <string>
#include <algorithm>

#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Perf/Reorderer/Reorderer.hpp"
#include "Module/Decoder/Turbo/Decoder_turbo_fast.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_turbo_fast<B,R>
::Decoder_turbo_fast(const int& K,
                     const int& N,
                     const int& n_ite,
                     const Decoder_SISO<B,R> &siso_n,
                     const Decoder_SISO<B,R> &siso_i,
                     const Interleaver<R> &pi,
                     const bool buffered_encoding)
: Decoder_turbo<B,R>(K, N, n_ite, siso_n, siso_i, pi, buffered_encoding)
{
	const std::string name = "Decoder_turbo_fast";
	this->set_name(name);
}

template <typename B, typename R>
void Decoder_turbo_fast<B,R>
::_load(const R *Y_N, const size_t frame_id)
{
	if (this->buffered_encoding && this->get_n_frames_per_wave() > 1)
	{
		const auto tail_n = this->siso_n->tail_length();
		const auto tail_i = this->siso_i->tail_length();

		if (this->get_n_frames_per_wave() == (size_t)mipp::nElReg<B>())
		{
			constexpr auto n_frames = mipp::nElReg<R>();

			std::vector<const R*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*this->N;
			tools::Reorderer_static<R,n_frames>::apply(frames, this->l_sn.data(), this->siso_n->get_K() + tail_n/2);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*this->N + this->siso_n->get_K() + tail_n/2;
			tools::Reorderer_static<R,n_frames>::apply(frames, this->l_pn.data(), this->siso_n->get_K() + tail_n/2);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*this->N + this->siso_n->get_N();
			tools::Reorderer_static<R,n_frames>::apply(frames, &this->l_si[this->K*n_frames], tail_i/2);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*this->N + this->siso_n->get_N() + tail_i/2;
			tools::Reorderer_static<R,n_frames>::apply(frames, this->l_pi.data(), this->siso_i->get_K() + tail_i/2);

			this->pi->interleave_reordering(this->l_sn.data(), this->l_si.data(), frame_id, false);
		}
		else
		{
			constexpr auto n_frames = mipp::nElReg<R>() / 8;

			std::vector<const R*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*this->N;
			tools::Reorderer_static<R,n_frames>::apply(frames, this->l_sn.data(), this->siso_n->get_K() + tail_n/2);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*this->N + this->siso_n->get_K() + tail_n/2;
			tools::Reorderer_static<R,n_frames>::apply(frames, this->l_pn.data(), this->siso_n->get_K() + tail_n/2);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*this->N + this->siso_n->get_N();
			tools::Reorderer_static<R,n_frames>::apply(frames, &this->l_si[this->K*n_frames], tail_i/2);

			for (auto f = 0; f < n_frames; f++)
				frames[f] = Y_N + f*this->N + this->siso_n->get_N() + tail_i/2;
			tools::Reorderer_static<R,n_frames>::apply(frames, this->l_pi.data(), this->siso_i->get_K() + tail_i/2);

			this->pi->interleave_reordering(this->l_sn.data(), this->l_si.data(), frame_id, false);
		}

		std::fill(this->l_e1n.begin(), this->l_e1n.end(), (R)0);
	}
	else
		Decoder_turbo<B,R>::_load(Y_N, frame_id);
}

template <typename B, typename R>
Decoder_turbo_fast<B,R>* Decoder_turbo_fast<B,R>
::clone() const
{
	auto m = new Decoder_turbo_fast(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
int Decoder_turbo_fast<B,R>
::_decode_siho(const R *Y_N, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N, frame_id);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	const auto n_frames = this->get_n_frames_per_wave();
	const auto tail_n_2 = this->siso_n->tail_length() / 2;
	const auto tail_i_2 = this->siso_i->tail_length() / 2;

	// iterative turbo decoding process
	bool stop = false;
	auto ite  = 1;
	do
	{
		// l_se = sys + ext
		for (size_t i = 0; i < this->K * n_frames; i += mipp::nElReg<R>())
		{
			const auto r_l_sen = mipp::Reg<R>(&this->l_sn[i]) + mipp::Reg<R>(&this->l_e1n[i]);
			r_l_sen.store(&this->l_sen[i]);
		}
		std::copy(this->l_sn .begin() +  this->K             * n_frames,
		          this->l_sn .begin() + (this->K + tail_n_2) * n_frames,
		          this->l_sen.begin() +  this->K             * n_frames);

		// SISO in the natural domain
		this->siso_n->decode_siso_alt(this->l_sen.data(), this->l_pn.data(), this->l_e2n.data(), frame_id, false);

		for (auto &pp : this->post_processings)
		{
			stop = pp->siso_n(ite, this->l_sen, this->l_e2n, this->s);
			if (stop) break;
		}

		if (!stop)
		{
			// make the interleaving
			if (n_frames > 1)
				this->pi->interleave_reordering(this->l_e2n.data(), this->l_e1i.data(), frame_id, false);
			else
				this->pi->interleave(this->l_e2n.data(), this->l_e1i.data(), frame_id, false);

			// l_se = sys + ext
			for (size_t i = 0; i < this->K * n_frames; i += mipp::nElReg<R>())
			{
				const auto r_l_sei = mipp::Reg<R>(&this->l_si[i]) + mipp::Reg<R>(&this->l_e1i[i]);
				r_l_sei.store(&this->l_sei[i]);
			}
			std::copy(this->l_si .begin() +  this->K             * n_frames,
			          this->l_si .begin() + (this->K + tail_i_2) * n_frames,
			          this->l_sei.begin() +  this->K             * n_frames);

			// SISO in the interleave domain
			this->siso_i->decode_siso_alt(this->l_sei.data(), this->l_pi.data(), this->l_e2i.data(), frame_id, false);

			for (auto &pp : this->post_processings)
			{
				stop = pp->siso_i(ite, this->l_sei, this->l_e2i);
				if (stop) break;
			}

			if (ite == this->n_ite || stop)
				// add the systematic information to the extrinsic information, gives the a posteriori information
				for (size_t i = 0; i < this->K * n_frames; i += mipp::nElReg<R>())
				{
					const auto r_post = mipp::Reg<R>(&this->l_e2i[i]) + mipp::Reg<R>(&this->l_sei[i]);
					r_post.store(&this->l_e2i[i]);
				}

			// make the deinterleaving
			if (n_frames > 1)
				this->pi->deinterleave_reordering(this->l_e2i.data(), this->l_e1n.data(), frame_id, false);
			else
				this->pi->deinterleave(this->l_e2i.data(), this->l_e1n.data(), frame_id, false);

			// compute the hard decision only if we are in the last iteration
			if (ite == this->n_ite || stop)
				tools::hard_decide(this->l_e1n.data(), this->s.data(), this->K * n_frames);
		}

		ite++; // increment the number of iteration
	}
	while ((ite <= this->n_ite) && !stop);

	for (auto &pp : this->post_processings)
		pp->end(ite -1);

//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store(V_K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::total,  d_load + d_decod + d_store);

	return 0;
}

template <typename B, typename R>
void Decoder_turbo_fast<B,R>
::_store(B *V_K) const
{
	if (this->get_n_frames_per_wave() > 1)
	{
		if (this->get_n_frames_per_wave() == (size_t)mipp::nElReg<B>())
		{
			constexpr auto n_frames = mipp::nElReg<B>();

			std::vector<B*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = V_K + f*this->K;
			tools::Reorderer_static<B,n_frames>::apply_rev(this->s.data(), frames, this->K);
		}
		else
		{
			constexpr auto n_frames = mipp::nElReg<B>() / 8;

			std::vector<B*> frames(n_frames);
			for (auto f = 0; f < n_frames; f++)
				frames[f] = V_K + f*this->K;
			tools::Reorderer_static<B,n_frames>::apply_rev(this->s.data(), frames, this->K);
		}
	}
	else
		Decoder_turbo<B,R>::_store(V_K);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_turbo_fast<B_8,Q_8>;
template class aff3ct::module::Decoder_turbo_fast<B_16,Q_16>;
template class aff3ct::module::Decoder_turbo_fast<B_32,Q_32>;
template class aff3ct::module::Decoder_turbo_fast<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_turbo_fast<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
