#include <string>
#include <algorithm>

#include "Module/Decoder/Polar/SCAN/Decoder_polar_SCAN_naive_sys.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
Decoder_polar_SCAN_naive_sys<B,R,F,V,H,I,S>
::Decoder_polar_SCAN_naive_sys(const int &K, const int &N, const int &max_iter, const std::vector<bool> &frozen_bits)
: Decoder_polar_SCAN_naive<B,R,F,V,H,I,S>(K, N, max_iter, frozen_bits)
{
	const std::string name = "Decoder_polar_SCAN_naive_sys";
	this->set_name(name);
}

template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
Decoder_polar_SCAN_naive_sys<B,R,F,V,H,I,S>* Decoder_polar_SCAN_naive_sys<B,R,F,V,H,I,S>
::clone() const
{
	auto m = new Decoder_polar_SCAN_naive_sys(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
int Decoder_polar_SCAN_naive_sys<B,R,F,V,H,I,S>
::_decode_siso(const R *sys, const R *par, R *ext, const size_t frame_id)
{
	// ----------------------------------------------------------------------------------------------------------- LOAD

	// init the softGraph (special case for the right most stage)
	auto sys_idx = 0, par_idx = 0;
	for (auto i = 0; i < this->N; i++)
		if (!this->frozen_bits[i])
			this->soft_graph[this->layers_count - 1][i] = sys[sys_idx++];
		else
			this->soft_graph[this->layers_count - 1][i] = par[par_idx++];

	// --------------------------------------------------------------------------------------------------------- DECODE
	this->_decode();

	// ---------------------------------------------------------------------------------------------------------- STORE
	sys_idx = 0;
	for (auto i = 0; i < this->N; i++)
		if (!this->frozen_bits[i]) // if "i" is NOT a frozen bit (information bit = sytematic bit)
			ext[sys_idx++] = this->feedback_graph[this->layers_count -1][i];

	return 0;
}

template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
int Decoder_polar_SCAN_naive_sys<B,R,F,V,H,I,S>
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

template <typename B, typename R,
          tools::proto_f<R> F, tools::proto_v<R> V, tools::proto_h<B,R> H, tools::proto_i<R> I, tools::proto_s<R> S>
void Decoder_polar_SCAN_naive_sys<B,R,F,V,H,I,S>
::_store(B *V_KN, bool coded) const
{
	if (!coded)
	{
		auto k = 0;
		for (auto i = 0; i < this->N; i++)
			if (!this->frozen_bits[i]) // if i is not a frozen bit
				V_KN[k++] = H(this->feedback_graph[this->layers_count -1][i] +
				              this->soft_graph    [this->layers_count -1][i]);
	}
	else
		for (auto i = 0; i < this->N; i++)
			V_KN[i] = H(this->feedback_graph[this->layers_count -1][i] +
			            this->soft_graph    [this->layers_count -1][i]);
}
}
}
