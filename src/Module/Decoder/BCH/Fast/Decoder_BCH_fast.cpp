#include <type_traits>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <string>
#include <limits>
#include <memory>

#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Perf/Reorderer/Reorderer.hpp"
#include "Tools/Exception/exception.hpp"
#include "Module/Decoder/BCH/Fast/Decoder_BCH_fast.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_BCH_fast<B,R>
::Decoder_BCH_fast(const int& K, const int& N, const tools::BCH_polynomial_generator<B> &GF_poly)
: Decoder_BCH<B,R>(K, N, GF_poly.get_t()                                   ),
  t2              (2 * this->t                                             ),
  YH_N            (this->N * mipp::N<B>()                                  ),
  Y_N_reorderered (this->N,                              mipp::Reg<B>((B)0)),
  elp             (this->N_p2_1+2, mipp::vector<mipp::Reg<B>>(this->N_p2_1)),
  discrepancy     (this->N_p2_1+2,                       mipp::Reg<B>((B)0)),
  l               (this->N_p2_1+2,                       mipp::Reg<B>((B)0)),
  u_lu            (this->N_p2_1+2,                       mipp::Reg<B>((B)0)),
  s               (t2+1,                                 mipp::Reg<B>((B)0)),
  reg             (this->t +1,                           mipp::Reg<B>((B)0)),
  m               (GF_poly.get_m()                                         ),
  d               (GF_poly.get_d()                                         ),
  alpha_to        (GF_poly.get_alpha_to()                                  ),
  index_of        (GF_poly.get_index_of()                                  )
{
	const std::string name = "Decoder_BCH_fast";
	this->set_name(name);
	this->set_n_frames_per_wave(mipp::N<B>());

	if ((this->N - this->K) != GF_poly.get_n_rdncy())
	{
		std::stringstream message;
		message << "'N - K' is different than 'GF_poly.get_n_rdncy()' ('K' = " << K << ", 'N' = " << N
		        << ", 'GF_poly.get_n_rdncy()' = " << GF_poly.get_n_rdncy() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N_p2_1*2 >= std::numeric_limits<B>::max() || std::is_unsigned<B>::value)
	{
		std::stringstream message;
		message << "'N_p2_1'*2 must be less than 'std::numeric_limits<B>::max()' and 'B' must be signed ('N_p2_1'*2 = "
		        << this->N_p2_1*2 << ", 'std::numeric_limits<B>::max()' = " << +std::numeric_limits<B>::max() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->last_is_codeword.resize(this->n_frames + mipp::N<B>()); // in case where only the last frame_id is called to prevent memory leak
}

template <typename B, typename R>
Decoder_BCH_fast<B,R>* Decoder_BCH_fast<B,R>
::clone() const
{
	auto m = new Decoder_BCH_fast(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, class A = std::allocator<B>>
mipp::Reg<B> read_array(const std::vector<B,A>& array, mipp::Reg<B> r_idx, const mipp::Msk<mipp::N<B>()>& m_idx = true)
{
	B idx[mipp::N<B>()], val[mipp::N<B>()];

	const auto r_zero = mipp::Reg<B>((B)0);
	r_idx = mipp::blend(r_idx, r_zero, m_idx);
	r_idx.store(idx);

	for (auto j = 0; j < mipp::N<B>(); j++)
		val[j] = array[idx[j]];

	return mipp::Reg<B>(val);
}

template <typename B, class A = std::allocator<B>>
void write_array(std::vector<mipp::Reg<B>,A>& array, const mipp::Reg<B>& r_idx, const mipp::Msk<mipp::N<B>()>& m_idx,
                 const mipp::Reg<B>& r_values)
{
	mipp::vector<B> v_iota(mipp::N<B>());
	std::iota(v_iota.begin(), v_iota.end(), 0);
	mipp::Reg<B> r_iota = v_iota.data();

	B idx[mipp::N<B>()];
	r_idx.store(idx);

	for (auto j = 0; j < mipp::N<B>(); j++)
		array[idx[j]] = mipp::blend(r_values, array[idx[j]], m_idx & (r_iota == j));
}

template <typename B>
mipp::Reg<B> operator%(mipp::Reg<B> r, int mod)
{
	r =    mipp::blend(r - (B)mod, r, r >= (B)mod);
	return mipp::blend(r + (B)mod, r, r <  (B)0  );
}

template <typename B, typename R>
int Decoder_BCH_fast<B,R>
::_decode(const size_t frame_id)
{
	const auto r_zero = mipp::Reg<B>((B)0);
	const auto r_one  = mipp::Reg<B>((B)1);
	const auto r_mone = mipp::Reg<B>((B)-1);


	// form the syndrome
	auto syn_error = r_zero != r_zero; // init to false

	std::fill(s.begin(), s.end(), r_zero);

	for (auto i = 1; i <= t2; i++)
	{
		for (auto j = 0; j < this->N; j++)
		{
			const mipp::Reg<B> r_alpha = alpha_to[(i * j) % this->N_p2_1];
			s[i] ^= mipp::blend(r_alpha, r_zero, Y_N_reorderered[j] != r_zero);
		}

		syn_error |= s[i] != r_zero;

		s[i] = read_array(index_of, s[i]);
	}


	if (!mipp::testz(syn_error))
	{
		// initialise table entries
		discrepancy[0] = r_zero; // index form
		discrepancy[1] = s[1]; // index form
		elp[0][0] = r_zero; // index form
		elp[1][0] = r_one; // polynomial form
		for (auto i = 1; i < t2; i++)
		{
			elp[0][i] = r_mone; // index form
			elp[1][i] = r_zero; // polynomial form
		}
		l[0]    = r_zero;
		l[1]    = r_zero;
		u_lu[0] = r_mone;
		u_lu[1] = r_zero;

		auto u = 0;
		auto m_process = syn_error;

		do
		{
			u++;
			mipp::Reg<B> r_u = (B)u;
			const auto u_m1 = u - 1;
			const auto u_p1 = u + 1;

			const auto m_disc     = (discrepancy[u] == r_mone) & m_process;
			const auto m_not_disc = (discrepancy[u] != r_mone) & m_process;



			//***** part if (discrepancy[u] == -1)
			if (!mipp::testz(m_disc))
			{
				l[u_p1] = mipp::blend(l[u], l[u_p1], m_disc);

				auto l_max = mipp::hmax(mipp::blend(l[u], r_zero, m_disc));
				for (auto i = 0; i <= l_max; i++)
				{
					auto m_ok = m_disc & (l[u] >= (B)i);
					elp[u_p1][i] = mipp::blend(elp[u][i], elp[u_p1][i], m_ok);
					elp[u   ][i] = mipp::blend(read_array(index_of, elp[u][i], m_ok), elp[u][i], m_ok);
				}
			}

			//**** part if (discrepancy[u] != -1)
			if (!mipp::testz(m_not_disc))
			{
				// search for words with greatest u_lu[q] for which discrepancy[q] != -1
				mipp::Reg<B> r_disc_q = discrepancy[0], r_l_q = l[0], r_u_lu_q = u_lu[0], r_q = r_zero;
				mipp::vector<mipp::Reg<B>> r_elp_q = elp[0];

				for (auto j = u_m1; j >= 0; j--)
				{
					auto m_ok = m_not_disc & (discrepancy[j] != r_mone) & (r_u_lu_q < u_lu[j]);

					r_q      = mipp::blend(mipp::Reg<B>(j), r_q,      m_ok);
					r_disc_q = mipp::blend( discrepancy[j], r_disc_q, m_ok);
					r_l_q    = mipp::blend(           l[j], r_l_q,    m_ok);
					r_u_lu_q = mipp::blend(        u_lu[j], r_u_lu_q, m_ok);

					for (unsigned i = 0; i < r_elp_q.size(); i++)
						r_elp_q[i] = mipp::blend(elp[j][i], r_elp_q[i], m_ok);
				}


				// store degree of new elp polynomial
				auto l_u_q = r_l_q + r_u - r_q;
				l_u_q   = mipp::blend(l_u_q, l[u   ], l[u] <= l_u_q);
				l[u_p1] = mipp::blend(l_u_q, l[u_p1],    m_not_disc);

				// form new elp(x)
				for (auto i = 0; i < t2; i++)
					elp[u_p1][i] = mipp::blend(r_zero, elp[u_p1][i], m_not_disc);

				auto l_max = mipp::hmax(mipp::blend(r_l_q, r_zero, m_not_disc));
				for (auto i = 0; i <= l_max; i++)
				{
					auto m_ok = m_not_disc & (r_elp_q[i] != r_mone) & (r_l_q >= i);

					if (mipp::testz(m_ok))
						continue;


					const auto r_alpha_idx = ((r_elp_q[i] - r_disc_q) % this->N_p2_1 + discrepancy[u]) % this->N_p2_1;
					const auto r_idx = mipp::Reg<B>(i) + r_u - r_q;
					write_array(elp[u_p1], r_idx, m_ok, read_array(alpha_to, r_alpha_idx, m_ok));
				}

				l_max = mipp::hmax(mipp::blend(l[u], r_zero, m_not_disc));
				for (auto i = 0; i <= l_max; i++)
				{
					auto m_ok = m_not_disc & (l[u] >= i);
					elp[u_p1][i] = mipp::blend(elp[u][i] ^ elp[u_p1][i], elp[u_p1][i], m_ok);
					elp[u   ][i] = mipp::blend(read_array(index_of, elp[u][i], m_ok), elp[u][i], m_ok);
				}
			}

			u_lu[u_p1] = r_u - l[u_p1];


			// form (u_p1)th discrepancy
			if (u < t2)
			{
				// no discrepancy computed on last iteration
				{
					const auto m_s         = s[u_p1] != -1;
					const auto r_alpha_idx = s[u_p1] % this->N_p2_1;
					const auto r_read      = mipp::blend(read_array(alpha_to, r_alpha_idx, m_s), r_zero, m_s);
					discrepancy[u_p1]      = mipp::blend(r_read, discrepancy[u_p1], m_process);
				}

				auto l_max = mipp::hmax(mipp::blend(l[u_p1], r_zero, m_process));
				for (auto i = 1; i <= l_max; i++)
				{
					auto u_p1_i = ((u_p1 - i) < 0) ? 0 : (u_p1 - i);
					auto m_ok = m_process & (s[u_p1_i] != r_mone) & (elp[u_p1][i] != r_zero) & (l[u_p1] >= i);

					if (mipp::testz(m_ok))
						continue;

					const auto r_alpha_idx = (s[u_p1_i] + read_array(index_of, elp[u_p1][i], m_ok)) % this->N_p2_1;
					const auto r_alpha = read_array(alpha_to, r_alpha_idx, m_ok);
					const auto r_disc  = discrepancy[u_p1] ^ r_alpha;

					discrepancy[u_p1] = mipp::blend(r_disc, discrepancy[u_p1], m_ok);
				}

				/* put d.at(u_p1) into index form */
				discrepancy[u_p1] = mipp::blend(read_array(index_of, discrepancy[u_p1], m_process), discrepancy[u_p1], m_process);
			}

			m_process &= l[u_p1] <= this->t;
		} while (u < t2 && !mipp::testz(m_process));


		u++;
		// Correct errors
		const auto m_corr = (l[u] <= this->t) & syn_error;

		if (!mipp::testz(m_corr))
		{
			// Chien search: find roots of the error location polynomial
			auto l_max = mipp::hmax(mipp::blend(l[u], r_zero, m_corr));
			for (auto i = 1; i <= l_max; i++)
			{
				auto r_idx = mipp::blend(elp[u][i], r_zero, l[u] >= i);
				reg[i] = read_array(index_of, r_idx, m_corr);
			}

			mipp::Reg<B> count = r_zero;
			for (auto i = 1; i <= this->N_p2_1; i++)
			{
				mipp::Reg<B> q = r_one;
				for (auto j = 1; j <= l_max; j++)
				{
					auto m_ok = (l[u] >= j) & (reg[j] != r_mone) & m_corr;
					reg[j] = mipp::blend((reg[j] + j) % this->N_p2_1, reg[j], m_ok);
					q = mipp::blend(q ^ read_array(alpha_to, reg[j], m_ok), q, m_ok);
				}

				auto m_flip = (q == r_zero) & m_corr;
				count += mipp::blend(r_one, r_zero, m_flip);


				auto idx = this->N_p2_1 - i;
				Y_N_reorderered[idx] = mipp::blend(Y_N_reorderered[idx] ^ r_one, Y_N_reorderered[idx], m_flip);
			}

			syn_error &= count != l[u];
		}
	}

	mipp::toReg<B>(~syn_error).store(this->last_is_codeword.data() + frame_id);

	return 0;
}

template <typename B, typename R>
void Decoder_BCH_fast<B,R>
::_load(const B *Y_N, const size_t frame_id)
{
	// reorder data into mipp registers
	std::vector<const B*> frames(mipp::N<B>());
	for (auto f = 0; f < mipp::N<B>(); f++)
		frames[f] = Y_N + f * this->N;

	tools::Reorderer_static<B,mipp::N<B>()>::apply(frames, (B*)this->Y_N_reorderered.data(), this->N);
}

template <typename B, typename R>
int Decoder_BCH_fast<B,R>
::_decode_hiho(const B *Y_N, B *V_K, const size_t frame_id)
{
	_load(Y_N, frame_id);

	auto status = this->_decode(frame_id);

	// reorder data into standard registers
	std::vector<B*> frames_rev(mipp::N<B>());
	for (auto f = 0; f < mipp::N<B>(); f++)
		frames_rev[f] = V_K + f * this->K;
	tools::Reorderer_static<B,mipp::N<B>()>::apply_rev((B*)(this->Y_N_reorderered.data() + this->N - this->K), frames_rev, this->K);

	return status;
}

template <typename B, typename R>
int Decoder_BCH_fast<B,R>
::_decode_hiho_cw(const B *Y_N, B *V_N, const size_t frame_id)
{
	_load(Y_N, frame_id);

	auto status = this->_decode(frame_id);

	// reorder data into standard registers
	std::vector<B*> frames_rev(mipp::N<B>());
	for (auto f = 0; f < mipp::N<B>(); f++)
		frames_rev[f] = V_N + f * this->N;
	tools::Reorderer_static<B,mipp::N<B>()>::apply_rev((B*)this->Y_N_reorderered.data(), frames_rev, this->N);

	return status;
}

template <typename B, typename R>
int Decoder_BCH_fast<B,R>
::_decode_siho(const R *Y_N, B *V_K, const size_t frame_id)
{
	tools::hard_decide(Y_N, this->YH_N.data(), this->N * mipp::N<B>());

	auto status = this->_decode_hiho(this->YH_N.data(), V_K, frame_id);

	return status;
}

template <typename B, typename R>
int Decoder_BCH_fast<B,R>
::_decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id)
{
	tools::hard_decide(Y_N, this->YH_N.data(), this->N * mipp::N<B>());

	auto status = this->_decode_hiho_cw(this->YH_N.data(), V_N, frame_id);

	return status;
}

template <typename B, typename R>
void Decoder_BCH_fast<B,R>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Decoder_BCH<B,R>::set_n_frames(n_frames);

		// in case where only the last frame_id is called to prevent memory leak
		this->last_is_codeword.resize(this->n_frames + mipp::N<B>());
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_BCH_fast<B_8,Q_8>;
template class aff3ct::module::Decoder_BCH_fast<B_16,Q_16>;
template class aff3ct::module::Decoder_BCH_fast<B_32,Q_32>;
template class aff3ct::module::Decoder_BCH_fast<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_BCH_fast<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
