#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
#include <sstream>
#include <fstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/LDPC/AList/AList.hpp"

#include "Encoder_LDPC_from_H.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_LDPC_from_H<B>
::Encoder_LDPC_from_H(const int K, const int N, const tools::Sparse_matrix &_H, const std::string& G_method,
                      const std::string& G_save_path, const int n_frames)
: Encoder_LDPC<B>(K, N, n_frames)
{
	const std::string name = "Encoder_LDPC_from_H";
	this->set_name(name);

	this->H = _H.turn(tools::Matrix::Way::HORIZONTAL);

	if (G_method == "FAST")
		this->G = tools::LDPC_matrix_handler::transform_H_to_G_fast(this->H, this->info_bits_pos);
	else if (G_method == "LU_DEC")
		this->G = tools::LDPC_matrix_handler::transform_H_to_G_decomp_LU(this->H, this->info_bits_pos);
	else
	{
		std::stringstream message;
		message << "Generation method of G 'G_method' is unknown ('G_method' = \"" << G_method << "\").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (G_save_path != "")
	{
		std::ofstream file(G_save_path);
		if (!file.is_open())
		{
			std::stringstream message;
			message << "'G_save_path' could not be opened ('G_save_path' = \"" << G_save_path << "\").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		tools::AList::write(this->G, file);
		tools::AList::write_info_bits_pos(this->info_bits_pos, file);
	}

	this->check_G_dimensions();
	this->check_H_dimensions();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_LDPC_from_H<B_8>;
template class aff3ct::module::Encoder_LDPC_from_H<B_16>;
template class aff3ct::module::Encoder_LDPC_from_H<B_32>;
template class aff3ct::module::Encoder_LDPC_from_H<B_64>;
#else
template class aff3ct::module::Encoder_LDPC_from_H<B>;
#endif
// ==================================================================================== explicit template instantiation
