#ifndef CODEBOOK_HXX_
#define CODEBOOK_HXX_

#include <sstream>
#include <fstream>

#include "Tools/Exception/exception.hpp"

#include "Codebook.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R>
Codebook<R>
::Codebook(const std::string& codebook_path)
{
	read_codebook(codebook_path);

	if (number_of_users <= 0)
	{
		std::stringstream message;
		message << "'number_of_users' has to be strictly positive ('number_of_users' = " << number_of_users << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (number_of_orthogonal_resources <= 0)
	{
		std::stringstream message;
		message << "'number_of_orthogonal_resources' has to be strictly positive ('number_of_orthogonal_resources' = " << number_of_orthogonal_resources << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (codebook_size <= 0)
	{
		std::stringstream message;
		message << "'number_of_users' has to be strictly positive ('codebook_size' = " << codebook_size << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename R>
void Codebook<R>
::read_codebook(const std::string& codebook_path)
{
	std::ifstream file(codebook_path);

	file >> number_of_users >> number_of_orthogonal_resources >> codebook_size;

	data.resize(number_of_users);

	for (int u = 0; u < number_of_users; ++u)
	{
		data[u].resize(number_of_orthogonal_resources);
		for (int r = 0; r < number_of_orthogonal_resources; ++r)
		{
			data[u][r].resize(codebook_size);
			for (int c = 0; c < codebook_size; ++c)
			{
				if (file.fail())
				{
					std::stringstream message;
					message << "There is not enough data in codebook file 'codebook_path' ( = \"" << codebook_path << "\").";
					throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
				}
				float real, imag;
				file >> real >> imag;

				data[u][r][c].real(real);
				data[u][r][c].imag(imag);
			}
		}
	}

	// Factor graph calculation TODO:  Now constants and fixed size (-> dynamic allocation later)
	int F[4][6] = {};

    for (int u = 0; u < number_of_users; ++u)
	{
		for (int r = 0; r < number_of_orthogonal_resources; ++r)
		{
			for (int c = 0; c < codebook_size; ++c)
			{
				if ((data[u][r][c].real() != 0.0) || (data[u][r][c].imag() != 0.0))
                {
					F[r][u] = 1;
					break;
                }
			}
		}
	}

	for (int r = 0; r < number_of_orthogonal_resources; ++r)
	{
		int idx = 0;
		for (int u = 0; u < number_of_users; ++u)
		{
			if (F[r][u] == 1)
			{
				resource_to_user[r][idx] = u;
				idx++;
			}
		}
	}

	for (int u = 0; u < number_of_users; ++u)
	{
		int idx = 0;
		for (int r = 0; r < number_of_orthogonal_resources; ++r)
		{
			if (F[r][u] == 1)
			{
				user_to_resource[u][idx] = r;
				idx++;
			}
		}
	}
}



template <typename R>
inline const std::complex<R>& Codebook<R>
::operator() (int u, int o, int c) const
{
	return data[u][o][c];
}

template <typename R>
inline int Codebook<R>
::get_number_of_users() const
{
	return number_of_users;
}

template <typename R>
inline int Codebook<R>
::get_number_of_orthogonal_resources() const
{
	return number_of_orthogonal_resources;
}

template <typename R>
inline int Codebook<R>
::get_codebook_size() const
{
	return codebook_size;
}

template <typename R>
inline int Codebook<R>
::get_resource_to_user(int r, int u) const
{
	return resource_to_user[r][u];
}

template <typename R>
inline int Codebook<R>
::get_user_to_resource(int u, int r) const
{
	return user_to_resource[u][r];
}

}
}

#endif // CODEBOOK_HXX_
