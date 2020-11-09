#include <cmath>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

// [sysn | tail sysn | pn | tail pn | tail sysi | pi | tail pi]

template <typename B>
Encoder_turbo<B>
::Encoder_turbo(const int& K, const int& N, const Encoder<B> &enco_n, const Encoder<B> &enco_i,
                const Interleaver<B> &pi)
: Encoder<B>(K, N),
  pi        (pi    .clone()),
  enco_n    (enco_n.clone()),
  enco_i    (enco_i.clone()),
  U_K_i     (K * enco_n.get_n_frames()),
  X_N_tmp   ((enco_n.get_N() >= enco_i.get_N() ? enco_n.get_N() : enco_i.get_N()) * enco_n.get_n_frames())
{
	const std::string name = "Encoder_turbo";
	this->set_name(name);

	if (N - (enco_n.tail_length() + enco_i.tail_length()) != 3 * K)
	{
		std::stringstream message;
		message << "'N' - ('enco_n.tail_length()' + 'enco_i.tail_length()') has to be equal to 3 * 'K' ('N' = " << N
		        << ", 'enco_n.tail_length()' = " << enco_n.tail_length()
		        << ", 'enco_i.tail_length()' = " << enco_i.tail_length()
		        << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)pi.get_core().get_size() != K)
	{
		std::stringstream message;
		message << "'pi.get_core().get_size()' has to be equal to 'K' ('pi.get_core().get_size()' = "
		        << pi.get_core().get_size() << ", 'K' = " << K << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (enco_n.get_n_frames() != enco_i.get_n_frames())
	{
		std::stringstream message;
		message << "'enco_n.get_n_frames()' has to be equal to 'enco_i.get_n_frames()' ('enco_n.get_n_frames()' = "
		        << enco_n.get_n_frames() << ", 'enco_i.get_n_frames()' = " << enco_i.get_n_frames() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (enco_n.get_n_frames() != pi.get_n_frames())
	{
		std::stringstream message;
		message << "'enco_n.get_n_frames()' has to be equal to 'pi.get_n_frames()' ('enco_n.get_n_frames()' = "
		        << enco_n.get_n_frames() << ", 'pi.get_n_frames()' = " << pi.get_n_frames() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->set_n_frames(enco_n.get_n_frames());
}

template <typename B>
Encoder_turbo<B>* Encoder_turbo<B>
::clone() const
{
	auto m = new Encoder_turbo(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_turbo<B>
::deep_copy(const Encoder_turbo<B> &m)
{
	Module::deep_copy(m);
	if (m.enco_n != nullptr) this->enco_n.reset(m.enco_n->clone());
	if (m.enco_i != nullptr) this->enco_i.reset(m.enco_i->clone());
	if (m.pi     != nullptr) this->pi    .reset(m.pi    ->clone());
}

template <typename B>
int Encoder_turbo<B>
::tail_length() const
{
	return enco_n->tail_length() + enco_i->tail_length();
}

template <typename B>
void Encoder_turbo<B>
::_encode(const B *U_K, B *X_N, const size_t frame_id)
{
	pi->interleave(U_K - frame_id * this->K, U_K_i.data(), frame_id);

	enco_n->encode(U_K - frame_id * this->K, X_N_tmp.data(), frame_id);

	std::copy(X_N_tmp.data() + frame_id * enco_n->get_N(),
	          X_N_tmp.data() + frame_id * enco_n->get_N() + enco_n->get_N(),
	          X_N);

	enco_i->encode(U_K_i.data(), X_N_tmp.data(), frame_id);

	std::copy(X_N_tmp.data() + frame_id * enco_i->get_N() + enco_i->get_K(),
	          X_N_tmp.data() + frame_id * enco_i->get_N() + enco_i->get_N(),
	          X_N                                         + enco_n->get_N());
}

template <typename B>
bool Encoder_turbo<B>
::is_codeword(const B *X_N)
{
	if (!enco_n->is_codeword(X_N))
		return false;

	const auto *U_K_n = X_N;
	pi->interleave(U_K_n, U_K_i.data(), 0, false);

	std::copy(U_K_i  .begin(),
	          U_K_i  .begin() + enco_i->get_K(),
	          X_N_tmp.begin());
	std::copy(X_N             + enco_n->get_N(),
	          X_N             + this->N,
	          X_N_tmp.begin() + enco_i->get_K());

	if (!enco_i->is_codeword(X_N_tmp.data()))
		return false;

	return true;
}

template <typename B>
void Encoder_turbo<B>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Encoder<B>::set_n_frames(n_frames);

		const auto old_U_K_i_size = this->U_K_i.size();
		const auto new_U_K_i_size = (old_U_K_i_size / old_n_frames) * n_frames;
		this->U_K_i.resize(new_U_K_i_size);

		const auto old_X_N_tmp_size = this->X_N_tmp.size();
		const auto new_X_N_tmp_size = (old_X_N_tmp_size / old_n_frames) * n_frames;
		this->X_N_tmp.resize(new_X_N_tmp_size);

		this->enco_n->set_n_frames(n_frames);
		this->enco_i->set_n_frames(n_frames);
		this->pi    ->set_n_frames(n_frames);
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_turbo<B_8>;
template class aff3ct::module::Encoder_turbo<B_16>;
template class aff3ct::module::Encoder_turbo<B_32>;
template class aff3ct::module::Encoder_turbo<B_64>;
#else
template class aff3ct::module::Encoder_turbo<B>;
#endif
// ==================================================================================== explicit template instantiation
