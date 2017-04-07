#include "Decoder_polar_SCAN_naive_sys.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R,
          tools::proto_i<R> I, tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H>
Decoder_polar_SCAN_naive_sys<B,R,I,F,V,H>
::Decoder_polar_SCAN_naive_sys(const int &K, const int &N, const int &max_iter, const mipp::vector<B> &frozen_bits,
                               const int n_frames, const std::string name)
: Decoder_polar_SCAN_naive<B,R,I,F,V,H>(K, N, max_iter, frozen_bits, n_frames, name)
{
}

template <typename B, typename R,
          tools::proto_i<R> I, tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H>
Decoder_polar_SCAN_naive_sys<B,R,I,F,V,H>
::~Decoder_polar_SCAN_naive_sys()
{
}

template <typename B, typename R,
          tools::proto_i<R> I, tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H>
void Decoder_polar_SCAN_naive_sys<B,R,I,F,V,H>
::_soft_decode(const R *sys, const R *par, R *ext)
{
	// ----------------------------------------------------------------------------------------------------------- LOAD
	this->_load_init();
	
	// init the softGraph (special case for the right most stage)
	auto sys_idx = 0, par_idx = 0;
	for (auto i = 0; i < this->N; i++)
		if (!this->frozen_bits[i])
			this->soft_graph[this->layers_count - 1][i] = sys[sys_idx++];
		else
			this->soft_graph[this->layers_count - 1][i] = par[par_idx++];

	// --------------------------------------------------------------------------------------------------------- DECODE
	Decoder_polar_SCAN_naive<B,R,I,F,V,H>::_decode();

	// ---------------------------------------------------------------------------------------------------------- STORE
	sys_idx = 0;
	for (auto i = 0; i < this->N; i++)
		if (!this->frozen_bits[i]) // if "i" is NOT a frozen bit (information bit = sytematic bit)
			ext[sys_idx++] = this->feedback_graph[this->layers_count -1][i];
}

template <typename B, typename R,
          tools::proto_i<R> I, tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H>
void Decoder_polar_SCAN_naive_sys<B,R,I,F,V,H>
::_soft_decode(const R *Y_N1, R *Y_N2)
{
	// ----------------------------------------------------------------------------------------------------------- LOAD
	this->_load(Y_N1);

	// --------------------------------------------------------------------------------------------------------- DECODE
	Decoder_polar_SCAN_naive<B,R,I,F,V,H>::_decode();

	// ---------------------------------------------------------------------------------------------------------- STORE
	for (auto i = 0; i < this->N; i++)
		Y_N2[i] = this->feedback_graph[this->layers_count -1][i];
}

template <typename B, typename R,
          tools::proto_i<R> I, tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H>
void Decoder_polar_SCAN_naive_sys<B,R,I,F,V,H>
::_store(B *V_N) const
{
	auto k = 0;
	for (auto i = 0; i < this->N; i++)
	{
		if (!this->frozen_bits[i]) // if i is not a frozen bit
			V_N[k++] = (H(this->feedback_graph[this->layers_count -1][i]) == 0) ? (B)0 : (B)1;
	}
}
}
}
