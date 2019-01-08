#include <vector>
#include <cmath>
#include <map>
#include <sstream>
#include <fstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Tools/general_utils.h"

#include "Encoder_polar_MK_sys.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename T = int32_t>
void kronecker_product(const std::vector<std::vector<T>>& A,
                       const std::vector<std::vector<T>>& B,
                             std::vector<std::vector<T>>& C)
{
	for (auto row_A = 0; row_A < (int)A.size(); row_A++)
		for (auto col_A = 0; col_A < (int)A[0].size(); col_A++)
			for (auto row_B = 0; row_B < (int)B.size(); row_B++)
				for (auto col_B = 0; col_B < (int)B[0].size(); col_B++)
					C[row_A * B.size() + row_B][col_A * B[0].size() + col_B] = A[row_A][col_A] * B[row_B][col_B];
}

template <typename T = int32_t>
std::vector<std::vector<T>> kronecker_product(const std::vector<std::vector<T>>& A,
                                              const std::vector<std::vector<T>>& B)
{
	// verifications --------------------------------------------------------------------------------------------------
	if (A.size() == 0)
	{
		std::stringstream message;
		message << "'A.size()' should be higher than 0 ('A.size()' = " << A.size() << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (B.size() == 0)
	{
		std::stringstream message;
		message << "'B.size()' should be higher than 0 ('B.size()' = " << B.size() << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto l = 0; l < (int)A.size(); l++)
	{
		if (A[l].size() != A.size())
		{
			std::stringstream message;
			message << "'A[l].size()' has to be equal to 'A.size()' ('l' = " << l
			        << ", 'A[l].size()' = " << A[l].size()
			        << ", 'A.size()' = " << A.size() << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}
	}

	for (auto l = 0; l < (int)B.size(); l++)
	{
		if (B[l].size() != B.size())
		{
			std::stringstream message;
			message << "'B[l].size()' has to be equal to 'B.size()' ('l' = " << l
			        << ", 'B[l].size()' = " << B[l].size()
			        << ", 'B.size()' = " << B.size() << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}
	}
	// ----------------------------------------------------------------------------------------------------------------

	std::vector<std::vector<T>> C(A.size() * B.size(), std::vector<T>(A[0].size() * B[0].size()));
	kronecker_product(A, B, C);
	return C;
}

template <typename B>
Encoder_polar_MK_sys<B>
::Encoder_polar_MK_sys(const int& K, const int& N, const std::vector<bool>& frozen_bits,
                       const std::vector<std::vector<bool>>& kernel_matrix, const int n_frames)
: Encoder_polar_MK<B>(K, N, frozen_bits, kernel_matrix, n_frames)
{
	const std::string name = "Encoder_polar_MK_sys";
	this->set_name(name);
	this->set_sys(true);

	this->init();
}

template <typename B>
Encoder_polar_MK_sys<B>
::Encoder_polar_MK_sys(const int& K, const int& N, const std::vector<bool>& frozen_bits,
                       const std::vector<std::vector<std::vector<bool>>>& kernel_matrices,
                       const std::vector<uint32_t> &stages, const int n_frames)
: Encoder_polar_MK<B>(K, N, frozen_bits, kernel_matrices, stages, n_frames)
{
	const std::string name = "Encoder_polar_MK_sys";
	this->set_name(name);
	this->set_sys(true);

	this->init();
}

template <typename B>
Encoder_polar_MK_sys<B>
::Encoder_polar_MK_sys(const int& K, const int& N, const std::vector<bool>& frozen_bits, const std::string &code_path,
                       const int n_frames)
: Encoder_polar_MK<B>(K, N, frozen_bits, code_path, n_frames)
{
	const std::string name = "Encoder_polar_MK_sys";
	this->set_name(name);
	this->set_sys(true);

	this->init();
}

template <typename B>
void Encoder_polar_MK_sys<B>
::init()
{
	// generate the "G" matrix from the "kernel_matrices"
	auto G = this->kernel_matrices[this->stages[0]];
	for (auto s = 1; s < (int)this->stages.size(); s++)
		G = kronecker_product<bool>(G, this->kernel_matrices[this->stages[s]]);

	// compute the "G x G" product
	auto G_x_G = G;
	for (auto i = 0; i < (int)G.size(); i++)
		for (auto j = 0; j < (int)G.size(); j++)
		{
			uint32_t sum_r = 0;
			for (auto k = 0; k < (int)G.size(); k++)
				sum_r += (uint32_t)(G[i][k] & G[k][j]);
			G_x_G[i][j] = (bool)(sum_r & (B)1);
		}

	// check if "G x G" is the identity matrix
	for (auto i = 0; i < (int)G_x_G.size(); i++)
		for (auto j = 0; j < (int)G_x_G.size(); j++)
			if (i == j && G_x_G[i][j] != true)
			{
				std::stringstream message;
				message << "'G_x_G' has to be the identity matrix ("
				        << "'i' = " << i << ", "
				        << "'j' = " << j << ", "
				        << "'G_x_G[i][j]' = " << G_x_G[i][j] << ").";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
			else if (i != j && G_x_G[i][j] != false)
			{
				std::stringstream message;
				message << "'G_x_G' has to be the identity matrix ("
				        << "'i' = " << i << ", "
				        << "'j' = " << j << ", "
				        << "'G_x_G[i][j]' = " << G_x_G[i][j] << ").";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
}

template <typename B>
void Encoder_polar_MK_sys<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	this->convert(U_K, X_N);

	// first time encode
	this->light_encode(X_N);

	for (auto i = 0; i < this->N; i++)
		X_N[i] = (B)(!this->frozen_bits[i]) && X_N[i];

	// second time encode because of systematic encoder
	this->light_encode(X_N);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_polar_MK_sys<B_8>;
template class aff3ct::module::Encoder_polar_MK_sys<B_16>;
template class aff3ct::module::Encoder_polar_MK_sys<B_32>;
template class aff3ct::module::Encoder_polar_MK_sys<B_64>;
#else
template class aff3ct::module::Encoder_polar_MK_sys<B>;
#endif
// ==================================================================================== explicit template instantiation
