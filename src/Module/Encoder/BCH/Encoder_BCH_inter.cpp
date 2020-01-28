#include <numeric>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/Reorderer/Reorderer.hpp"
#include "Module/Encoder/BCH/Encoder_BCH_inter.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_BCH_inter<B>
::Encoder_BCH_inter(const int& K, const int& N, const tools::BCH_polynomial_generator<B>& GF_poly, const int n_frames)
: Encoder_BCH<B>(K, N, GF_poly, true, n_frames, mipp::N<B>()),
  U_K_reordered(this->K       * mipp::N<B>()),
  par_reordered(this->n_rdncy * mipp::N<B>())
{
	const std::string name = "Encoder_BCH_inter";
	this->set_name(name);
}

template <typename B>
Encoder_BCH_inter<B>* Encoder_BCH_inter<B>
::clone() const
{
	auto m = new Encoder_BCH_inter(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_BCH_inter<B>
::__encode(const B *U_K, B *par)
{
	mipp::Reg<B> reg_g_0 = this->g[0];
	mipp::Reg<B> reg_zero = (B)0;

	std::vector<mipp::Reg<B>> regs_par(this->n_rdncy, mipp::Reg<B>((B)0));

	for (auto i = this->K - 1; i >= 0; i--)
	{
		const auto reg_feedback = regs_par[this->n_rdncy -1] ^ &U_K[i * mipp::N<B>()];
		for (auto j = this->n_rdncy -1; j > 0; j--)
			regs_par[j] = this->g[j] ? regs_par[j -1] ^ reg_feedback : regs_par[j -1];
		regs_par[0] = mipp::blend(reg_g_0 & reg_feedback, reg_zero, reg_feedback == 1);
	}

	for (size_t p = 0; p < regs_par.size(); p++)
		regs_par[p].store(&par[p * mipp::N<B>()]);
}

template <typename B>
void Encoder_BCH_inter<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	constexpr int n_frames = mipp::N<B>();
	std::vector<const B*> frames_in(n_frames);
		for (auto f = 0; f < n_frames; f++)
			frames_in[f] = U_K + f*this->K;

	tools::Reorderer_static<B,n_frames>::apply(frames_in, this->U_K_reordered.data(), this->K);

	// generate the parity bits
	this->__encode(this->U_K_reordered.data(), par_reordered.data());

	std::vector<B*> frames_out(n_frames);
	for (auto f = 0; f < n_frames; f++)
		frames_out[f] = X_N + f * this->N;
	tools::Reorderer_static<B,n_frames>::apply_rev(this->par_reordered.data(), frames_out, this->n_rdncy);

	// copy the sys bits at the end of the codeword
	for (auto f = 0; f < n_frames; f++)
		std::copy(U_K + (f +0) * this->K,
		          U_K + (f +1) * this->K,
	 	          X_N + (f +0) * this->N + this->n_rdncy);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_BCH_inter<B_8>;
template class aff3ct::module::Encoder_BCH_inter<B_16>;
template class aff3ct::module::Encoder_BCH_inter<B_32>;
template class aff3ct::module::Encoder_BCH_inter<B_64>;
#else
template class aff3ct::module::Encoder_BCH_inter<B>;
#endif
// ==================================================================================== explicit template instantiation
