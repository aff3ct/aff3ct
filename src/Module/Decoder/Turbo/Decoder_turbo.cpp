#include <string>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/Reorderer/Reorderer.hpp"
#include "Module/Decoder/Turbo/Decoder_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_turbo<B,R>
::Decoder_turbo(const int& K,
                const int& N,
                const int& n_ite,
                const Decoder_SISO<B,R> &siso_n,
                const Decoder_SISO<B,R> &siso_i,
                const Interleaver<R> &pi,
                const bool buffered_encoding)
: Decoder_SIHO<B,R>(K, N),
  n_ite(n_ite),
  buffered_encoding(buffered_encoding),
  pi(pi.clone()),
  siso_n(dynamic_cast<Decoder_SISO<B,R>*>(siso_n.clone())),
  siso_i(dynamic_cast<Decoder_SISO<B,R>*>(siso_i.clone())),
  l_sn ((K                                                        + (siso_n.tail_length() / 2)) * siso_n.get_n_frames_per_wave() + mipp::nElReg<R>()),
  l_si ((K                                                        + (siso_i.tail_length() / 2)) * siso_i.get_n_frames_per_wave() + mipp::nElReg<R>()),
  l_sen((K                                                        + (siso_n.tail_length() / 2)) * siso_n.get_n_frames_per_wave() + mipp::nElReg<R>()),
  l_sei((K                                                        + (siso_i.tail_length() / 2)) * siso_i.get_n_frames_per_wave() + mipp::nElReg<R>()),
  l_pn (((N - (siso_n.tail_length() + siso_i.tail_length()) -K)/2 + (siso_n.tail_length() / 2)) * siso_n.get_n_frames_per_wave() + mipp::nElReg<R>()),
  l_pi (((N - (siso_n.tail_length() + siso_i.tail_length()) -K)/2 + (siso_i.tail_length() / 2)) * siso_i.get_n_frames_per_wave() + mipp::nElReg<R>()),
  l_e1n((K                                                                                    ) * siso_n.get_n_frames_per_wave() + mipp::nElReg<R>()),
  l_e2n((K                                                                                    ) * siso_n.get_n_frames_per_wave() + mipp::nElReg<R>()),
  l_e1i((K                                                                                    ) * siso_i.get_n_frames_per_wave() + mipp::nElReg<R>()),
  l_e2i((K                                                                                    ) * siso_i.get_n_frames_per_wave() + mipp::nElReg<R>()),
  s    ((K                                                                                    ) * siso_n.get_n_frames_per_wave())
{
	const std::string name = "Decoder_turbo";
	this->set_name(name);
	this->set_n_frames(siso_n.get_n_frames());
	this->set_n_frames_per_wave(siso_n.get_n_frames_per_wave());
	this->pi->set_n_frames_per_wave(siso_n.get_n_frames_per_wave());

	(*this->siso_n.get())[dec::tsk::decode_siso_alt        ].set_fast(true);
	(*this->siso_i.get())[dec::tsk::decode_siso_alt        ].set_fast(true);
	(*this->pi    .get())[itl::tsk::interleave             ].set_fast(true);
	(*this->pi    .get())[itl::tsk::deinterleave           ].set_fast(true);
	(*this->pi    .get())[itl::tsk::interleave_reordering  ].set_fast(true);
	(*this->pi    .get())[itl::tsk::deinterleave_reordering].set_fast(true);

	if (siso_n.get_K() != K)
	{
		std::stringstream message;
		message << "'siso_n.get_K()' has to be equal to 'K' ('siso_n.get_K()' = " << siso_n.get_K()
		        << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (siso_i.get_K() != K)
	{
		std::stringstream message;
		message << "'siso_i.get_K()' has to be equal to 'K' ('siso_i.get_K()' = " << siso_i.get_K()
		        << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N - (siso_n.tail_length() + siso_i.tail_length()) != K * 3)
	{
		std::stringstream message;
		message << "'N' - ('siso_n.tail_length()' + 'siso_i.tail_length()') has to be equal to 'K' * 3 ('N' = "
		        << N << ", 'siso_n.tail_length()' = " << siso_n.tail_length()
		        << ", 'siso_i.tail_length()' = " << siso_i.tail_length() << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_ite <= 0)
	{
		std::stringstream message;
		message << "'n_ite' has to be greater than 0 ('n_ite' = " << n_ite << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)pi.get_core().get_size() != K)
	{
		std::stringstream message;
		message << "'pi.get_core().get_size()' has to be equal to 'K' ('pi.get_core().get_size()' = "
		        << pi.get_core().get_size() << ", 'K' = " << K << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (siso_n.get_n_frames() != siso_i.get_n_frames())
	{
		std::stringstream message;
		message << "'siso_n.get_n_frames()' has to be equal to 'siso_i.get_n_frames()' ('siso_n.get_n_frames()' = "
		        << siso_n.get_n_frames() << ", 'siso_i.get_n_frames()' = " << siso_i.get_n_frames() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (pi.get_n_frames() != siso_n.get_n_frames())
	{
		std::stringstream message;
		message << "'pi.get_n_frames()' has to be equal to 'siso_n.get_n_frames()' ('pi.get_n_frames()' = "
		        << pi.get_n_frames() << ", 'siso_n.get_n_frames()' = " << siso_n.get_n_frames() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (siso_n.get_n_frames_per_wave() != siso_i.get_n_frames_per_wave())
	{
		std::stringstream message;
		message << "'siso_n.get_n_frames_per_wave()' has to be equal to 'siso_i.get_n_frames_per_wave()' "
		        << "('siso_n.get_n_frames_per_wave()' = " << siso_n.get_n_frames_per_wave()
		        << ", 'siso_i.get_n_frames_per_wave()' = " << siso_i.get_n_frames_per_wave() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R>
Decoder_turbo<B,R>* Decoder_turbo<B,R>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
void Decoder_turbo<B,R>
::deep_copy(const Decoder_turbo<B,R> &m)
{
	Module::deep_copy(m);
	if (m.siso_n != nullptr) this->siso_n.reset(dynamic_cast<Decoder_SISO<B,R>*>(m.siso_n->clone()));
	if (m.siso_i != nullptr) this->siso_i.reset(dynamic_cast<Decoder_SISO<B,R>*>(m.siso_i->clone()));
	if (m.pi     != nullptr) this->pi    .reset(                                 m.pi    ->clone());
	this->post_processings.clear();
	for (auto &pp : m.post_processings)
		this->post_processings.push_back(std::shared_ptr<tools::Post_processing_SISO<B,R>>(pp->clone()));
}

template <typename B, typename R>
void Decoder_turbo<B,R>
::add_post_processing(const tools::Post_processing_SISO<B,R> &post_processing)
{
	this->post_processings.push_back(std::shared_ptr<tools::Post_processing_SISO<B,R>>(post_processing.clone()));
	if (this->post_processings.back()->get_n_frames() != this->get_n_frames_per_wave())
		this->post_processings.back()->set_n_frames(this->get_n_frames_per_wave());
}

template <typename B, typename R>
void Decoder_turbo<B,R>
::_load(const R *Y_N, const size_t frame_id)
{
	if (buffered_encoding)
		this->buffered_load(Y_N, frame_id);
	else
		this->standard_load(Y_N, frame_id);
}

template <typename B, typename R>
void Decoder_turbo<B,R>
::buffered_load(const R *Y_N, const size_t frame_id)
{
	if (this->get_n_frames_per_wave() == 1)
	{
		std::copy(Y_N                                            , Y_N + siso_n->get_K() + siso_n->tail_length()/2, l_sn.begin()          );
		std::copy(Y_N + siso_n->get_K() + siso_n->tail_length()/2, Y_N + siso_n->get_N(),                           l_pn.begin()          );
		std::copy(Y_N + siso_n->get_N(),                           Y_N + siso_n->get_N() + siso_i->tail_length()/2, l_si.begin() + this->K);
		std::copy(Y_N + siso_n->get_N() + siso_i->tail_length()/2, Y_N + this->N                                  , l_pi.begin()          );
		pi->interleave(l_sn.data(), l_si.data(), frame_id, false);
	}
	else
	{
		const auto n_frames = this->get_n_frames_per_wave();

		std::vector<const R*> frames(n_frames);
		for (size_t f = 0; f < n_frames; f++)
			frames[f] = Y_N + f*this->N;
		tools::Reorderer<R>::apply(frames, l_sn.data(), siso_n->get_K() + siso_n->tail_length()/2);

		for (size_t f = 0; f < n_frames; f++)
			frames[f] = Y_N + f*this->N + siso_n->get_K() + siso_n->tail_length()/2;
		tools::Reorderer<R>::apply(frames, l_pn.data(), siso_n->get_K() + siso_n->tail_length()/2);

		for (size_t f = 0; f < n_frames; f++)
			frames[f] = Y_N + f*this->N + siso_n->get_N();
		tools::Reorderer<R>::apply(frames, &l_si[this->K*n_frames], siso_i->tail_length()/2);

		for (size_t f = 0; f < n_frames; f++)
			frames[f] = Y_N + f*this->N + siso_n->get_N() + siso_i->tail_length()/2;
		tools::Reorderer<R>::apply(frames, l_pi.data(), siso_i->get_K() + siso_i->tail_length()/2);

		pi->interleave_reordering(l_sn.data(), l_si.data(), frame_id, false);
	}
	std::fill(l_e1n.begin(), l_e1n.end(), (R)0);
}

template <typename B, typename R>
void Decoder_turbo<B,R>
::standard_load(const R *Y_N, const size_t frame_id)
{
	const auto tail_n = siso_n->tail_length();
	const auto tail_i = siso_i->tail_length();

	if (this->get_n_frames_per_wave() == 1)
	{
		for (auto i = 0; i < this->K; i++)
		{
			l_sn[i] = Y_N[i*3 +0];
			l_pn[i] = Y_N[i*3 +1];
			l_pi[i] = Y_N[i*3 +2];
		}
		pi->interleave(l_sn.data(), l_si.data(), frame_id, false);

		// tails bit in the natural domain
		for (auto i = 0; i < tail_n/2; i++)
		{
			l_sn[this->K +i] = Y_N[this->K*3 + 2*i +0];
			l_pn[this->K +i] = Y_N[this->K*3 + 2*i +1];
		}

		// tails bit in the interleaved domain
		for (auto i = 0; i < tail_i/2; i++)
		{
			l_si[this->K +i] = Y_N[this->K*3 + tail_n + 2*i +0];
			l_pi[this->K +i] = Y_N[this->K*3 + tail_n + 2*i +1];
		}
	}
	else // inter frame => input reordering
	{
		const auto n_frames = this->get_n_frames_per_wave();

		for (auto i = 0; i < this->K; i++)
		{
			for (size_t j = 0; j < n_frames; j++)
			{
				l_sn[i*n_frames +j] = Y_N[j*((this->K*3) + tail_n + tail_i) + i * 3 +0];
				l_pn[i*n_frames +j] = Y_N[j*((this->K*3) + tail_n + tail_i) + i * 3 +1];
				l_pi[i*n_frames +j] = Y_N[j*((this->K*3) + tail_n + tail_i) + i * 3 +2];
			}
		}
		pi->interleave_reordering(l_sn.data(), l_si.data(), frame_id, false);

		// tails bit in the natural domain
		for (auto i = 0; i < tail_n/2; i++)
		{
			for (size_t j = 0; j < n_frames; j++)
			{
				l_sn[(this->K +i)*n_frames +j] = Y_N[j*((this->K*3) + tail_n + tail_i) + this->K*3 + 2*i +0];
				l_pn[(this->K +i)*n_frames +j] = Y_N[j*((this->K*3) + tail_n + tail_i) + this->K*3 + 2*i +1];
			}
		}

		// tails bit in the interleaved domain
		for (auto i = 0; i < tail_i/2; i++)
		{
			for (size_t j = 0; j < n_frames; j++)
			{
				l_si[(this->K +i)*n_frames +j] = Y_N[j*((this->K*3) + tail_n + tail_i) + this->K*3 + tail_n + 2*i +0];
				l_pi[(this->K +i)*n_frames +j] = Y_N[j*((this->K*3) + tail_n + tail_i) + this->K*3 + tail_n + 2*i +1];
			}
		}
	}

	std::fill(l_e1n.begin(), l_e1n.end(), (R)0);
}

template <typename B, typename R>
void Decoder_turbo<B,R>
::_store(B *V_K) const
{
	if (this->get_n_frames_per_wave() == 1)
	{
		std::copy(s.data(), s.data() + this->K, V_K);
	}
	else // inter frame => output reordering
	{
		const auto n_frames = this->get_n_frames_per_wave();

		std::vector<B*> frames(n_frames);
		for (size_t f = 0; f < n_frames; f++)
			frames[f] = V_K + f*this->K;
		tools::Reorderer<B>::apply_rev(s.data(), frames, this->K);
	}
}

template <typename B, typename R>
void Decoder_turbo<B,R>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Decoder_SIHO<B,R>::set_n_frames(n_frames);
		this->siso_n->set_n_frames(n_frames);
		this->siso_i->set_n_frames(n_frames);
		this->pi->set_n_frames(n_frames);
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_turbo<B_8,Q_8>;
template class aff3ct::module::Decoder_turbo<B_16,Q_16>;
template class aff3ct::module::Decoder_turbo<B_32,Q_32>;
template class aff3ct::module::Decoder_turbo<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_turbo<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
