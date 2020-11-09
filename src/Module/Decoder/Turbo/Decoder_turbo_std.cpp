#include <string>

#include "Tools/Perf/common/hard_decide.h"
#include "Module/Decoder/Turbo/Decoder_turbo_std.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_turbo_std<B,R>
::Decoder_turbo_std(const int& K,
                    const int& N,
                    const int& n_ite,
                    const Decoder_SISO<B,R> &siso_n,
                    const Decoder_SISO<B,R> &siso_i,
                    const Interleaver<R> &pi,
                    const bool buffered_encoding)
: Decoder_turbo<B,R>(K, N, n_ite, siso_n, siso_i, pi, buffered_encoding)
{
	const std::string name = "Decoder_turbo_std";
	this->set_name(name);
}

template <typename B, typename R>
Decoder_turbo_std<B,R>* Decoder_turbo_std<B,R>
::clone() const
{
	auto m = new Decoder_turbo_std(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
int Decoder_turbo_std<B,R>
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
		// sys + ext
		for (size_t i = 0; i < this->K * n_frames; i++)
			this->l_sen[i] = this->l_sn[i] + this->l_e1n[i];

		for (auto i = this->K * n_frames; i < (this->K + tail_n_2) * n_frames; i++)
			this->l_sen[i] = this->l_sn[i];

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

			// sys + ext
			for (size_t i = 0; i < this->K * n_frames; i++)
				this->l_sei[i] = this->l_si[i] + this->l_e1i[i];

			for (auto i = this->K * n_frames; i < (this->K + tail_i_2) * n_frames; i++)
				this->l_sei[i] = this->l_si[i];

			// SISO in the interleave domain
			this->siso_i->decode_siso_alt(this->l_sei.data(), this->l_pi.data(), this->l_e2i.data(), frame_id, false);

			for (auto &pp : this->post_processings)
			{
				stop = pp->siso_i(ite, this->l_sei, this->l_e2i);
				if (stop) break;
			}

			if (ite == this->n_ite || stop)
				// add the systematic information to the extrinsic information, gives the a posteriori information
				for (size_t i = 0; i < this->K * n_frames; i++)
					this->l_e2i[i] += this->l_sei[i];

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

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_turbo_std<B_8,Q_8>;
template class aff3ct::module::Decoder_turbo_std<B_16,Q_16>;
template class aff3ct::module::Decoder_turbo_std<B_32,Q_32>;
template class aff3ct::module::Decoder_turbo_std<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_turbo_std<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
