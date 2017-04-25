#include <limits>
#include <cmath>
#include <iostream>

#include "Tools/Math/utils.h"
#include "Tools/Perf/Reorderer/Reorderer.hpp"

#include "Decoder_LDPC_BP_layered_ONMS_inter.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Decoder_LDPC_BP_layered_ONMS_inter<B,R>
::Decoder_LDPC_BP_layered_ONMS_inter(const int &K, const int &N, const int& n_ite,
                                     const AList_reader &alist_data,
                                     const mipp::vector<B> &info_bits_pos,
                                     const float normalize_factor,
                                     const float offset,
                                     const bool enable_syndrome,
                                     const int syndrome_depth,
                                     const int n_frames,
                                     const std::string name)
: Decoder_SISO<B,R>(K, N, n_frames, mipp::nElReg<R>(), name                                        ),
  cur_wave         (0                                                                              ),
  normalize_factor (normalize_factor                                                               ),
  offset           ((R)offset                                                                      ),
  contributions    (alist_data.get_CN_max_degree()                                                 ),
  saturation       ((1 << ((sizeof(R) * 8 -2) - (int)std::log2(alist_data.get_VN_max_degree()))) -1),
  n_ite            (n_ite                                                                          ),
  n_C_nodes        ((int)alist_data.get_n_CN()                                                     ),
  enable_syndrome  (enable_syndrome                                                                ),
  syndrome_depth   (syndrome_depth                                                                 ),
  init_flag        (true                                                                           ),
  info_bits_pos    (info_bits_pos                                                                  ),
  CN_to_VN         (alist_data.get_CN_to_VN()                                                      ),
  var_nodes        (this->n_dec_waves, mipp::vector<mipp::Reg<R>>(N                          )     ),
  branches         (this->n_dec_waves, mipp::vector<mipp::Reg<R>>(alist_data.get_n_branches())     ),
  Y_N_reorderered  (N                                                                              ),
  V_K_reorderered  (K                                                                              )
{
	if (n_ite <= 0)
		throw std::invalid_argument("aff3ct::module::Decoder_LDPC_BP_layered_ONMS_inter: \"n_ite\" has to be greater "
		                            "than 0.");
	if (syndrome_depth <= 0)
		throw std::invalid_argument("aff3ct::module::Decoder_LDPC_BP_layered_ONMS_inter: \"syndrome_depth\" has to be "
		                            "greater than 0.");
	if (N != (int)alist_data.get_n_VN())
		throw std::invalid_argument("aff3ct::module::Decoder_LDPC_BP_layered_ONMS_inter: \"N\" is not compatible with "
		                            "the alist file.");
	if (typeid(R) == typeid(signed char))
		throw std::runtime_error("aff3ct::module::Decoder_LDPC_BP_layered_ONMS_inter: this decoder does not work in "
		                         "8-bit fixed-point (try in 16-bit).");
	if (saturation <= 0)
		throw std::runtime_error("aff3ct::module::Decoder_LDPC_BP_layered_ONMS_inter: \"saturation\" has to be "
		                         "greater than 0.");
}

