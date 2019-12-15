#include <fstream>
#include <sstream>
#include <cmath>
#include <map>

#include "Tools/Exception/exception.hpp"
#include "Tools/general_utils.h"

#include "Tools/Code/Polar/Polar_code.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

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

								if (values.size() == (size_t)kernel_size)
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

					if (values.size() == (size_t)n_stages)
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

Polar_code
::Polar_code(const std::string &code_path)
: N(),
  kernel_matrices(),
  stages()
{
	tools::read_polar_MK_code(code_path, this->kernel_matrices, this->stages);

	this->N = 1;
	for (auto s : this->stages)
		this->N *= (int)this->kernel_matrices[s].size();

	this->verif_MK();
}

Polar_code
::Polar_code(const int N, const std::vector<std::vector<bool>>& kernel_matrix)
: N(N),
  kernel_matrices(1, kernel_matrix),
  stages((int)(std::log(N)/std::log(kernel_matrix.size())), 0)
{
	this->verif();
}

Polar_code
::Polar_code(const std::vector<std::vector<std::vector<bool>>>& kernel_matrices,
             const std::vector<uint32_t> &stages)
: N(),
  kernel_matrices(kernel_matrices),
  stages(stages)
{
	this->N = 1;
	for (auto s : this->stages)
		this->N *= (int)this->kernel_matrices[s].size();

	this->verif_MK();
}

void Polar_code
::verif_MK()
{
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

	this->verif();
}

void Polar_code
::verif()
{
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
}

bool Polar_code
::can_be_systematic() const
{
	if (!this->is_mono_kernel())
	{
		const auto n_stages_2 = this->get_stages().size() / 2;
		for (size_t sl = 0; sl < n_stages_2; sl++)
			if (this->get_stages()[sl] != this->get_stages()[this->get_stages().size() -1 -sl])
				return false;
	}

	for (auto &kernel : this->get_kernel_matrices())
	{
		// compute the "K x K" product
		auto k_x_k = kernel;
		for (auto i = 0; i < (int)kernel.size(); i++)
			for (auto j = 0; j < (int)kernel.size(); j++)
			{
				uint32_t sum_r = 0;
				for (auto k = 0; k < (int)kernel.size(); k++)
					sum_r += (uint32_t)(kernel[i][k] & kernel[k][j]);
				k_x_k[i][j] = (bool)(sum_r & (uint32_t)1);
			}

		// check if "kernel x kernel" is the identity matrix
		for (auto i = 0; i < (int)k_x_k.size(); i++)
			for (auto j = 0; j < (int)k_x_k.size(); j++)
				if (i == j && k_x_k[i][j] != true)
					return false;
				else if (i != j && k_x_k[i][j] != false)
					return false;
	}

	return true;
}

size_t Polar_code
::is_mono_kernel() const
{
	auto type = stages[0];
	for (size_t s = 1; s < stages.size(); s++)
		if (stages[s] != type)
			return 0;
	return this->get_kernel_matrices()[0].size();
}

int Polar_code
::get_codeword_size() const
{
	return this->N;
}

const std::vector<std::vector<std::vector<bool>>>& Polar_code
::get_kernel_matrices() const
{
	return this->kernel_matrices;
}

const std::vector<uint32_t>& Polar_code
::get_stages() const
{
	return this->stages;
}

size_t Polar_code
::get_biggest_kernel_size() const
{
	size_t biggest_kernel_size = 0;
	for (auto ke = 0; ke < (int)this->get_kernel_matrices().size(); ke++)
		biggest_kernel_size = std::max(biggest_kernel_size, this->get_kernel_matrices()[ke].size());

	return biggest_kernel_size;
}