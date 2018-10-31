#include <vector>
#include <cmath>
#include <map>
#include <sstream>
#include <fstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Tools/general_utils.h"

#include "Encoder_polar_MK.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

// // DEBUG
// template <typename T = int32_t>
// void display_matrix(const std::vector<std::vector<T>>& M)
// {
// 	for (auto row = 0; row < (int)M.size(); row++)
// 	{
// 		for (auto col = 0; col < (int)M[0].size(); col++)
// 		{
// 			std::cout << M[row][col] << "\t";
// 		}
// 		std::cout << std::endl;
// 	}
// }

std::string aff3ct::tools::display_kernel(const std::vector<std::vector<bool>>& pattern_bits)
{
	std::string m = "{";

	for(auto &v : pattern_bits)
	{
		for(const auto &vb : v)
			m += std::to_string(vb);

		m += ",";
	}

	if (m.size())
		m.erase(m.size() -1);

	m += "}";

	return m;
}

void aff3ct::tools::read_polar_MK_code(const std::string                                 &code_path,
                                             std::vector<std::vector<std::vector<bool>>> &kernel_matrices,
                                             std::vector<uint32_t>                       &stages)
{
	std::ifstream file(code_path.c_str(), std::ios::in);
	std::string line;

	if (file.is_open())
	{
		tools::getline(file, line);
		auto values = tools::split(line);

		if (values.size() > 0)
		{
			auto n_kernels = std::stoi(values[0]);
			if (n_kernels > 0)
			{
				kernel_matrices.resize(n_kernels);

				for (auto k = 0; k < n_kernels; k++)
				{
					tools::getline(file, line);
					auto values = tools::split(line);

					if (values.size() > 0)
					{
						auto kernel_size = std::stoi(values[0]);
						std::vector<std::vector<bool>> kernel(kernel_size, std::vector<bool>(kernel_size));

						if (kernel_size > 1)
						{
							for (auto i = 0; i < kernel_size; i++)
							{
								tools::getline(file, line);
								auto values = tools::split(line);

								if (values.size() == kernel_size)
								{
									for (auto j = 0; j < kernel_size; j++)
										kernel[i][j] = (bool)std::stoi(values[j]);
								}
								else
								{
									file.close();

									std::stringstream message;
									message << "'values.size()' should be equal to 'kernel_size' ('values.size()' = "
									        << values.size() << ", 'kernel_size' = " << kernel_size << ").";
									throw runtime_error(__FILE__, __LINE__, __func__, message.str());
								}
							}
						}
						else
						{
							file.close();

							std::stringstream message;
							message << "'kernel_size' should be greater than 1 ('kernel_size' = "
							        << kernel_size << ").";
							throw runtime_error(__FILE__, __LINE__, __func__, message.str());
						}

						kernel_matrices[k] = kernel;
					}
					else
					{
						file.close();

						std::stringstream message;
						message << "'values.size()' should be greater than 0 ('values.size()' = "
						        << values.size() << ").";
						throw runtime_error(__FILE__, __LINE__, __func__, message.str());
					}
				}
			}
			else
			{
				file.close();

				std::stringstream message;
				message << "'n_kernels' should be greater than 0 ('n_kernels' = " << n_kernels << ").";
				throw runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			tools::getline(file, line);
			auto values = tools::split(line);

			if (values.size() > 0)
			{
				auto n_stages = std::stoi(values[0]);
				if (n_stages > 0)
				{
					stages.resize(n_stages);

					tools::getline(file, line);
					auto values = tools::split(line);

					if (values.size() == n_stages)
					{
						for (auto s = 0; s < n_stages; s++)
							stages[s] = (uint32_t)std::stoi(values[s]);
					}
					else
					{
						file.close();

						std::stringstream message;
						message << "'values.size()' should be equal to 'n_stages' ('values.size()' = "
						        << values.size() << ", 'n_stages' = " << n_stages << ").";
						throw runtime_error(__FILE__, __LINE__, __func__, message.str());
					}
				}
				else
				{
					file.close();

					std::stringstream message;
					message << "'n_stages' should be greater than 0 ('n_stages' = " << n_stages << ").";
					throw runtime_error(__FILE__, __LINE__, __func__, message.str());
				}
			}
			else
			{
				file.close();

				std::stringstream message;
				message << "'values.size()' should be greater than 0 ('values.size()' = "
				        << values.size() << ").";
				throw runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
		}
		else
		{
			file.close();

			std::stringstream message;
			message << "'values.size()' should be greater than 0 ('values.size()' = " << values.size() << ").";
			throw runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
	}
	else
	{
		std::stringstream message;
		message << "Can't open '" + code_path + "' file.";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

// Function to get cofactor of mat[p][q] in tmp[][]. n is current
// dimension of mat[][]
template <typename T = int32_t>
void get_cofactor(const std::vector<std::vector<T>> &mat, std::vector<std::vector<T>> &tmp,
                  const int p, const int q, const int n)
{
	int i = 0, j = 0;

	// Looping for each element of the matrix
	for (int row = 0; row < n; row++)
	{
		for (int col = 0; col < n; col++)
		{
			// Copying into temporary matrix only those element
			// which are not in given row and column
			if (row != p && col != q)
			{
				tmp[i][j++] = mat[row][col];

				// Row is filled, so increase row index and
				// reset col index
				if (j == n - 1)
				{
					j = 0;
					i++;
				}
			}
		}
	}
}

/* Recursive function for finding determinant of matrix.
n is current dimension of mat[][]. */
template <typename T = int32_t>
int determinant_of_matrix(const std::vector<std::vector<T>> &mat, const int n)
{
	int D = 0; // Initialize result

	// Base case : if matrix contains single element
	if (n == 1)
		return mat[0][0];

	std::vector<std::vector<T>> tmp(mat.size(), std::vector<T>(mat.size())); // To store cofactors

	int sign = 1; // To store sign multiplier

	// Iterate for each element of first row
	for (int f = 0; f < n; f++)
	{
		// Getting Cofactor of mat[0][f]
		get_cofactor(mat, tmp, 0, f, n);
		D += sign * mat[0][f] * determinant_of_matrix(tmp, n - 1);

		// terms are to be added with alternate sign
		sign = -sign;
	}

	return D;
}

template <typename T = int32_t>
bool is_invertible(const std::vector<std::vector<T>> &mat)
{
	return determinant_of_matrix(mat, (int)mat.size()) != 0;
}

template <typename B>
Encoder_polar_MK<B>
::Encoder_polar_MK(const int& K, const int& N, const std::vector<bool>& frozen_bits,
                   const std::vector<std::vector<bool>>& kernel_matrix, const int n_frames)
: Encoder<B>(K, N, n_frames),
  frozen_bits(frozen_bits),
  kernel_matrices(1, kernel_matrix),
  stages((int)(std::log(N)/std::log(kernel_matrix.size())), 0),
  // X_N_tmp(this->N),
  Ke(1, std::vector<B>(kernel_matrix.size() * kernel_matrix.size())),
  idx(kernel_matrix.size()),
  u(kernel_matrix.size())
{
	const std::string name = "Encoder_polar_MK";
	this->set_name(name);
	this->set_sys(false);

	this->init();
}

template <typename B>
Encoder_polar_MK<B>
::Encoder_polar_MK(const int& K, const int& N, const std::vector<bool>& frozen_bits,
                   const std::vector<std::vector<std::vector<bool>>>& kernel_matrices,
                   const std::vector<uint32_t> &stages, const int n_frames)
: Encoder<B>(K, N, n_frames),
  frozen_bits(frozen_bits),
  kernel_matrices(kernel_matrices),
  stages(stages),
  // X_N_tmp(this->N),
  Ke(kernel_matrices.size()),
  idx(),
  u()
{
	const std::string name = "Encoder_polar_MK";
	this->set_name(name);
	this->set_sys(false);

	this->init_MK();
}

template <typename B>
Encoder_polar_MK<B>
::Encoder_polar_MK(const int& K, const int& N, const std::vector<bool>& frozen_bits, const std::string &code_path,
                   const int n_frames)
: Encoder<B>(K, N, n_frames),
  frozen_bits(frozen_bits),
  kernel_matrices(),
  stages(),
  // X_N_tmp(this->N),
  Ke(),
  idx(),
  u()
{
	const std::string name = "Encoder_polar_MK";
	this->set_name(name);
	this->set_sys(false);

	// dark cpp but I know what I'm doing... I suppose...
	auto kernel_matrices_bis = const_cast<std::vector<std::vector<std::vector<bool>>>*>(&this->kernel_matrices);
	auto stages_bis = const_cast<std::vector<uint32_t>*>(&this->stages);
	tools::read_polar_MK_code(code_path, *kernel_matrices_bis, *stages_bis);

	this->Ke.resize(this->kernel_matrices.size());
	this->init_MK();
}

template <typename B>
void Encoder_polar_MK<B>
::init_MK()
{
	size_t biggest_kernel_size = 0;
	for (auto ke = 0; ke < (int)this->kernel_matrices.size(); ke++)
	{
		Ke[ke].resize(this->kernel_matrices[ke].size() * this->kernel_matrices[ke].size());
		biggest_kernel_size = std::max(biggest_kernel_size, this->kernel_matrices[ke].size());
	}

	idx.resize(biggest_kernel_size);
	u.resize(biggest_kernel_size);

	if (this->stages.size() == 0)
	{
		std::stringstream message;
		message << "'stages.size()' has to be higher than 0 ("
		        << "'stages.size()' = " << this->stages.size() << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->stages.size() < this->kernel_matrices.size())
	{
		std::stringstream message;
		message << "'stages.size()' has to be higher or equal to 'kernel_matrices.size()' ("
		        << "'stages.size()' = " << this->stages.size() << ", "
		        << "'kernel_matrices.size()' = " << this->kernel_matrices.size() << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto s : this->stages)
		if(s >= this->kernel_matrices.size())
		{
			std::stringstream message;
			message << "'s' should not be higher than 'kernel_matrices.size()' ("
			        << "'s' = " << s << ", "
			        << "'kernel_matrices.size()' = " << this->kernel_matrices.size() << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

	this->init();
}

template <typename B>
void Encoder_polar_MK<B>
::init()
{
	if (this->N != (int)this->frozen_bits.size())
	{
		std::stringstream message;
		message << "'frozen_bits.size()' has to be equal to 'N' ("
		        << "'frozen_bits.size()' = " << this->frozen_bits.size() << ", "
		        << "'N' = " << this->N << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	auto k = 0; for (auto i = 0; i < this->N; i++) if (this->frozen_bits[i] == 0) k++;
	if (this->K != k)
	{
		std::stringstream message;
		message << "The number of information bits in the frozen_bits is invalid ("
		        << "'K' = " << this->K << ", "
		        << "'k' = " << k << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto ke = 0; ke < (int)this->kernel_matrices.size(); ke++)
		for (auto l = 0; l < (int)this->kernel_matrices[ke].size(); l++)
			if (this->kernel_matrices[ke][l].size() != this->kernel_matrices[ke].size())
			{
				std::stringstream message;
				message << "Square matrices are required for the polar kernels: 'kernel_matrices[ke][l].size()' has to "
				        << "be equal to 'kernel_matrices[ke].size()' ("
				        << "'ke' = " << ke << ", "
				        << "'l' = " << l << ", "
				        << "'kernel_matrices[ke][l].size()' = " << this->kernel_matrices[ke][l].size() << ", "
				        << "'kernel_matrices[ke].size()' = " << this->kernel_matrices[ke].size() << ").";
				throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
			}

	for (auto ke = 0; ke < (int)this->kernel_matrices.size(); ke++)
		if (!is_invertible(this->kernel_matrices[ke]))
		{
			std::stringstream message;
			message << "'kernel_matrices[ke]' has to be invertible ("
			        << "'ke' = " << ke << ", "
			        << "'kernel_matrices[ke]' = " << tools::display_kernel(this->kernel_matrices[ke]) << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

	std::map<int,int> n_kernels_per_type;
	for (auto s : stages)
	{
		if (n_kernels_per_type.find(s) != n_kernels_per_type.end())
			n_kernels_per_type[s]++;
		else
			n_kernels_per_type[s] = 1;
	}
	auto expected_N = (int)1;
	for (auto kt : n_kernels_per_type)
		expected_N *= (int)std::pow((float)kernel_matrices[kt.first].size(), (int)kt.second);
	if (expected_N != this->N)
	{
		std::stringstream message;
		message << "'expected_N' should be equal to 'N' ("
		        << "'expected_N' = " << expected_N << ", "
		        << "'N' = " << this->N << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto ke = 0; ke < (int)this->kernel_matrices.size(); ke++)
	{
		const auto kernel_size = (int)this->kernel_matrices[ke].size();
		for (auto i = 0; i < kernel_size; i++)
			for (auto j = 0; j < kernel_size; j++)
				this->Ke[ke][i * kernel_size +j] = (B)this->kernel_matrices[ke][j][i];
	}

	this->notify_frozenbits_update();
}

template <typename B>
void Encoder_polar_MK<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	this->convert(U_K, X_N);
	this->light_encode(X_N);
}

template <typename B>
void polar_kernel(const B *u, const uint32_t *idx, const B *Ke, B *x, const int size)
{
	for (auto i = 0; i < size; i++)
	{
		const auto stride = i * size;
		auto sum = 0;
		for (auto j = 0; j < size; j++)
			sum += u[j] & Ke[stride +j];
		x[idx[i]] = sum & (B)1;
	}
}

template <typename B>
void Encoder_polar_MK<B>
::light_encode(B *X_N)
{
	auto n_kernels = (int)1;
	for (auto s = 0; s < (int)this->stages.size(); s++)
	{
		const auto kernel_size = (int)this->kernel_matrices[stages[s]].size();
		const auto n_blocks = this->N / (n_kernels * kernel_size);

		for (auto b = 0; b < n_blocks; b++)
		{
			for (auto k = 0; k < n_kernels; k++)
			{
				for (auto i = 0; i < kernel_size; i++)
				{
					this->idx[i] = (uint32_t)(b * n_kernels * kernel_size + n_kernels * i +k);
					this->u[i] = X_N[this->idx[i]];
				}

				polar_kernel(this->u.data(), this->idx.data(), this->Ke[stages[s]].data(), X_N, kernel_size);
			}
		}

		n_kernels *= kernel_size;
	}
}

template <typename B>
void Encoder_polar_MK<B>
::convert(const B *U_K, B *U_N)
{
	if (U_K == U_N)
	{
		std::vector<B> U_K_tmp(this->K);
		std::copy(U_K, U_K + this->K, U_K_tmp.begin());

		auto j = 0;
		for (unsigned i = 0; i < frozen_bits.size(); i++)
			U_N[i] = (frozen_bits[i]) ? (B)0 : U_K_tmp[j++];
	}
	else
	{
		auto j = 0;
		for (unsigned i = 0; i < frozen_bits.size(); i++)
			U_N[i] = (frozen_bits[i]) ? (B)0 : U_K[j++];
	}
}


// template <typename B>
// bool Encoder_polar_MK<B>
// ::is_codeword(const B *X_N)
// {
// 	std::copy(X_N, X_N + this->N, this->X_N_tmp.data());

// 	for (auto k = (this->N >> 1); k > 0; k >>= 1)
// 		for (auto j = 0; j < this->N; j += 2 * k)
// 		{
// 			for (auto i = 0; i < k; i++)
// 				this->X_N_tmp[j + i] = this->X_N_tmp[j + i] ^ this->X_N_tmp[k + j + i];

// 			if (this->frozen_bits[j + k -1] && this->X_N_tmp[j + k -1])
// 				return false;
// 		}

// 	return true;
// }

template <typename B>
void Encoder_polar_MK<B>
::notify_frozenbits_update()
{
	auto k = 0;
	for (auto n = 0; n < this->N; n++)
		if (!frozen_bits[n])
			this->info_bits_pos[k++] = n;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_polar_MK<B_8>;
template class aff3ct::module::Encoder_polar_MK<B_16>;
template class aff3ct::module::Encoder_polar_MK<B_32>;
template class aff3ct::module::Encoder_polar_MK<B_64>;
#else
template class aff3ct::module::Encoder_polar_MK<B>;
#endif
// ==================================================================================== explicit template instantiation
