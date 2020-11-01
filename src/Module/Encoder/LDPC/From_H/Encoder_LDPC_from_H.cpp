#include <sstream>
#include <fstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Matrix/Matrix.hpp"
#include "Tools/Code/LDPC/AList/AList.hpp"
#include "Tools/Code/LDPC/Matrix_handler/LDPC_matrix_handler.hpp"
#include "Module/Encoder/LDPC/From_H/Encoder_LDPC_from_H.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
std::thread::id aff3ct::module::Encoder_LDPC_from_H<B>::master_thread_id = std::this_thread::get_id();

template <typename B>
Encoder_LDPC_from_H<B>
::Encoder_LDPC_from_H(const int K, const int N, const tools::Sparse_matrix &_H, const std::string& G_method,
                      const std::string& G_save_path, const bool G_save_path_single_thread)
: Encoder_LDPC<B>(K, N)
{
	const std::string name = "Encoder_LDPC_from_H";
	this->set_name(name);

	this->H = _H.turn(tools::Matrix::Way::HORIZONTAL);

	if (G_method == "IDENTITY")
		this->G = tools::LDPC_matrix_handler::transform_H_to_G_identity(this->H, this->info_bits_pos);
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
		if (!G_save_path_single_thread || this->master_thread_id == std::this_thread::get_id())
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
	}

	this->check_G_dimensions();
	this->check_H_dimensions();
}

template <typename B>
Encoder_LDPC_from_H<B>* Encoder_LDPC_from_H<B>
::clone() const
{
	auto m = new Encoder_LDPC_from_H(*this);
	m->deep_copy(*this);
	return m;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_LDPC_from_H<B_8>;
template class aff3ct::module::Encoder_LDPC_from_H<B_16>;
template class aff3ct::module::Encoder_LDPC_from_H<B_32>;
template class aff3ct::module::Encoder_LDPC_from_H<B_64>;
#else
template class aff3ct::module::Encoder_LDPC_from_H<B>;
#endif
// ==================================================================================== explicit template instantiation
