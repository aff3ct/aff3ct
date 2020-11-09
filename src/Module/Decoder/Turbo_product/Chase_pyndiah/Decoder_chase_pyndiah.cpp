#define NDEBUG_TPC

#include <cmath>
#include <string>
#include <limits>
#include <numeric>
#include <sstream>
#include <algorithm>
#ifndef NDEBUG_TPC
#include <iostream>
#endif

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/common/hard_decide.h"
#include "Tools/Perf/compute_parity.h"
#ifndef NDEBUG_TPC
#include "Tools/Display/Frame_trace/Frame_trace.hpp"
#endif
#include "Module/Decoder/Turbo_product/Chase_pyndiah/Decoder_chase_pyndiah.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_chase_pyndiah<B,R>
::Decoder_chase_pyndiah(const int K, const int N, // N includes the parity bit if any
                        const Decoder_BCH<B,R> &dec_,
                        const Encoder    <B  > &enc_,
                        const int n_least_reliable_positions_,
                        const int n_test_vectors_,
                        const int n_competitors_,
                        const std::vector<float>& cp_coef)
: Decoder_SISO<B,R>(K, N),
  dec                       (dec_.clone()                                                            ),
  enc                       (enc_.clone()                                                            ),
  N_np                      (dec->get_N()                                                            ),
  n_least_reliable_positions(n_least_reliable_positions_                                             ),
  n_test_vectors            (n_test_vectors_ ? n_test_vectors_ : (int)1 << n_least_reliable_positions),
  n_competitors             (n_competitors_  ? n_competitors_  : n_test_vectors                      ),
  parity_extended           (this->N == (N_np +1)                                                    ),
  cp_coef                   (cp_coef                                                                 ),
  least_reliable_pos        (n_least_reliable_positions                                              ),
  competitors               (n_test_vectors                                                          ),
  hard_Y_N                  (this->N                                                                 ),
  test_vect                 (n_test_vectors * hard_Y_N.size()                                        ),
  metrics                   (n_test_vectors                                                          ),
  is_wrong                  (n_test_vectors                                                          ),
  beta_is_set               (false                                                                   )
{
	const std::string name = "Decoder_chase_pyndiah";
	this->set_name(name);

	if (n_least_reliable_positions <= 0 || n_least_reliable_positions >= N_np)
	{
		std::stringstream message;
		message << "'n_least_reliable_positions' has to be positive and lower than 'N_np' ('n_least_reliable_positions' = "
		        << n_least_reliable_positions << ", 'N_np' = " << N_np << ").";
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


	if (enc->get_K() != dec->get_K())
	{
		std::stringstream message;
		message << "'enc->get_K()' has to be equal to 'dec->get_K()' "
		        << "('enc->get_K()' = " << enc->get_K()
		        << ", 'dec->get_K()' = " << dec->get_K() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (enc->get_N() != dec->get_N())
	{
		std::stringstream message;
		message << "'enc->get_N()' has to be equal to 'dec->get_K()' "
		        << "('enc->get_N()' = " << enc->get_N()
		        << ", 'dec->get_N()' = " << dec->get_N() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (cp_coef.size() != 5)
	{
		std::stringstream message;
		message << "'cp_coef.size()' has to be equal to 5 (the 5 coef a, b, c, d and e of the Pyndiah)"
		        << "('cp_coef.size()' = " << cp_coef.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	generate_bit_flipping_candidates(); // generate bit flipping patterns in 'test_patterns'
}

template <typename B, typename R>
Decoder_chase_pyndiah<B,R>* Decoder_chase_pyndiah<B,R>
::clone() const
{
	auto m = new Decoder_chase_pyndiah(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::deep_copy(const Decoder_chase_pyndiah<B,R> &m)
{
	Module::deep_copy(m);
	if (m.dec != nullptr) this->dec.reset(m.dec->clone());
	if (m.enc != nullptr) this->enc.reset(m.enc->clone());
}

template <typename B, typename R>
const std::vector<uint32_t>& Decoder_chase_pyndiah<B,R>
::get_info_bits_pos()
{
	return enc->get_info_bits_pos();
}

template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::set_beta(R b)
{
	if (b < (R)0.)
	{
		std::stringstream message;
		message << "beta value 'b' has to be positive or null ('b' = " << b << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	beta_is_set = true;
	beta = b;
}

template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::clear_beta()
{
	beta_is_set = false;
}

template <typename B, typename R>
int Decoder_chase_pyndiah<B,R>
::_decode_siho(const R *Y_N, B *V_K, const size_t frame_id)
{
	auto status = decode_chase(Y_N, frame_id);

	auto* DW = test_vect.data() + competitors.front().pos;
	auto& info_bits_pos = enc->get_info_bits_pos();

	for (int j = 0; j < this->K; j++)
		V_K[j] = DW[info_bits_pos[j]];

	return status;
}

template <typename B, typename R>
int Decoder_chase_pyndiah<B,R>
::_decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id)
{
	auto status = decode_chase(Y_N, frame_id);

	auto* DW = test_vect.data() + competitors.front().pos;

	std::copy(DW, DW + this->N, V_N);

	return status;
}

template <typename B, typename R>
int Decoder_chase_pyndiah<B,R>
::_decode_siso(const R *Y_N1, R *Y_N2, const size_t frame_id)
{
	auto status = decode_chase(Y_N1, frame_id);

	compute_reliability(Y_N1, Y_N2);

#ifndef NDEBUG_TPC
    std::cerr  << "(II) Y_N2 : " <<  std::endl;

    for (int i = 0; i < this->N; i++)
        std::cerr << Y_N2[i] << " ";
    std::cerr << std::endl;
#endif

    return status;
}

template <typename B, typename R>
int Decoder_chase_pyndiah<B,R>
::decode_chase(const R *Y_N, const size_t frame_id)
{
	tools::hard_decide(Y_N, hard_Y_N.data(), this->N);

#ifndef NDEBUG_TPC
	tools::Frame_trace<> ft(0,3,std::cerr);

    std::cerr << "(II) Y_N : " << std::endl;
    for (int i = 0; i < this->N; i++)
    {
        std::cerr << Y_N[i] << " ";
    }
    std::cerr << std::endl;
    { std::vector<R> v(Y_N, Y_N+this->N); ft.display_real_vector(v); }

    std::cerr << "(II) hard_Y_N : " << std::endl;
    for (unsigned i = 0; hard_Y_N.size() > i; i++)
    {
        std::cerr << hard_Y_N[i] << " ";
    }
    std::cerr << std::endl;
    { ft.display_bit_vector(hard_Y_N); }
#endif

	// if (this->parity_extended)
	// {
	// 	auto parity_calc = tools::compute_parity(hard_Y_N.data(), dec->get_N());
	// 	parity_diff = parity_calc ^ hard_Y_N[dec->get_N()];
	// 	hard_Y_N[dec->get_N()] = parity_calc;
	// }
	// else
	// 	parity_diff = false;

	find_least_reliable_pos(Y_N); // without parity bit if any
	compute_test_vectors   (frame_id); // make bit flipping of least reliable positions and try to decode them
	compute_metrics        (Y_N); // compute euclidian metrics for each test vectors

#ifndef NDEBUG_TPC
    std::cerr << "(II) least_reliable_pos : " << std::endl;
    for (unsigned i = 0; i < least_reliable_pos.size(); i++)
    {
        std::cerr << i << ". (" << least_reliable_pos[i].pos << ", " << least_reliable_pos[i].metric << ") -> " << hard_Y_N[least_reliable_pos[i].pos]  << std::endl;
    }


    std::cerr << "(II) Test vectors : " << std::endl;
    ft.display_bit_vector(test_vect, this->N);

    std::cerr << "(II) Metriques : " << std::endl;
    for (unsigned i = 0; i < metrics.size(); i++)
    {
        std::cerr << i << ". " << metrics[i] << " -> ";

		for (unsigned j = 0; j < test_patterns[i].size(); j++)
			std::cerr << test_patterns[i][j] << " ";


    	std::cerr << std::endl;
    }

	auto* DW = test_vect.data() + competitors.front().pos;
    std::cerr << "(II) DW : " << std::endl;
    for (int i = 0; this->N > i; i++)
    {
        std::cerr << DW[i] << " ";
    }
    std::cerr << std::endl;
    { std::vector<R> v(DW, DW+this->N);  ft.display_bit_vector(v); }
#endif

    return 0;
}

template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::find_least_reliable_pos(const R* Y_N)
{
	for (auto it = least_reliable_pos.begin(); it != least_reliable_pos.end(); it++)
	{
		it->metric = std::numeric_limits<R>::max();
		it->pos    = this->N;
	}

	for (int i = 0; i < N_np; i++)
	{
		// check if R[i] is a least reliable value compared to the already saved positions
		for (auto it = least_reliable_pos.begin(); it != least_reliable_pos.end(); it++)
		{
			R val = std::abs(Y_N[i]);

			if (val < it->metric)
			{ // Y_N[i] is weak, then save its position
				least_reliable_pos.pop_back(); // remove the last position that is not weak enough
				it = least_reliable_pos.insert(it, {val, i});
				break;
			}
		}
	}
}

template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::compute_test_vectors(const size_t frame_id)
{
	int dec_offset = frame_id * N_np; // dec add automaticaly an address offset to process the frame_id-th frame
	                                  // hard_Y_N. But there is only one frame so we need to substract a forecast
	                                  // address offset.

	for (int c = 0; c < n_test_vectors; c++)
	{
		// rearrange hard_Y_N to be a good candidate
		bit_flipping(hard_Y_N.data(), c);

		dec->decode_hiho_cw(hard_Y_N.data() - dec_offset, test_vect.data() + c*this->N - dec_offset, frame_id); // parity bit is ignored by the decoder
		//is_wrong[c] = !enc->is_codeword(test_vect.data() + c*this->N);
		is_wrong[c] = !dec->get_last_is_codeword(frame_id);

		if (this->parity_extended)
			test_vect[(c+1)*this->N -1] = tools::compute_parity(test_vect.data() + c*this->N, N_np);

#ifndef NDEBUG_TPC
		tools::Frame_trace<> ft(0,3,std::cerr);
	    std::cerr << "(II) Test vectors " << c << " before correction" << std::endl;
	    ft.display_bit_vector(hard_Y_N);
	    std::cerr << "(II) Test vectors " << c << " after correction : is wrong = " << is_wrong[c] << std::endl;
	    { std::vector<B> v(test_vect.data() + c*this->N, test_vect.data() + (c+1)*this->N); ft.display_bit_vector(v); }
#endif
		bit_flipping(hard_Y_N.data(), c); // apply again the bit flipping to recover the original hard_Y_N
	}
}

template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::compute_metrics(const R* Y_N)
{
	std::fill(metrics.begin(), metrics.end(), (R)0);

	n_good_competitors = 0;
	for (int c = 0; c < n_test_vectors; c++)
	{
		if (is_wrong[c])
		{
			metrics[c] = std::numeric_limits<R>::max()/2;
			continue;
		}
		n_good_competitors++;

		int tv_off = c*this->N;

		for (int i = 0; i < this->N; i++)
			if (hard_Y_N[i] ^ test_vect[tv_off + i])
				metrics[c] += std::abs(Y_N[i]);
	}

	// reorder metrics -> decided word is at first position of competitors list
	for (int c = 0; c < n_test_vectors; c++)
	{
		competitors[c].metric = metrics[c];
		competitors[c].pos    = c*this->N;
	}

	std::sort(competitors.begin(), competitors.end(), [](const info& a, const info& b) { return a.metric < b.metric; });


	// // remove duplicated metrics
	// unsigned start_pos = 0;
	// while ((int)competitors.size() > n_competitors && start_pos < (competitors.size() -1))
	// {
	// 	auto it = competitors.begin();
	// 	std::advance(it, start_pos +1);

	// 	// remove all duplications of the metric at position 'start_pos'
	// 	while (it != competitors.end())
	// 	{
	// 		if (it->metric == competitors[start_pos].metric)
	// 		{
	// 			it = competitors.erase(it);

	// 			if ((int)competitors.size() == n_competitors)
	// 				break;
	// 		}
	// 		else
	// 			it++;
	// 	}

	// 	start_pos++; // try with the next one
	// }

	// competitors.resize(n_test_vectors);


#ifndef NDEBUG_TPC
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
::compute_reliability(const R* Y_N1, R* Y_N2)
{
	const auto & DW = competitors.front(); // the Decided Word

	R beta_applied = this->beta;

	if (!this->beta_is_set)
	{
		// compute beta, the sum of the least reliable position reliabilities in the decided word
		beta_applied = 0;
		int max_sum = cp_coef[4] ? std::min((int)cp_coef[4], n_least_reliable_positions) : n_least_reliable_positions;
		for (int i = 0; i < max_sum; i++)
			beta_applied += least_reliable_pos[i].metric;

		beta_applied -= (R)cp_coef[2] * DW.metric;
	}


#ifndef NDEBUG_TPC
		std::cerr << "beta = " << beta_applied << ", DW.metric = " << DW.metric << std::endl;
#endif

	n_good_competitors = std::min(n_good_competitors, n_competitors); // if there is less than 'n_competitors' good competitors
	                                                                  // then take only them for reliability calculation

	for (int j = 1; j < n_good_competitors; j++)
		competitors[j].metric = (competitors[j].metric - DW.metric) * (R)cp_coef[1];


	for (int i = 0; i < this->N; i++)
	{
		const auto DB = test_vect[DW.pos + i]; // decided bit at the position i

		int j = 1;
		for (; j < n_good_competitors; j++)
			if (test_vect[competitors[j].pos + i] != DB)
				break;

		R reliability;

		if (j < n_good_competitors) // then the competitor j has a different bit at the position i
		{
			reliability = competitors[j].metric;
		}
		else if (beta_is_set)
		{
			reliability = this->beta;
		}
		else // same bits for each candidates
		{
			reliability = beta_applied + (R)cp_coef[3] * std::abs(Y_N1[i]);
			if (reliability < 0)
				reliability = 0;
		}

		if (DB) // if DB is a 1
			reliability = -reliability; // set as negative

#ifndef NDEBUG_TPC
		std::cerr << "Rel:  i = " << i << ", Y_N1 = " << Y_N1[i] << ", competitors[" << j << "].metric = " << competitors[j].metric;
#endif

		Y_N2[i] = reliability - (R)cp_coef[0] * Y_N1[i];

#ifndef NDEBUG_TPC
		std::cerr  << ", reliability = " << reliability << ", Y_N2 = " << Y_N2[i] << std::endl;
#endif
	}
}

template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::bit_flipping(B* hard_vect, const int c)
{
	for (int i = 0; i < n_least_reliable_positions; i++)
		hard_vect[least_reliable_pos[i].pos] = test_patterns[c][i] ? !hard_vect[least_reliable_pos[i].pos] : hard_vect[least_reliable_pos[i].pos];
}

template <typename B, typename R>
void Decoder_chase_pyndiah<B,R>
::generate_bit_flipping_candidates()
{
	test_patterns.resize(n_test_vectors, std::vector<bool>(n_least_reliable_positions, false));

	std::vector<int> cand(n_test_vectors, 0);

	if (n_test_vectors == (1 << n_least_reliable_positions))
	{
		std::iota(cand.begin(), cand.end(), 0);
	}
	else if (n_test_vectors == 16 && n_least_reliable_positions == 5)
	{ // 3 among 5
		cand[ 0] = 0;
		cand[ 1] = 1 << 0;
		cand[ 2] = 1 << 1;
		cand[ 3] = 1 << 2;
		cand[ 4] = 1 << 3;
		cand[ 5] = 1 << 4;
		cand[ 6] = 1 + (1 << 1);
		cand[ 7] = 1 + (1 << 2);
		cand[ 8] = 1 + (1 << 3);
		cand[ 9] = 1 + (1 << 4);
		cand[10] = 3 + (1 << 2);
		cand[11] = 3 + (1 << 3);
		cand[12] = 3 + (1 << 4);
		cand[13] = 5 + (1 << 3);
		cand[14] = 5 + (1 << 4);
		cand[15] = 9 + (1 << 4);
	}
	else if (n_test_vectors <= (n_least_reliable_positions * (n_least_reliable_positions + 1) / 2 + 1))
	{
		// (0) do not flip any bit -> 1 test vector
		// (1) flip one by one all least reliable positions -> 'n_least_reliable_positions' test vectors
		// (2) always flip the least realiable position and do (1) on the 'n_least_reliable_positions' -1 left bits -> 'n_least_reliable_positions' -1 test vectors
		// (3) always flip the two least reliable position and do (1) on the 'n_least_reliable_positions' -2 left bits -> 'n_least_reliable_positions' -2 test vectors
		// (4) and so on until reaching the wish number of test vectors with a maximum of the sum from 1 to 'n_least_reliable_positions' + 1

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


	// one 'test_patterns' element is the bit flipping pattern to apply on the least reliable positions
	// in this pattern, the first position is the instruction to flip or not the least reliable position
	// the second position is the instruction to flip the second least reliable position, and so on.
	for (int i = 0; i < n_test_vectors; i++)
		for (int j = 0; j < n_least_reliable_positions; j++)
			test_patterns[i][j] = ((cand[i] >> j) & (int)1) != 0;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_chase_pyndiah<B_8,Q_8>;
template class aff3ct::module::Decoder_chase_pyndiah<B_16,Q_16>;
template class aff3ct::module::Decoder_chase_pyndiah<B_32,Q_32>;
template class aff3ct::module::Decoder_chase_pyndiah<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_chase_pyndiah<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
