#include <string>
#include <fstream>
#include <sstream>
#include <numeric>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/general_utils.h"

#include "Factory/Module/Puncturer/Puncturer.hpp"

#include "Module/Decoder/Decoder_SISO_SIHO.hpp"
#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"
#include "Module/Puncturer/LDPC/Puncturer_LDPC.hpp"

#include "Module/Codec/LDPC/Codec_LDPC.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Codec_LDPC<B,Q>
::Codec_LDPC(const factory::Encoder_LDPC  ::parameters &enc_params,
             const factory::Decoder_LDPC  ::parameters &dec_params,
                   factory::Puncturer_LDPC::parameters *pct_params)
: Codec          <B,Q>(enc_params.K, enc_params.N_cw, pct_params ? pct_params->N : enc_params.N_cw, enc_params.tail_length, enc_params.n_frames),
  Codec_SISO_SIHO<B,Q>(enc_params.K, enc_params.N_cw, pct_params ? pct_params->N : enc_params.N_cw, enc_params.tail_length, enc_params.n_frames)
{
	const std::string name = "Codec_LDPC";
	this->set_name(name);

	// ----------------------------------------------------------------------------------------------------- exceptions
	if (enc_params.K != dec_params.K)
	{
		std::stringstream message;
		message << "'enc_params.K' has to be equal to 'dec_params.K' ('enc_params.K' = " << enc_params.K
		        << ", 'dec_params.K' = " << dec_params.K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (enc_params.N_cw != dec_params.N_cw)
	{
		std::stringstream message;
		message << "'enc_params.N_cw' has to be equal to 'dec_params.N_cw' ('enc_params.N_cw' = " << enc_params.N_cw
		        << ", 'dec_params.N_cw' = " << dec_params.N_cw << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (enc_params.n_frames != dec_params.n_frames)
	{
		std::stringstream message;
		message << "'enc_params.n_frames' has to be equal to 'dec_params.n_frames' ('enc_params.n_frames' = "
		        << enc_params.n_frames << ", 'dec_params.n_frames' = " << dec_params.n_frames << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// ---------------------------------------------------------------------------------------------------------- tools
	if (enc_params.type == "LDPC")
	{
		G = tools::LDPC_matrix_handler::read(enc_params.G_path, &info_bits_pos);
	}
	else if (enc_params.type == "LDPC_DVBS2")
	{
		dvbs2 = tools::build_dvbs2(this->K, this->N);
		H     = tools::build_H(*dvbs2);
	}

	if (H.get_n_connections() == 0)
	{
		tools::LDPC_matrix_handler::Positions_vector* ibp = nullptr;
		std::vector<bool>* pct = nullptr;

		if (info_bits_pos.empty())
			ibp = &info_bits_pos;

		if (pct_params != nullptr && pct_params->pattern.empty())
			pct = &pct_params->pattern;

		H = tools::LDPC_matrix_handler::read(dec_params.H_path, ibp, pct);
	}

	if (dec_params.H_reorder != "NONE")
	{	// reorder the H matrix following the check node degrees
		H.sort_cols_per_density(dec_params.H_reorder == "ASC" ? tools::Matrix::Sort::ASCENDING : tools::Matrix::Sort::DESCENDING);
	}



	if (info_bits_pos.empty())
	{
		if (enc_params.type == "LDPC_H")
			this->set_encoder(factory::Encoder_LDPC::build<B>(enc_params, G, H));
	}
	else
	{
		tools::LDPC_matrix_handler::check_info_pos(info_bits_pos, enc_params.K, enc_params.N_cw);
	}

	// ---------------------------------------------------------------------------------------------------- allocations
	if (pct_params == nullptr)
	{
		factory::Puncturer::parameters pctno_params;
		pctno_params.type     = "NO";
		pctno_params.K        = enc_params.K;
		pctno_params.N        = enc_params.N_cw;
		pctno_params.N_cw     = enc_params.N_cw;
		pctno_params.n_frames = enc_params.n_frames;

		this->set_puncturer(factory::Puncturer::build<B,Q>(pctno_params));
	}
	else
	{
		try
		{
			this->set_puncturer(factory::Puncturer_LDPC::build<B,Q>(*pct_params));
		}
		catch (tools::cannot_allocate const&)
		{
			this->set_puncturer(factory::Puncturer::build<B,Q>(*pct_params));
		}
	}


	try
	{
		this->get_encoder();
	}
	catch (tools::runtime_error const&)
	{ // encoder not set when building encoder LDPC_H
		try
		{
			this->set_encoder(factory::Encoder_LDPC::build<B>(enc_params, G, H, *dvbs2));
		}
		catch(tools::cannot_allocate const&)
		{
			this->set_encoder(factory::Encoder::build<B>(enc_params));
		}
	}

	if (info_bits_pos.empty())
	{
		try
		{
			info_bits_pos = this->get_encoder()->get_info_bits_pos();
		}
		catch(tools::unimplemented_error const&)
		{
			// generate a default vector [0, 1, 2, 3, ..., K-1]
			info_bits_pos.resize(enc_params.K);
			std::iota(info_bits_pos.begin(), info_bits_pos.end(), 0);
		}
	}

	try
	{
		this->set_decoder_siso_siho(factory::Decoder_LDPC::build_siso<B,Q>(dec_params, H, info_bits_pos, this->get_encoder()));
	}
	catch (const std::exception&)
	{
		this->set_decoder_siho(factory::Decoder_LDPC::build<B,Q>(dec_params, H, info_bits_pos, this->get_encoder()));
	}
}

template <typename B, typename Q>
void Codec_LDPC<B,Q>
::_extract_sys_par(const Q *Y_N, Q *sys, Q *par, const int frame_id)
{
	const auto K    = this->K;
	const auto N_cw = this->N_cw;

	for (auto i = 0; i < K; i++)
		sys[i] = Y_N[info_bits_pos[i]];

	auto sys_idx = 0;
	for (auto i = 0; i < N_cw; i++)
		if (std::find(info_bits_pos.begin(), info_bits_pos.end(), i) != info_bits_pos.end())
		{
			par[sys_idx] = Y_N[i];
			sys_idx++;
		}
}

template <typename B, typename Q>
void Codec_LDPC<B,Q>
::_extract_sys_llr(const Q *Y_N, Q *Y_K, const int frame_id)
{
	for (auto i = 0; i < this->K; i++)
		Y_K[i] = Y_N[info_bits_pos[i]];
}

template <typename B, typename Q>
void Codec_LDPC<B,Q>
::_extract_sys_bit(const Q *Y_N, B *V_K, const int frame_id)
{
	for (auto i = 0; i < this->K; i++)
		V_K[i] = Y_N[info_bits_pos[i]] >= 0 ? (B)0 : (B)1;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Codec_LDPC<B_8,Q_8>;
template class aff3ct::module::Codec_LDPC<B_16,Q_16>;
template class aff3ct::module::Codec_LDPC<B_32,Q_32>;
template class aff3ct::module::Codec_LDPC<B_64,Q_64>;
#else
template class aff3ct::module::Codec_LDPC<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
