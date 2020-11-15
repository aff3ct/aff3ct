#include <algorithm>
#include <sstream>
#include <string>
#include <limits>
#include <cmath>

#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/Reorderer/Reorderer.hpp"
#include "Module/Decoder/LDPC/BP/Horizontal_layered/ONMS/Decoder_LDPC_BP_horizontal_layered_ONMS_inter.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R>
Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,R>
::Decoder_LDPC_BP_horizontal_layered_ONMS_inter(const int K, const int N, const int n_ite,
                                                const tools::Sparse_matrix &_H,
                                                const std::vector<unsigned> &info_bits_pos,
                                                const float normalize_factor,
                                                const R offset,
                                                const bool enable_syndrome,
                                                const int syndrome_depth)
: Decoder_SISO<B,R>(K, N                                                                               ),
  Decoder_LDPC_BP  (K, N, n_ite, _H, enable_syndrome, syndrome_depth                                   ),
  normalize_factor (normalize_factor                                                                   ),
  offset           (offset                                                                             ),
  contributions    (this->H.get_cols_max_degree()                                                      ),
  saturation       ((R)((1 << ((sizeof(R) * 8 -2) - (int)std::log2(this->H.get_rows_max_degree()))) -1)),
  info_bits_pos    (info_bits_pos                                                                      ),
  var_nodes        (this->get_n_waves(), mipp::vector<mipp::Reg<R>>(N)                                 ),
  branches         (this->get_n_waves(), mipp::vector<mipp::Reg<R>>(this->H.get_n_connections())       ),
  Y_N_reorderered  (N                                                                                  ),
  V_reorderered    (N                                                                                  )
{
	const std::string name = "Decoder_LDPC_BP_horizontal_layered_ONMS_inter";
	this->set_name(name);
	this->set_n_frames_per_wave(mipp::N<R>());

	tools::check_LUT(info_bits_pos, "info_bits_pos", (size_t)K);

	if (sizeof(R) == 1)
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "This decoder does not work in 8-bit fixed-point.");

	if (saturation <= 0)
	{
		std::stringstream message;
		message << "'saturation' has to be greater than 0 ('saturation' = " << saturation << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->reset();
}

