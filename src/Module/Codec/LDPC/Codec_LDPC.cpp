#include <string>
#include <fstream>
#include <sstream>
#include <numeric>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/LDPC/AList/AList.hpp"
#include "Tools/Code/LDPC/QC_matrix/QC_matrix.hpp"
#include "Tools/general_utils.h"

#include "Factory/Module/Puncturer/Puncturer.hpp"

#include "Module/Decoder/Decoder_SISO_SIHO.hpp"
#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"
#include "Module/Puncturer/LDPC/Puncturer_LDPC.hpp"

#include "Codec_LDPC.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
std::string Codec_LDPC<B,Q>
::get_matrix_format(const std::string& filename)
{
	std::ifstream file(filename, std::ifstream::in);

	if (file.is_open())
	{
		std::string line;
		tools::getline(file, line);
		file.close();

		auto values = tools::split(line);

		if (values.size() == 3)
			return "QC";
		else
			return "ALIST";
	}
	else
	{
		return "BAD_FILE";
	}
}

template <typename B, typename Q>
Codec_LDPC<B,Q>
::Codec_LDPC(const factory::Encoder_LDPC::parameters &enc_params,
             const factory::Decoder_LDPC::parameters &dec_params,
                   factory::Puncturer_LDPC::parameters &pct_params,
             const std::string name)
: Codec          <B,Q>(enc_params.K, enc_params.N_cw, pct_params.N, enc_params.tail_length, enc_params.n_frames, name),
  Codec_SISO_SIHO<B,Q>(enc_params.K, enc_params.N_cw, pct_params.N, enc_params.tail_length, enc_params.n_frames, name),
  info_bits_pos(enc_params.K)
{
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

	if (dec_params.H_path.empty())
	{
		std::stringstream message;
		message << "'dec_params.H_path' has to be set to a matrix file.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// ---------------------------------------------------------------------------------------------------------- tools
	bool is_info_bits_pos = false;
	if (enc_params.type == "LDPC")
	{
		auto G_format = get_matrix_format(enc_params.G_path);

		if (G_format == "QC")
		{
			std::ifstream file_G(enc_params.G_path, std::ifstream::in);
			tools::QC_matrix QC = tools::QC_matrix::load(file_G);
			G = QC.expand_QC();
			file_G.close();
		}
		else if (G_format == "ALIST")
		{
			std::ifstream file_G(enc_params.G_path, std::ifstream::in);
			G = tools::AList::read(file_G);

			try
			{
				info_bits_pos = tools::AList::read_info_bits_pos(file_G, this->K, this->N_cw);
				is_info_bits_pos = true;
			}
			catch (std::exception const&)
			{
				// information bits positions are not in the G matrix file
			}

			file_G.close();
		}
		else if (G_format == "BAD_FILE")
		{
			std::stringstream message;
			message << "'enc_params.G_path' can't be opened ('enc_params.G_path' = \"" + enc_params.G_path + "\").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	auto H_format = get_matrix_format(dec_params.H_path);
	if (H_format == "QC")
	{
		std::ifstream file_H(dec_params.H_path, std::ifstream::in);
		tools::QC_matrix QC = tools::QC_matrix::load(file_H);
		H = QC.expand_QC();
		pct_params.pattern = QC.get_pct_pattern();

		std::iota(info_bits_pos.begin(), info_bits_pos.end(), 0);

		file_H.close();
	}
	else if (H_format == "ALIST")
	{
		std::ifstream file_H(dec_params.H_path, std::ifstream::in);
		H = tools::AList::read(file_H);

		if (!is_info_bits_pos)
		{
			try
			{
				if (enc_params.type == "LDPC_H")
				{
					auto encoder_LDPC = factory::Encoder_LDPC::build<B>(enc_params, G, H);
					encoder_LDPC->get_info_bits_pos(info_bits_pos);
					delete encoder_LDPC;
				}
				else
					info_bits_pos = tools::AList::read_info_bits_pos(file_H, enc_params.K, enc_params.N_cw);
			}
			catch (std::exception const&)
			{
				std::iota(info_bits_pos.begin(), info_bits_pos.end(), 0);
			}
		}

		file_H.close();
	}
	else if (H_format == "BAD_FILE")
	{
		std::stringstream message;
		message << "'dec_params.H_path' can't be opened ('dec_params.H_path' = \"" + dec_params.H_path + "\").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// ---------------------------------------------------------------------------------------------------- allocations
	try
	{
		this->set_puncturer(factory::Puncturer_LDPC::build<B,Q>(pct_params));
	}
	catch (tools::cannot_allocate const&)
	{
		this->set_puncturer(factory::Puncturer::build<B,Q>(pct_params));
	}

	try
	{
		this->set_encoder(factory::Encoder_LDPC::build<B>(enc_params, G, H));
	}
	catch (tools::cannot_allocate const&)
	{
		this->set_encoder(factory::Encoder::build<B>(enc_params));
	}

	try
	{
		auto decoder_siso_siho = factory::Decoder_LDPC::build_siso<B,Q>(dec_params, H, info_bits_pos);
		this->set_decoder_siso(decoder_siso_siho);
		this->set_decoder_siho(decoder_siso_siho);
	}
	catch (const std::exception&)
	{
		this->set_decoder_siho(factory::Decoder_LDPC::build<B,Q>(dec_params, H, info_bits_pos));
	}
}

template <typename B, typename Q>
Codec_LDPC<B,Q>
::~Codec_LDPC()
{
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
#ifdef MULTI_PREC
template class aff3ct::module::Codec_LDPC<B_8,Q_8>;
template class aff3ct::module::Codec_LDPC<B_16,Q_16>;
template class aff3ct::module::Codec_LDPC<B_32,Q_32>;
template class aff3ct::module::Codec_LDPC<B_64,Q_64>;
#else
template class aff3ct::module::Codec_LDPC<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
