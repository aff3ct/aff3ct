#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>

#include "Decoder_chase_pyndiah.hpp"

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/hard_decision.h"
#include "Tools/Perf/compute_parity.h"

using namespace aff3ct;
using namespace aff3ct::module;

#define NDEBUG

template <typename B, typename R>
Decoder_chase_pyndiah<B,R>
::Decoder_chase_pyndiah(const int& n_ite,
                        const Interleaver<R> &pi,
                        Decoder_HIHO<B> &dec_r,
                        Decoder_HIHO<B> &dec_c,
                        Encoder     <B> &enc_r,
                        Encoder     <B> &enc_c,
                        const R alpha_,
                        const int n_least_reliable_positions_,
                        const int n_test_vectors_,
                        const int n_competitors_)
: Decoder(dec_r.get_K() * dec_c.get_K(), pi.get_core().get_size(), dec_r.get_n_frames(), dec_r.get_simd_inter_frame_level()),
  Decoder_turbo_product<B,R>(n_ite, pi, dec_r, dec_c, enc_r, enc_c),
  alpha                     (alpha_                                                                  ),
  n_least_reliable_positions(n_least_reliable_positions_                                             ),
  least_reliable_pos        (n_least_reliable_positions                                              ),
  hard_Rprime               (std::max(dec_r.get_N(), dec_c.get_N()) +1                             ), // +1 for parity bit if any
  n_test_vectors            (n_test_vectors_ ? n_test_vectors_ : (int)1 << n_least_reliable_positions),
  test_vect                 (n_test_vectors * hard_Rprime.size()                                     ),
  metrics                   (n_test_vectors                                                          ),
  is_wrong                  (n_test_vectors                                                          ),
  n_competitors             (n_competitors_ ? n_competitors_ : n_test_vectors                        ),
  competitors               (n_test_vectors                                                          ),
  Y_N_cha_i                 (pi.get_core().get_size()                                                )
  // Alpha ({0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.35, 0.35,0.35,0.35,0.35,0.4,0.4,0.4,0.5})
{
	const std::string name = "Decoder_chase_pyndiah";
	this->set_name(name);

	if (n_least_reliable_positions <= 0 || n_least_reliable_positions >= dec_r.get_N() || n_least_reliable_positions >= dec_c.get_N())
	{
		std::stringstream message;
		message << "'n_least_reliable_positions' has to be positive and lower than 'dec_r.get_N()' and 'dec_c.get_N()' ('n_least_reliable_positions' = "
		        << n_least_reliable_positions << ", 'dec_c.get_N()' = " << dec_c.get_N() << " and 'dec_r.get_N()' = " << dec_r.get_N() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_test_vectors <= 0 || n_test_vectors > ((int)1 << n_least_reliable_positions))
	{
		std::stringstream message;
		message << "'n_test_vectors' has to be positive and lower than 2^'n_least_reliable_positions' ('n_least_reliable_positions' = "
		        << n_least_reliable_positions << ", 'n_test_vectors' = " << n_test_vectors << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (n_competitors <= 0 || n_competitors > n_test_vectors)
	{
		std::stringstream message;
		message << "'n_competitors' has to be positive and lower or equal than 'n_test_vectors' ('n_competitors' = "
		        << n_competitors << " and 'n_test_vectors' = " << n_test_vectors << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	generate_bit_flipping_candidates();
}

template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::_decode(const R *Y_N_cha, int return_K_siso)
{
	const int n_cols = this->dec_r.get_N() + (this->parity_extended ? 1 : 0);
	const int n_rows = this->dec_c.get_N() + (this->parity_extended ? 1 : 0);

	this->pi.interleave(Y_N_cha, this->Y_N_cha_i.data(), 0, 1); // interleave data from the channel


	for (int i = 0; i < this->n_ite; i++)
	{
		// alpha = Alpha[2*i];


		this->pi.interleave(this->Y_N_i.data(), this->Y_N_pi.data(), 0, 1); // columns becomes rows

		// decode each col
		for (int j = 0; j < n_cols; j++)
		{
#ifndef NDEBUG
    std::cerr << std::endl;
    std::cerr << std::endl;
    std::cerr << std::endl;
    std::cerr << "(II) col : " << j << std::endl;
#endif
			_decode_row_siso(Y_N_cha_i   .data() + j*n_rows,
			                 this->Y_N_pi.data() + j*n_rows,
			                 this->Y_N_pi.data() + j*n_rows,
			                 this->dec_c,
			                 this->enc_c,
			                 n_rows); // overwrite Y_N_pi
		}

		// alpha = Alpha[2*i+1];

		this->pi.deinterleave(this->Y_N_pi.data(), this->Y_N_i.data(), 0, 1); // rows go back as columns

		// decode each row
		if (i < this->n_ite -1 || (return_K_siso != 0 && return_K_siso != 1))
		{
			for (int j = 0; j < n_rows; j++)
			{
	#ifndef NDEBUG
	    std::cerr << std::endl;
	    std::cerr << std::endl;
	    std::cerr << std::endl;
	    std::cerr << "(II) row : " << j << std::endl;
	#endif
				_decode_row_siso(Y_N_cha + j*n_cols,
				                 this->Y_N_i.data() + j*n_cols,
				                 this->Y_N_i.data() + j*n_cols,
				                 this->dec_r,
				                 this->enc_r,
				                 n_cols); // overwrite Y_N_i
			}
		}
		else if(return_K_siso == 0)
		{
			for (int j = 0; j < this->dec_c.get_K(); j++)
			{
				auto pos = this->enc_c.get_info_bits_pos()[j];

				_decode_row_siho(Y_N_cha + pos*n_cols,
				                 this->Y_N_i.data() + pos*n_cols,
				                 this->V_K_i.data() + j*this->dec_r.get_K(),
				                 this->dec_r,
				                 this->enc_r,
				                 n_cols,
				                 true);
			}
		}
		else if (return_K_siso == 1)
		{
			for (int j = 0; j < n_cols; j++)
				_decode_row_siho(Y_N_cha + j*n_cols,
				                 this->Y_N_i.data() + j*n_cols,
				                 this->V_N_i.data() + j*n_cols,
				                 this->dec_r,
				                 this->enc_r,
				                 n_cols,
				                 false);
		}

	}
}


template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::_decode_row_siho(const R *R_cha, const R *R_prime, B *R_dec, Decoder_HIHO<B> &dec, Encoder<B> &enc, const int size,
                   const bool return_K)
{
	auto& info_bits_pos = enc.get_info_bits_pos();

	if (_decode_chase(R_prime, dec, enc, size))
	{	// syndrome ok, R_prime is a code word, then copy its hard decided version and exit the function

		if (return_K)
		{
			for (int j = 0; j < dec.get_K(); j++)
				R_dec[j] = hard_Rprime[info_bits_pos[j]];
		}
		else
		{
			std::copy(hard_Rprime.data(), hard_Rprime.data() + size, R_dec);
		}
		return;
	}

	auto* DW = test_vect.data() + competitors.front().pos;

	if (return_K)
	{
		for (int j = 0; j < dec.get_K(); j++)
			R_dec[j] = DW[info_bits_pos[j]];
	}
	else
	{
		std::copy(DW, DW + size, R_dec);
	}
}

template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::_decode_row_siso(const R *R_cha, const R *R_prime, R *R_dec, Decoder_HIHO<B> &dec, Encoder<B> &enc, const int size)
{
	if (_decode_chase(R_prime, dec, enc, size))
	{	// syndrome ok, R_prime is a code word, then copy it as decided word and exit the function
		std::copy(R_prime, R_prime + size, R_dec);
		return;
	}

	compute_reliability(R_cha, R_prime, R_dec, size);

#ifndef NDEBUG
    std::cerr  << "(II) R_dec : " <<  std::endl;

    for (int i = 0; i < size; i++)
        std::cerr << R_dec[i] << " ";
    std::cerr << std::endl;
#endif
}

#include "Tools/Display/Frame_trace/Frame_trace.hpp"

template <typename B, typename R>
bool Decoder_chase_pyndiah<B,R>
::_decode_chase(const R *R_prime, Decoder_HIHO<B> &dec, Encoder<B> &enc, const int size)
{
	tools::Frame_trace<> ft(0,3,std::cerr);

	tools::hard_decide(R_prime, hard_Rprime.data(), size);

#ifndef NDEBUG
    std::cerr << "(II) R_prime : " << std::endl;
    for (int i = 0; i < size; i++)
    {
        std::cerr << R_prime[i] << " ";
    }
    std::cerr << std::endl;
    { std::vector<R> v(R_prime, R_prime+size); ft.display_real_vector(v); }

    std::cerr << "(II) hard_Rprime : " << std::endl;
    for (unsigned i = 0; hard_Rprime.size() > i; i++)
    {
        std::cerr << hard_Rprime[i] << " ";
    }
    std::cerr << std::endl;
    { ft.display_bit_vector(hard_Rprime); }
#endif

	// if (this->parity_extended)
	// {
	// 	auto parity_calc = tools::compute_parity(hard_Rprime.data(), dec.get_N());
	// 	parity_diff = parity_calc ^ hard_Rprime[dec.get_N()];
	// 	hard_Rprime[dec.get_N()] = parity_calc;
	// }
	// else
	// 	parity_diff = false;

	find_least_reliable_pos(R_prime, dec.get_N()); // without parity bit if any
	compute_test_vectors   (dec, enc, size);
	compute_metrics        (R_prime,  size);

#ifndef NDEBUG
    std::cerr << "(II) least_reliable_pos : " << std::endl;
    for (unsigned i = 0; i < least_reliable_pos.size(); i++)
    {
        std::cerr << i << ". (" << least_reliable_pos[i].pos << ", " << least_reliable_pos[i].metric << ") -> " << hard_Rprime[least_reliable_pos[i].pos]  << std::endl;
    }


    std::cerr << "(II) Test vectors : " << std::endl;
    ft.display_bit_vector(test_vect, size);

    std::cerr << "(II) Metriques : " << std::endl;
    for (unsigned i = 0; i < metrics.size(); i++)
    {
        std::cerr << i << ". " << metrics[i] << " -> ";

		for (unsigned j = 0; j < tv_candidates[i].size(); j++)
			std::cerr << tv_candidates[i][j] << " ";


    	std::cerr << std::endl;
    }

	auto* DW = test_vect.data() + competitors.front().pos;
    std::cerr << "(II) DW : " << std::endl;
    for (int i = 0; size > i; i++)
    {
        std::cerr << DW[i] << " ";
    }
    std::cerr << std::endl;
    { std::vector<R> v(DW, DW+size);  ft.display_bit_vector(v); }
#endif

	return false;
}

template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::find_least_reliable_pos(const R* R_prime, const int size)
{
	for (auto it = least_reliable_pos.begin(); it != least_reliable_pos.end(); it++)
	{
	    it->metric = std::numeric_limits<R>::max();
	    it->pos    = size;
	}

	for (int i = 0; i < size; i++)
	{
		// check if R[i] is a least reliable value compared to the already saved positions
		for (auto it = least_reliable_pos.begin(); it != least_reliable_pos.end(); it++)
		{
			R val = std::abs(R_prime[i]);

			if (val < it->metric)
			{ // R_prime[i] is weak, then save its position
				least_reliable_pos.pop_back(); // remove the last position that is not weak enough
				it = least_reliable_pos.insert(it, {val, i});
				break;
			}
		}
	}
}

template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::compute_test_vectors(Decoder_HIHO<B> &dec, Encoder<B> &enc, const int size)
{
	tools::Frame_trace<> ft(0,3,std::cerr);
	for (int c = 0; c < n_test_vectors; c++)
	{
		// rearrange hard_Rprime to be a good candidate
		bit_flipping(hard_Rprime.data(), c);

		dec.decode_hiho_cw(hard_Rprime.data(), test_vect.data() + c*size); // parity bit is ignored by the decoder
		is_wrong[c] = !enc.is_codeword(test_vect.data() + c*size);

		if (this->parity_extended)
			test_vect[(c+1)*size -1] = tools::compute_parity(test_vect.data() + c*size, dec.get_N());

#ifndef NDEBUG
	    std::cerr << "(II) Test vectors " << c << " before correction" << std::endl;
	    ft.display_bit_vector(hard_Rprime);
	    std::cerr << "(II) Test vectors " << c << " after correction : is wrong = " << is_wrong[c] << std::endl;
	    { std::vector<B> v(test_vect.data() + c*size, test_vect.data() + (c+1)*size); ft.display_bit_vector(v); }
#endif
		bit_flipping(hard_Rprime.data(), c); // apply again the bit flipping to recover the original hard_Rprime
	}
}

template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::compute_metrics(const R* R_prime, const int size)
{
	std::fill(metrics.begin(), metrics.end(), (R)0);

	for (int c = 0; c < n_test_vectors; c++)
	{
		if (is_wrong[c])
		{
			metrics[c] = std::numeric_limits<R>::max()/2;
			continue;
		}

		int tv_off = c*size;

		for (int i = 0; i < size; i++)
			if (hard_Rprime[i] ^ test_vect[tv_off + i])
				metrics[c] += std::abs(R_prime[i]);
	}

	// reorder metrics -> decided word is at first position of competitors list
	for (int c = 0; c < n_test_vectors; c++)
	{
		competitors[c].metric = metrics[c];
		competitors[c].pos    = c*size;
	}

	std::sort(competitors.begin(), competitors.end(), [](const info& a, const info& b) { return a.metric < b.metric; });


	// remove duplicated metrics
	unsigned start_pos = 0;
	while ((int)competitors.size() > n_competitors && start_pos < (competitors.size() -1))
	{
		auto it = competitors.begin();
		std::advance(it, start_pos +1);

		// remove all duplications of the metric at position 'start_pos'
		while (it != competitors.end())
		{
			if (it->metric == competitors[start_pos].metric)
			{
				it = competitors.erase(it);

				if ((int)competitors.size() == n_competitors)
					break;
			}
			else
				it++;
		}

		start_pos++; // try with the next one
	}

	competitors.resize(n_test_vectors);


#ifndef NDEBUG
    std::cerr << "(II) competitors : " << std::endl;
    for (unsigned i = 0; i < competitors.size(); i++)
    {
        std::cerr << i << ". " << competitors[i].pos << " " << competitors[i].metric << std::endl;
    }
	    std::cerr << std::endl;
#endif
}

template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::compute_reliability(const R* R_cha, const R* R_prime, R* R_dec, const int size)
{
	const auto & DW = competitors.front(); // the Decided Word

	// compute beta, the sum of the least reliable position reliabilities in the decided word
	R beta = 0;
	for (int i = 0; i < n_least_reliable_positions; i++)
		beta += least_reliable_pos[i].metric;

#ifndef NDEBUG
		std::cerr << "beta = " << beta << ", alpha = " << alpha<< ", DW.metric = " << DW.metric << std::endl;
#endif

	beta -= DW.metric;

	for (int j = 1; j < n_competitors; j++)
		competitors[j].metric -= DW.metric;


	for (int i = 0; i < size; i++)
	{
		const auto DB = test_vect[DW.pos + i]; // decided bit at the position i

		int j = 1;
		for (; j < n_competitors; j++)
			if (test_vect[competitors[j].pos + i] != DB)
				break;

		R reliability;

		if (j < n_competitors) // then there is a competitor with a different bit at the position i
		{
			reliability = competitors[j].metric;
		}
		else // same bits for each candidates
		{
			reliability = std::abs(R_prime[i]) + beta;
			if (reliability < 0)
				reliability = 0;
		}

		if (DB) // if DB is a 1
			reliability = -reliability; // set as negative

#ifndef NDEBUG
		std::cerr << "Rel:  i = " << i << ", R_prime = " << R_prime[i] << ", competitors[" << j << "].metric = " << competitors[j].metric;
#endif

		R_dec[i] = R_cha[i] + alpha * (reliability - R_prime[i]);
#ifndef NDEBUG
		std::cerr  << ", reliability = " << reliability << ", R_cha = " << R_cha[i]<< ", R_dec = " << R_dec[i] << std::endl;
#endif
	}
}

template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::bit_flipping(B* hard_vect, const int c)
{
	for (int i = 0; i < n_least_reliable_positions; i++)
		hard_vect[least_reliable_pos[i].pos] = tv_candidates[c][i] ? !hard_vect[least_reliable_pos[i].pos] : hard_vect[least_reliable_pos[i].pos];
}

template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::generate_bit_flipping_candidates()
{
	tv_candidates.resize(n_test_vectors, std::vector<bool>(n_least_reliable_positions, false));

	std::vector<int> cand(n_test_vectors, 0);

	if (n_test_vectors == (1 << n_least_reliable_positions))
	{
		std::iota(cand.begin(), cand.end(), 0);
	}
	else if (n_test_vectors <= (n_least_reliable_positions * (n_least_reliable_positions + 1) / 2 + 1))
	{
		// (0) do not flip any bit -> 1 test vector
		// (1) flip one by one all least reliable positions -> 'n_least_reliable_positions' test vectors
		// (2) always flip the least realiable position and do (1) on the 'n_least_reliable_positions' -1 left bits -> 'n_least_reliable_positions' -1 test vectors
		// (3) always flip the two least reliable position and do (1) on the 'n_least_reliable_positions' -2 left bits -> 'n_least_reliable_positions' -2 test vectors
		// (4) and so on until reaching the will number of test vectors with a maximum of the sum from 1 to 'n_least_reliable_positions' + 1

		int idx_pos = 0, start_idx_pos = 0;
		int least_reliable_pos_mask = 0;
		int flipped_bit_mask = 1;

		for (int i = 1; i < n_test_vectors; i++)
		{
			cand[i] = flipped_bit_mask | least_reliable_pos_mask;

			idx_pos++;
			flipped_bit_mask <<= 1;

			if (idx_pos >= n_least_reliable_positions)
			{
				start_idx_pos ++;
				idx_pos = start_idx_pos;

				least_reliable_pos_mask <<= 1;
				least_reliable_pos_mask  += 1;

				flipped_bit_mask = 1 << start_idx_pos;
			}
		}
	}
	else
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "unimplemented method");

	for (int i = 0; i < n_test_vectors; i++)
		for (int j = 0; j < n_least_reliable_positions; j++)
			tv_candidates[i][j] = ((cand[i] >> j) & (int)1) != 0;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_chase_pyndiah<B_8,Q_8>;
template class aff3ct::module::Decoder_chase_pyndiah<B_16,Q_16>;
template class aff3ct::module::Decoder_chase_pyndiah<B_32,Q_32>;
template class aff3ct::module::Decoder_chase_pyndiah<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_chase_pyndiah<B,R>;
#endif
// ==================================================================================== explicit template instantiation