template <typename B, typename R>
Decoder_LDPC_BP_layered_ONMS_inter<B,R>
::~Decoder_LDPC_BP_layered_ONMS_inter()
{
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered_ONMS_inter<B,R>
::_soft_decode(const R *Y_N1, R *Y_N2)
{
	// memory zones initialization
	this->_load(Y_N1);

	// actual decoding
	if (typeid(R) == typeid(short) || typeid(R) == typeid(signed char))
	{
		     if (normalize_factor == 0.125f) this->BP_decode<1>();
		else if (normalize_factor == 0.250f) this->BP_decode<2>();
		else if (normalize_factor == 0.375f) this->BP_decode<3>();
		else if (normalize_factor == 0.500f) this->BP_decode<4>();
		else if (normalize_factor == 0.625f) this->BP_decode<5>();
		else if (normalize_factor == 0.750f) this->BP_decode<6>();
		else if (normalize_factor == 0.875f) this->BP_decode<7>();
		else if (normalize_factor == 1.000f) this->BP_decode<8>();
		else
			throw std::invalid_argument("aff3ct::module::Decoder_LDPC_BP_layered_ONMS_inter: \"normalize_factor\" can "
			                            "only be 0.125f, 0.250f, 0.375f, 0.500f, 0.625f, 0.750f, 0.875f or 1.000f.");
	}
	else // float or double
	{
		if (normalize_factor == 1.000f) this->BP_decode<8>();
		else                            this->BP_decode<0>();
	}

	// prepare for next round by processing extrinsic information
	for (auto i = 0; i < this->N; i++)
		this->var_nodes[cur_wave][i] -= Y_N_reorderered[i];

	std::vector<R*> frames(mipp::nElReg<R>());
	for (auto f = 0; f < mipp::nElReg<R>(); f++) frames[f] = Y_N2 + f * this->N;
	Reorderer_static<R,mipp::nElReg<R>()>::apply_rev((R*)this->var_nodes[cur_wave].data(), frames, this->N);

	cur_wave = (cur_wave +1) % this->n_dec_waves;
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered_ONMS_inter<B,R>
::_load(const R *Y_N)
{
	// memory zones initialization
	if (this->init_flag)
	{
		const auto zero = mipp::Reg<R>((R)0);
		std::fill(this->branches [cur_wave].begin(), this->branches [cur_wave].end(), zero);
		std::fill(this->var_nodes[cur_wave].begin(), this->var_nodes[cur_wave].end(), zero);

		if (cur_wave == this->n_dec_waves -1) this->init_flag = false;
	}

	std::vector<const R*> frames(mipp::nElReg<R>());
	for (auto f = 0; f < mipp::nElReg<R>(); f++) frames[f] = Y_N + f * this->N;
	Reorderer_static<R,mipp::nElReg<R>()>::apply(frames, (R*)this->Y_N_reorderered.data(), this->N);

	for (auto i = 0; i < (int)var_nodes[cur_wave].size(); i++)
		this->var_nodes[cur_wave][i] += this->Y_N_reorderered[i]; // var_nodes contain previous extrinsic information
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered_ONMS_inter<B,R>
::_hard_decode(const R *Y_N, B *V_K)
{
	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N);
	auto d_load = std::chrono::steady_clock::now() - t_load;

	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	if (typeid(R) == typeid(short) || typeid(R) == typeid(signed char))
	{
		     if (normalize_factor == 0.125f) this->BP_decode<1>();
		else if (normalize_factor == 0.250f) this->BP_decode<2>();
		else if (normalize_factor == 0.375f) this->BP_decode<3>();
		else if (normalize_factor == 0.500f) this->BP_decode<4>();
		else if (normalize_factor == 0.625f) this->BP_decode<5>();
		else if (normalize_factor == 0.750f) this->BP_decode<6>();
		else if (normalize_factor == 0.875f) this->BP_decode<7>();
		else if (normalize_factor == 1.000f) this->BP_decode<8>();
		else
			throw std::invalid_argument("aff3ct::module::Decoder_LDPC_BP_layered_ONMS_inter: \"normalize_factor\" can "
			                            "only be 0.125f, 0.250f, 0.375f, 0.500f, 0.625f, 0.750f, 0.875f or 1.000f.");
	}
	else // float or double
	{
		if (normalize_factor == 1.000f) this->BP_decode<8>();
		else                            this->BP_decode<0>();
	}

	auto d_decod = std::chrono::steady_clock::now() - t_decod;

	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	this->_store(V_K);
	auto d_store = std::chrono::steady_clock::now() - t_store;

	this->d_load_total  += d_load;
	this->d_decod_total += d_decod;
	this->d_store_total += d_store;
}

template <typename B, typename R>
void Decoder_LDPC_BP_layered_ONMS_inter<B,R>
::_store(B *V_K)
{
	// take the hard decision
	const auto zero = mipp::Reg<R>((R)0);
	for (auto i = 0; i < this->K; i++)
	{
		const auto k = this->info_bits_pos[i];
		V_K_reorderered[i] = mipp::cast<R,B>(zero > this->var_nodes[cur_wave][k]);
	}

	std::vector<B*> frames(mipp::nElReg<R>());
	for (auto f = 0; f < mipp::nElReg<R>(); f++) frames[f] = V_K + f * this->K;
	Reorderer_static<B,mipp::nElReg<R>()>::apply_rev((B*)V_K_reorderered.data(), frames, this->K);

	cur_wave = (cur_wave +1) % this->n_dec_waves;

	// set the flag so the branches can be reset to 0 only at the beginning of the loop in iterative decoding
	if (cur_wave == 0) this->init_flag = true;
}

// BP algorithm
template <typename B, typename R>
template <int F>
void Decoder_LDPC_BP_layered_ONMS_inter<B,R>
::BP_decode()
{
	auto cur_syndrome_depth = 0;

	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		this->BP_process<F>(this->var_nodes[cur_wave], this->branches[cur_wave]);

		// stop criterion
		if (this->enable_syndrome && this->check_syndrome())
		{
			cur_syndrome_depth++;
			if (cur_syndrome_depth == this->syndrome_depth)
				break;
		}
		else
			cur_syndrome_depth = 0;
	}
}

template <typename B, typename R>
bool Decoder_LDPC_BP_layered_ONMS_inter<B,R>
::check_syndrome()
{
	auto syndrome = mipp::Reg<B>((B)0);

	auto k = 0;
	for (auto i = 0; i < this->n_C_nodes; i++)
	{
		auto sign = mipp::Reg<B>((B)0);

		const auto n_VN = (int)this->CN_to_VN[i].size();
		for (auto j = 0; j < n_VN; j++)
		{
			const auto value = this->var_nodes[cur_wave][this->CN_to_VN[i][j]] - this->branches[cur_wave][k++];
			sign ^= mipp::cast<R,B>(mipp::sign(value));
		}

		syndrome |= sign;
	}

	auto i = 0;
	while (i < mipp::nElReg<B>() && syndrome[i] == 0) i++;
	return (i == mipp::nElReg<B>());
}

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------- SIMD TOOLS

//                                                                                                           saturation
template <typename R>
inline mipp::Reg<R> simd_sat(const mipp::Reg<R> val, const R saturation)
{
	return val;
}
template <>
inline mipp::Reg<short> simd_sat(const mipp::Reg<short> v, const short s)
{
	return mipp::sat(v, (short)-s, (short)+s);
}

//                                                                                                        normalization
template <typename R, int F = 0> inline mipp::Reg<R> simd_normalize(const mipp::Reg<R> val, const float factor)
{
	return val * mipp::Reg<R>((R)factor);
}
template <> inline mipp::Reg<short > simd_normalize<short, 1>(const mipp::Reg<short > v, const float f) { return (v >> 3);                       } // v * 0.125
template <> inline mipp::Reg<short > simd_normalize<short, 2>(const mipp::Reg<short > v, const float f) { return            (v >> 2);            } // v * 0.250
template <> inline mipp::Reg<short > simd_normalize<short, 3>(const mipp::Reg<short > v, const float f) { return (v >> 3) + (v >> 2);            } // v * 0.375
template <> inline mipp::Reg<short > simd_normalize<short, 4>(const mipp::Reg<short > v, const float f) { return                       (v >> 1); } // v * 0.500
template <> inline mipp::Reg<short > simd_normalize<short, 5>(const mipp::Reg<short > v, const float f) { return (v >> 3) +            (v >> 1); } // v * 0.625
template <> inline mipp::Reg<short > simd_normalize<short, 6>(const mipp::Reg<short > v, const float f) { return            (v >> 2) + (v >> 1); } // v * 0.750
template <> inline mipp::Reg<short > simd_normalize<short, 7>(const mipp::Reg<short > v, const float f) { return (v >> 3) + (v >> 2) + (v >> 1); } // v * 0.825
template <> inline mipp::Reg<short > simd_normalize<short, 8>(const mipp::Reg<short > v, const float f) { return v;                              } // v * 1.000
template <> inline mipp::Reg<float > simd_normalize<float, 8>(const mipp::Reg<float > v, const float f) { return v;                              } // v * 1.000
template <> inline mipp::Reg<double> simd_normalize<double,8>(const mipp::Reg<double> v, const float f) { return v;                              } // v * 1.000

// --------------------------------------------------------------------------------------------------------- SIMD TOOLS
// --------------------------------------------------------------------------------------------------------------------

// BP algorithm
template <typename B, typename R>
template <int F>
void Decoder_LDPC_BP_layered_ONMS_inter<B,R>
::BP_process(mipp::vector<mipp::Reg<R>> &var_nodes, mipp::vector<mipp::Reg<R>> &branches)
{
	auto kr = 0;
	auto kw = 0;

	const auto zero = mipp::Reg<R>((R)0);
	for (auto i = 0; i < this->n_C_nodes; i++)
	{
		auto sign = mipp::Reg<B>((B)0);
		auto min1 = mipp::Reg<R>(std::numeric_limits<R>::max());
		auto min2 = mipp::Reg<R>(std::numeric_limits<R>::max());

		const auto n_VN = (int)this->CN_to_VN[i].size();
		for (auto j = 0; j < n_VN; j++)
		{
			contributions[j]  = var_nodes[this->CN_to_VN[i][j]] - branches[kr++];
			const auto v_abs  = mipp::abs (contributions[j]);
			const auto c_sign = mipp::sign(contributions[j]);
			const auto v_temp = min1;

			sign ^= mipp::cast<R,B>(c_sign);
			min1  = mipp::min(min1,           v_abs         );
			min2  = mipp::min(min2, mipp::max(v_abs, v_temp));
		}

		auto cste1 = simd_sat<R>(simd_normalize<R,F>(min2 - offset, normalize_factor), saturation);
		auto cste2 = simd_sat<R>(simd_normalize<R,F>(min1 - offset, normalize_factor), saturation);

		cste1 = mipp::blend(zero, cste1, zero > cste1);
		cste2 = mipp::blend(zero, cste2, zero > cste2);

		for (auto j = 0; j < n_VN; j++)
		{
			const auto value = contributions[j];
			const auto v_abs = mipp::abs(value);
			      auto v_res = mipp::blend(cste1, cste2, v_abs == min1);
			const auto v_sig = sign ^ mipp::cast<R,B>(mipp::sign(value));
			           v_res = mipp::copysign(v_res, mipp::cast<B,R>(v_sig));

			branches[kw++] = v_res;
			var_nodes[this->CN_to_VN[i][j]] = contributions[j] + v_res;
		}
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP_layered_ONMS_inter<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP_layered_ONMS_inter<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP_layered_ONMS_inter<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP_layered_ONMS_inter<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP_layered_ONMS_inter<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
