#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <numeric>
#include <string>

#include "Tools/Exception/exception.hpp"
#include "Module/Extractor/LDPC/Extractor_LDPC.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"
#include "Factory/Module/Puncturer/Puncturer.hpp"
#include "Tools/Codec/LDPC/Codec_LDPC.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_LDPC<B,Q>
::Codec_LDPC(const factory::Encoder_LDPC   &enc_params,
             const factory::Decoder_LDPC   &dec_params,
                   factory::Puncturer_LDPC *pct_params)
: Codec_SISO<B,Q>(enc_params.K, enc_params.N_cw, pct_params ? pct_params->N : enc_params.N_cw),
  H(new Sparse_matrix()),
  G(new Sparse_matrix()),
  info_bits_pos(new LDPC_matrix_handler::Positions_vector())
{
	// ----------------------------------------------------------------------------------------------------- exceptions
	if (enc_params.K != dec_params.K)
	{
		std::stringstream message;
		message << "'enc_params.K' has to be equal to 'dec_params.K' ('enc_params.K' = " << enc_params.K
		        << ", 'dec_params.K' = " << dec_params.K << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (enc_params.N_cw != dec_params.N_cw)
	{
		std::stringstream message;
		message << "'enc_params.N_cw' has to be equal to 'dec_params.N_cw' ('enc_params.N_cw' = " << enc_params.N_cw
		        << ", 'dec_params.N_cw' = " << dec_params.N_cw << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// ---------------------------------------------------------------------------------------------------------- tools
	if (enc_params.type == "LDPC")
	{
		*G = LDPC_matrix_handler::read(enc_params.G_path, info_bits_pos.get());
	}
	else if (enc_params.type == "LDPC_DVBS2")
	{
		dvbs2 = build_dvbs2(this->K, this->N);
		*H    = build_H(*dvbs2);
	}

	if (H->get_n_connections() == 0)
	{
		LDPC_matrix_handler::Positions_vector* ibp = nullptr;
		std::vector<bool>* pct = nullptr;

		if (info_bits_pos->empty())
			ibp = info_bits_pos.get();

		if (pct_params != nullptr && pct_params->pattern.empty())
			pct = &pct_params->pattern;

		*H = LDPC_matrix_handler::read(dec_params.H_path, ibp, pct);
	}

	if (dec_params.H_reorder != "NONE")
	{	// reorder the H matrix following the check node degrees
		H->sort_cols_per_density(dec_params.H_reorder == "ASC" ? Matrix::Sort::ASCENDING :
		                                                         Matrix::Sort::DESCENDING);
	}

	if (info_bits_pos->empty())
	{
		if (enc_params.type == "LDPC_H")
			this->set_encoder(enc_params.build<B>(*G, *H));
	}
	else
	{
		LDPC_matrix_handler::check_info_pos(*info_bits_pos, enc_params.K, enc_params.N_cw);
	}

	// ---------------------------------------------------------------------------------------------------- allocations
	if (pct_params == nullptr)
	{
		factory::Puncturer pctno_params;
		pctno_params.type = "NO";
		pctno_params.K    = enc_params.K;
		pctno_params.N    = enc_params.N_cw;
		pctno_params.N_cw = enc_params.N_cw;

		this->set_puncturer(pctno_params.build<B,Q>());
	}
	else
	{
		try
		{
			this->set_puncturer(pct_params->build<B,Q>());
		}
		catch (cannot_allocate const&)
		{
			this->set_puncturer(static_cast<const factory::Puncturer*>(pct_params)->build<B,Q>());
		}
	}

	try
	{
		this->get_encoder();
	}
	catch (runtime_error const&)
	{ // encoder not set when building encoder LDPC_H
		try
		{
			this->set_encoder(enc_params.build<B>(*G, *H, *dvbs2));
		}
		catch(cannot_allocate const&)
		{
			this->set_encoder(static_cast<const factory::Encoder*>(&enc_params)->build<B>());
		}
	}

	if (info_bits_pos->empty())
	{
		try
		{
			*info_bits_pos = this->get_encoder().get_info_bits_pos();
		}
		catch(unimplemented_error const&)
		{
			// generate a default vector [0, 1, 2, 3, ..., K-1]
			info_bits_pos->resize(enc_params.K);
			std::iota(info_bits_pos->begin(), info_bits_pos->end(), 0);
		}
	}

	this->set_extractor(new module::Extractor_LDPC<B,Q>(enc_params.K,
	                                                    enc_params.N_cw,
	                                                    *info_bits_pos));

	try
	{
		this->set_decoder_siso(dec_params.build_siso<B,Q>(*H, *info_bits_pos, &this->get_encoder()));
	}
	catch (const std::exception&)
	{
		this->set_decoder_siho(dec_params.build<B,Q>(*H, *info_bits_pos, &this->get_encoder()));
	}
}

template <typename B, typename Q>
Codec_LDPC<B,Q>* Codec_LDPC<B,Q>
::clone() const
{
	auto t = new Codec_LDPC(*this);
	t->deep_copy(*this);
	return t;
}

template <typename B, typename Q>
const Sparse_matrix& Codec_LDPC<B,Q>
::get_H() const
{
	if (this->H->size() == 0)
	{
		std::stringstream message;
		message << "'H->size()' has to be strictly greater than 0 ('H->size()' = " << H->size() << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->H;
}

template <typename B, typename Q>
const Sparse_matrix& Codec_LDPC<B,Q>
::get_G() const
{
	if (this->G->size() == 0)
	{
		std::stringstream message;
		message << "'G->size()' has to be strictly greater than 0 ('G->size()' = " << G->size() << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->G;
}

template <typename B, typename Q>
const LDPC_matrix_handler::Positions_vector& Codec_LDPC<B,Q>
::get_info_bits_pos() const
{
	return *info_bits_pos;
}

template <typename B, typename Q>
const dvbs2_values& Codec_LDPC<B,Q>
::get_DVBS2() const
{
	if (this->dvbs2 == nullptr)
	{
		std::stringstream message;
		message << "'dvbs2' can't be nullptr.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->dvbs2.get();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Codec_LDPC<B_8,Q_8>;
template class aff3ct::tools::Codec_LDPC<B_16,Q_16>;
template class aff3ct::tools::Codec_LDPC<B_32,Q_32>;
template class aff3ct::tools::Codec_LDPC<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_LDPC<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