template <typename B, typename R>
Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,R>* Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,R>
::clone() const
{
	auto m = new Decoder_LDPC_BP_horizontal_layered_ONMS_inter(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R>
void Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,R>
::_reset(const size_t frame_id)
{
	const auto cur_wave = frame_id / this->get_n_frames_per_wave();
	const auto zero = mipp::Reg<R>((R)0);
	std::fill(this->branches [cur_wave].begin(), this->branches [cur_wave].end(), zero);
	std::fill(this->var_nodes[cur_wave].begin(), this->var_nodes[cur_wave].end(), zero);
}

template <typename B, typename R>
void Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,R>
::_load(const R *Y_N, const size_t frame_id)
{
	const auto cur_wave = frame_id / this->get_n_frames_per_wave();

	std::vector<const R*> frames(mipp::N<R>());
	for (auto f = 0; f < mipp::N<R>(); f++) frames[f] = Y_N + f * this->N;
	tools::Reorderer_static<R,mipp::N<R>()>::apply(frames, (R*)this->Y_N_reorderered.data(), this->N);

	for (auto i = 0; i < (int)var_nodes[cur_wave].size(); i++)
		this->var_nodes[cur_wave][i] += this->Y_N_reorderered[i]; // var_nodes contain previous extrinsic information
}

template <typename B, typename R>
int Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,R>
::_decode_siso(const R *Y_N1, int8_t *CWD, R *Y_N2, const size_t frame_id)
{
	// memory zones initialization
	this->_load(Y_N1, frame_id);

	// actual decoding
	int status = 0;
	if (typeid(R) == typeid(short) || typeid(R) == typeid(signed char))
	{
		     if (normalize_factor == 0.125f) status = this->_decode<1>(frame_id);
		else if (normalize_factor == 0.250f) status = this->_decode<2>(frame_id);
		else if (normalize_factor == 0.375f) status = this->_decode<3>(frame_id);
		else if (normalize_factor == 0.500f) status = this->_decode<4>(frame_id);
		else if (normalize_factor == 0.625f) status = this->_decode<5>(frame_id);
		else if (normalize_factor == 0.750f) status = this->_decode<6>(frame_id);
		else if (normalize_factor == 0.875f) status = this->_decode<7>(frame_id);
		else if (normalize_factor == 1.000f) status = this->_decode<8>(frame_id);
		else
		{
			std::stringstream message;
			message << "'normalize_factor' can only be 0.125f, 0.250f, 0.375f, 0.500f, 0.625f, 0.750f, 0.875f or 1.000f"
			        << " ('normalize_factor' = " << normalize_factor << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}
	else // float or double
	{
		if (normalize_factor == 1.000f) status = this->_decode<8>(frame_id);
		else                            status = this->_decode<0>(frame_id);
	}

	// prepare for next round by processing extrinsic information
	const auto cur_wave = frame_id / this->get_n_frames_per_wave();
	for (auto v = 0; v < this->N; v++)
		this->var_nodes[cur_wave][v] -= Y_N_reorderered[v];

	std::vector<R*> frames(mipp::N<R>());
	for (auto f = 0; f < mipp::N<R>(); f++) frames[f] = Y_N2 + f * this->N;
	tools::Reorderer_static<R,mipp::N<R>()>::apply_rev((R*)this->var_nodes[cur_wave].data(), frames, this->N);

	for (auto f = 0; f < mipp::N<R>() ; f++)
		CWD[f] = !((status >> (mipp::N<R>() -1 -f)) & 1);
	return status;
}

template <typename B, typename R>
int Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,R>
::_decode_siho(const R *Y_N, int8_t *CWD, B *V_K, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N, frame_id);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	int status = 0;
	if (typeid(R) == typeid(short) || typeid(R) == typeid(signed char))
	{
		     if (normalize_factor == 0.125f) status = this->_decode<1>(frame_id);
		else if (normalize_factor == 0.250f) status = this->_decode<2>(frame_id);
		else if (normalize_factor == 0.375f) status = this->_decode<3>(frame_id);
		else if (normalize_factor == 0.500f) status = this->_decode<4>(frame_id);
		else if (normalize_factor == 0.625f) status = this->_decode<5>(frame_id);
		else if (normalize_factor == 0.750f) status = this->_decode<6>(frame_id);
		else if (normalize_factor == 0.875f) status = this->_decode<7>(frame_id);
		else if (normalize_factor == 1.000f) status = this->_decode<8>(frame_id);
		else
		{
			std::stringstream message;
			message << "'normalize_factor' can only be 0.125f, 0.250f, 0.375f, 0.500f, 0.625f, 0.750f, 0.875f or 1.000f"
			        << " ('normalize_factor' = " << normalize_factor << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}
	else // float or double
	{
		if (normalize_factor == 1.000f) status = this->_decode<8>(frame_id);
		else                            status = this->_decode<0>(frame_id);
	}
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	// take the hard decision
	const auto cur_wave = frame_id / this->get_n_frames_per_wave();
	const auto zero = mipp::Reg<R>((R)0);
	for (auto i = 0; i < this->K; i++)
	{
		const auto k = this->info_bits_pos[i];
		V_reorderered[i] = mipp::cast<R,B>(this->var_nodes[cur_wave][k]) >> (sizeof(B) * 8 - 1);
	}

	std::vector<B*> frames(mipp::N<R>());
	for (auto f = 0; f < mipp::N<R>(); f++) frames[f] = V_K + f * this->K;
	tools::Reorderer_static<B,mipp::N<R>()>::apply_rev((B*)V_reorderered.data(), frames, this->K);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
//	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);

	for (auto f = 0; f < mipp::N<R>() ; f++)
		CWD[f] = !((status >> (mipp::N<R>() -1 -f)) & 1);
	return status;
}

template <typename B, typename R>
int Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,R>
::_decode_siho_cw(const R *Y_N, int8_t *CWD, B *V_N, const size_t frame_id)
{
//	auto t_load = std::chrono::steady_clock::now(); // ----------------------------------------------------------- LOAD
	this->_load(Y_N, frame_id);
//	auto d_load = std::chrono::steady_clock::now() - t_load;

//	auto t_decod = std::chrono::steady_clock::now(); // -------------------------------------------------------- DECODE
	// actual decoding
	int status = 0;
	if (typeid(R) == typeid(short) || typeid(R) == typeid(signed char))
	{
		     if (normalize_factor == 0.125f) status = this->_decode<1>(frame_id);
		else if (normalize_factor == 0.250f) status = this->_decode<2>(frame_id);
		else if (normalize_factor == 0.375f) status = this->_decode<3>(frame_id);
		else if (normalize_factor == 0.500f) status = this->_decode<4>(frame_id);
		else if (normalize_factor == 0.625f) status = this->_decode<5>(frame_id);
		else if (normalize_factor == 0.750f) status = this->_decode<6>(frame_id);
		else if (normalize_factor == 0.875f) status = this->_decode<7>(frame_id);
		else if (normalize_factor == 1.000f) status = this->_decode<8>(frame_id);
		else
		{
			std::stringstream message;
			message << "'normalize_factor' can only be 0.125f, 0.250f, 0.375f, 0.500f, 0.625f, 0.750f, 0.875f or 1.000f"
			        << " ('normalize_factor' = " << normalize_factor << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}
	else // float or double
	{
		if (normalize_factor == 1.000f) this->_decode<8>(frame_id);
		else                            this->_decode<0>(frame_id);
	}
//	auto d_decod = std::chrono::steady_clock::now() - t_decod;

//	auto t_store = std::chrono::steady_clock::now(); // --------------------------------------------------------- STORE
	// take the hard decision
	const auto cur_wave = frame_id / this->get_n_frames_per_wave();
	const auto zero = mipp::Reg<R>((R)0);
	for (auto v = 0; v < this->N; v++)
		V_reorderered[v] = mipp::cast<R,B>(this->var_nodes[cur_wave][v]) >> (sizeof(B) * 8 - 1);

	std::vector<B*> frames(mipp::N<R>());
	for (auto f = 0; f < mipp::N<R>(); f++) frames[f] = V_N + f * this->N;
	tools::Reorderer_static<B,mipp::N<R>()>::apply_rev((B*)V_reorderered.data(), frames, this->N);
//	auto d_store = std::chrono::steady_clock::now() - t_store;

//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
//	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);

	for (auto f = 0; f < mipp::N<R>() ; f++)
		CWD[f] = !((status >> (mipp::N<R>() -1 -f)) & 1);
	return status;
}

template <typename B, typename R>
template <int F>
int Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,R>
::_decode(const size_t frame_id)
{
	const auto cur_wave = frame_id / this->get_n_frames_per_wave();

	auto cur_syndrome_depth = 0;
	int packed_synd = 0;
	for (auto ite = 0; ite < this->n_ite; ite++)
	{
		this->_decode_single_ite<F>(this->var_nodes[cur_wave], this->branches[cur_wave]);

		// stop criterion
		if (this->enable_syndrome && (packed_synd = this->_check_syndrome_status(frame_id)) == status_t::SUCCESS)
		{
			cur_syndrome_depth++;
			if (cur_syndrome_depth == this->syndrome_depth)
				break;
		}
		else
			cur_syndrome_depth = 0;
	}

	return packed_synd;
}

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------- SIMD TOOLS

// --------------------------------------------------------------------------------------------------------- saturation
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

// ------------------------------------------------------------------------------------------------------ normalization
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
void Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,R>
::_decode_single_ite(mipp::vector<mipp::Reg<R>> &var_nodes, mipp::vector<mipp::Reg<R>> &branches)
{
	auto kr = 0;
	auto kw = 0;

	const auto zero_msk    = mipp::Msk<mipp::N<B>()>(false);
	const auto zero        = mipp::Reg<R>((R)0);
	const auto n_chk_nodes = (int)H.get_n_cols();
	for (auto c = 0; c < n_chk_nodes; c++)
	{
		auto sign = zero_msk;
		auto min1 = mipp::Reg<R>(std::numeric_limits<R>::max());
		auto min2 = mipp::Reg<R>(std::numeric_limits<R>::max());

		const auto chk_degree = (int)this->H[c].size();
		for (auto v = 0; v < chk_degree; v++)
		{
			contributions[v]    = var_nodes[this->H[c][v]] - branches[kr++];
			const auto var_abs  = mipp::abs (contributions[v]);
			const auto var_sign = mipp::sign(contributions[v]);
			const auto tmp      = min1;

			sign ^= var_sign;
			min1  = mipp::min(min1,           var_abs      );
			min2  = mipp::min(min2, mipp::max(var_abs, tmp));
		}

		auto cste1 = simd_sat<R>(simd_normalize<R,F>(min2 - offset, normalize_factor), saturation);
		auto cste2 = simd_sat<R>(simd_normalize<R,F>(min1 - offset, normalize_factor), saturation);

		cste1 = mipp::blend(zero, cste1, zero > cste1);
		cste2 = mipp::blend(zero, cste2, zero > cste2);

		for (auto v = 0; v < chk_degree; v++)
		{
			const auto var_val = contributions[v];
			const auto var_abs = mipp::abs(var_val);
			      auto res_abs = mipp::blend(cste1, cste2, var_abs == min1);
			const auto res_sng = sign ^ mipp::sign(var_val);
			const auto res     = mipp::copysign(res_abs, res_sng);

			branches[kw++] = res;
			var_nodes[this->H[c][v]] = contributions[v] + res;
		}
	}
}

template <typename B, typename R>
bool Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,R>
::_check_syndrome(const size_t frame_id)
{
	const auto cur_wave = frame_id / this->get_n_frames_per_wave();
	const auto zero = mipp::Msk<mipp::N<B>()>(false);
	auto syndrome = zero;

	auto n_chk_nodes = (int)H.get_n_cols();
	auto c = 0;
	auto syndrome_scalar = true;
	while (c < n_chk_nodes && (syndrome_scalar = mipp::testz(syndrome)))
	{
		auto sign = zero;
		const auto chk_degree = (int)this->H[c].size();
		for (auto v = 0; v < chk_degree; v++)
		{
			const auto value = this->var_nodes[cur_wave][this->H[c][v]];
			sign ^= mipp::sign(value);
		}

		syndrome |= sign;
		c++;
	}

	return syndrome_scalar;
}

template <typename B, typename R>
int Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,R>
::_check_syndrome_status(const size_t frame_id)
{
	const auto cur_wave = frame_id / this->get_n_frames_per_wave();
	const auto zero = mipp::Msk<mipp::N<B>()>(false);
	auto syndrome = zero;

	auto n_chk_nodes = (int)H.get_n_cols();
	auto c = 0;
	auto syndrome_scalar = true;
	while (c < n_chk_nodes)
	{
		auto sign = zero;
		const auto chk_degree = (int)this->H[c].size();
		for (auto v = 0; v < chk_degree; v++)
		{
			const auto value = this->var_nodes[cur_wave][this->H[c][v]];
			sign ^= mipp::sign(value);
		}

		syndrome |= sign;
		c++;
	}

	syndrome_scalar = mipp::testz(syndrome);

	if (!syndrome_scalar)
	{
		int packed_synd = 0;
		for (auto n = 0; n < mipp::N<B>(); n++)
		{
			packed_synd <<= 1;
			packed_synd |= (int)syndrome[n];
		}
		return packed_synd;
	}
	else
		return status_t::SUCCESS;
}

template <typename B, typename R>
void Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,R>
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Decoder_SISO<B,R>::set_n_frames(n_frames);

		const auto vec_size = this->var_nodes[0].size();
		this->var_nodes.resize(this->get_n_waves(), mipp::vector<mipp::Reg<R>>(vec_size));

		const auto vec_size2 = this->branches[0].size();
		this->branches.resize(this->get_n_waves(), mipp::vector<mipp::Reg<R>>(vec_size2));
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B_8,Q_8>;
template class aff3ct::module::Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B_16,Q_16>;
template class aff3ct::module::Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B_32,Q_32>;
template class aff3ct::module::Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B_64,Q_64>;
#else
template class aff3ct::module::Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
