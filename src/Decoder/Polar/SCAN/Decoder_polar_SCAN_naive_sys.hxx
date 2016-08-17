 #include "../decoder_polar_functions.h"

 #include "Decoder_polar_SCAN_naive_sys.hpp"

template <typename B, typename R,
          proto_i<R> I, proto_f<R> F, proto_v<R> V, proto_h<B,R> H>
Decoder_polar_SCAN_naive_sys<B,R,I,F,V,H>
::Decoder_polar_SCAN_naive_sys(const int &K, const int &m, const int &max_iter, const mipp::vector<B> &frozen_bits, 
                               const std::string name)
: Decoder_polar_SCAN_naive<B,R,I,F,V,H>(K, m, max_iter, frozen_bits, name)
{
}

template <typename B, typename R,
          proto_i<R> I, proto_f<R> F, proto_v<R> V, proto_h<B,R> H>
Decoder_polar_SCAN_naive_sys<B,R,I,F,V,H>
::~Decoder_polar_SCAN_naive_sys()
{
}

template <typename B, typename R,
          proto_i<R> I, proto_f<R> F, proto_v<R> V, proto_h<B,R> H>
void Decoder_polar_SCAN_naive_sys<B,R,I,F,V,H>
::decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext)
{
	assert(sys.size() + par.size() == (unsigned)this->N);

	// ----------------------------------------------------------------------------------------------------------- LOAD
	this->load_init();
	
	// init the softGraph (special case for the right most stage)
	auto sys_idx = 0, par_idx = 0;
	for (auto i = 0; i < this->N; i++)
		if (!this->frozen_bits[i])
			this->soft_graph[this->layers_count - 1][i] = sys[sys_idx++];
		else
			this->soft_graph[this->layers_count - 1][i] = par[par_idx++];

	// --------------------------------------------------------------------------------------------------------- DECODE
	Decoder_polar_SCAN_naive<B,R,I,F,V,H>::decode();

	// ---------------------------------------------------------------------------------------------------------- STORE
	sys_idx = 0;
	for (auto i = 0; i < this->N; i++)
		if (!this->frozen_bits[i]) // if "i" is NOT a frozen bit (information bit = sytematic bit)
			ext[sys_idx++] = this->feedback_graph[this->layers_count -1][i];
}

template <typename B, typename R,
          proto_i<R> I, proto_f<R> F, proto_v<R> V, proto_h<B,R> H>
void Decoder_polar_SCAN_naive_sys<B,R,I,F,V,H>
::store(mipp::vector<B>& V_N) const
{
	auto k = 0;
	for (auto i = 0; i < this->N; i++)
	{
		if (!this->frozen_bits[i]) // if i is not a frozen bit
			V_N[k++] = (H(this->feedback_graph[this->layers_count -1][i]) == 0) ? (B)0 : (B)1;
	}
}


template <typename B, typename R,
          proto_i<R> I, proto_f<R> F, proto_v<R> V, proto_h<B,R> H>
void Decoder_polar_SCAN_naive_sys<B,R,I,F,V,H>
::decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2)
{
	std::cerr << bold_red("(EE) This decoder does not support this interface.") << std::endl;
	std::exit(-1);
}
