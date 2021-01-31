#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <algorithm>
#include <sstream>
#include <cmath>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Tools/Code/Polar/fb_assert.h"
#include "Module/Decoder/Polar/SCAN/Decoder_polar_SCAN_naive.hpp"

namespace aff3ct
{
namespace module
{
/********************************************************************/
/** CONSTRUCTOR **/
/********************************************************************/
template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>
::Decoder_polar_SCAN_naive(const int &K, const int &N, const int &max_iter, const std::vector<bool> &frozen_bits)
: Decoder_SISO<B,R>(K, N),
  m                ((int)std::log2(N)),
  max_iter         (max_iter         ),
  layers_count     (this->m +1       ),
  frozen_bits      (frozen_bits      ),
  feedback_graph   (layers_count     ),
  soft_graph       (layers_count     )
{
	const std::string name = "Decoder_polar_SCAN_naive";
	this->set_name(name);

	if (!tools::is_power_of_2(this->N))
	{
		std::stringstream message;
		message << "'N' has to be a power of 2 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N != (int)frozen_bits.size())
	{
		std::stringstream message;
		message << "'frozen_bits.size()' has to be equal to 'N' ('frozen_bits.size()' = " << frozen_bits.size()
		        << ", 'N' = " << N << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	auto k = 0; for (auto i = 0; i < this->N; i++) if (frozen_bits[i] == 0) k++;
	if (this->K != k)
	{
		std::stringstream message;
		message << "The number of information bits in the frozen_bits is invalid ('K' = " << K << ", 'k' = "
		        << k << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (max_iter <= 0)
	{
		std::stringstream message;
		message << "'max_iter' has to be greater than 0 ('max_iter' = " << max_iter << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto t = 0; t < layers_count; t++)
	{
		feedback_graph[t].resize(this->N);
		soft_graph[t].resize(this->N);
	}

	this->reset();
}

template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>* Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>
::clone() const
{
	auto m = new Decoder_polar_SCAN_naive(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
void Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>
::_reset(const size_t frame_id)
{
	// init feedback graph (special case for the left most stage)
	for (auto i = 0; i < this->N; i++)
		if (frozen_bits[i])// if i is a frozen bit
			feedback_graph[0][i] = S();
		else
			feedback_graph[0][i] = I();

	// init the rest of the feedback graph
	for (auto t = 1; t < layers_count; t++)
		for (auto i = 0; i < this->N; i++)
			feedback_graph[t][i] = I();

	// init the softGraph
	// (except for the layer "layers_count -1" because it will made by the "load" routine from the LLRs)
	for (auto t = 0; t < layers_count -1; t++)
		for (auto i = 0; i < this->N; i++)
			soft_graph[t][i] = I();
}

/********************************************************************/
/** load **/
/********************************************************************/
template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
void Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>
::_load(const R *Y_N)
{
	// init the softGraph (special case for the right most stage)
	for (auto i = 0; i < this->N; i++)
		soft_graph[layers_count - 1][i] = Y_N[i];
}

/********************************************************************/
/** frame_decode **/
/********************************************************************/
template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
void Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>
::_decode()
{
	for (auto iter = 0; iter < max_iter; iter++)
	{
		for (auto i = 0; i < this->N; i++)
		{
			/**********************/
			/* forward (right to left) propagation of soft information in the graph */
			fb_compute_soft_output(i);
			/**********************/

			if (frozen_bits[i])// if i is a frozen bit
			{
				feedback_graph[0][i] = S();
				soft_graph    [0][i] = S();
			}
			else
				feedback_graph[0][i] = I();

			/**********************/
			/* propagate back (left to right) the soft information in the graph */
			set_soft_val_and_propagate(0, i, feedback_graph[0][i]);
			/**********************/
		}
	}
}

template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
int Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>
::_decode_siho(const R *Y_N, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode();
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store(V_K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);

	return 0;
}

template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
int Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>
::_decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	this->_decode();
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store(V_N, true);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);

	return 0;
}

template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
int Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>
::_decode_siso(const R *Y_N1, R *Y_N2, const size_t frame_id)
{
	// ----------------------------------------------------------------------------------------------------------- LOAD
	this->_load(Y_N1);

	// --------------------------------------------------------------------------------------------------------- DECODE
	this->_decode();

	// ---------------------------------------------------------------------------------------------------------- STORE
	std::copy(this->feedback_graph[this->layers_count -1].begin(),
	          this->feedback_graph[this->layers_count -1].begin() + this->N,
	          Y_N2);

	return 0;
}

/********************************************************************/
/** frame store **/
/********************************************************************/
template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
void Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>
::_store(B *V_KN, bool coded) const
{
	if (!coded)
	{
		auto k = 0;
		for (auto i = 0; i < this->N; i++)
			if (!frozen_bits[i]) // if i is not a frozen bit
				V_KN[k++] = H(soft_graph[0][i]);
	}
	else
		for (auto i = 0; i < this->N; i++)
			V_KN[i] = H(this->feedback_graph[this->layers_count -1][i] +
			            this->soft_graph    [this->layers_count -1][i]);
}

/********************************************************************/
/** set_and_propagate **/
/** set the soft information (l,j) to v and propagate the value in feedbackGraph**/
/********************************************************************/
template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
void Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>
::set_soft_val_and_propagate(const int l, const int j, const R v)
{
	const auto up_ix = j - (1 << l);
	const auto down_ix = j;

	feedback_graph[l][j] = v; // set the bit in the feedback graph

	if ((j >> l) & 1) // propagate recursively if the l'th bit of j == 1
	{
		auto v1 = F(feedback_graph[l][up_ix  ], V(feedback_graph[l][down_ix], soft_graph[l + 1][down_ix]));
		auto v2 = V(feedback_graph[l][down_ix], F(feedback_graph[l][up_ix  ], soft_graph[l + 1][up_ix  ]));

		set_soft_val_and_propagate(l +1,   up_ix, v1); // recursive call
		set_soft_val_and_propagate(l +1, down_ix, v2); // recursive call
	}
}

/********************************************************************/
/** compute_soft_output **/
/********************************************************************/
template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
void Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>
::fb_compute_soft_output(const int &i)
{
	auto l_start = tools::compute_depth(i, this->m);

	for (auto l = l_start; l >= 0; l--) // for each layer
	{
		auto j_stop = 1 << (l);
		for (auto j = i; j < i + j_stop; j++)
		{
			auto f_up_ix   = j;
			auto f_down_ix = j + (1 << l);
			auto g_up_ix   = j - (1 << l);
			auto g_down_ix = j;

			if ((i & (1 << l)) == 0) // function F
				soft_graph[l][j] = F(soft_graph[l + 1][  f_up_ix],
				                     V(soft_graph[l + 1][f_down_ix], feedback_graph[l][f_down_ix]));
			else // function G
				soft_graph[l][j] = V(soft_graph[l + 1][g_down_ix],
				                     F(feedback_graph[l][  g_up_ix], soft_graph[l + 1][  g_up_ix]));
		}
	}
}

template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
void Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>
::set_n_frames(const size_t n_frames)
{
	if (n_frames != 1)
	{
		std::stringstream message;
		message << "'n_frames' has to be equal to 1 ('n_frames' = " << n_frames << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
		Decoder_SISO<B,R>::set_n_frames(n_frames);
}

template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
void Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>
::set_frozen_bits(const std::vector<bool>& fb)
{
	aff3ct::tools::fb_assert(frozen_bits, this->K, this->N);
	std::copy(fb.begin(), fb.end(), this->frozen_bits.begin());
}

template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
const std::vector<bool>& Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>
::get_frozen_bits() const
{
	return this->frozen_bits;
}

}
}
