#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <sstream>
#include <fstream>
#include <cmath>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/SCMA/Codebook.hpp"

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

	if (number_of_resources <= 0)
	{
		std::stringstream message;
		message << "'number_of_resources' has to be strictly positive ('number_of_resources' = " << number_of_resources << ").";
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

	if (file.bad())
	{
		std::stringstream message;
		message << "Can't open '" + codebook_path + "' codebook file.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	file >> number_of_users >> number_of_resources >> codebook_size;

	data.resize(number_of_users);

	for (int u = 0; u < number_of_users; ++u)
	{
		data[u].resize(number_of_resources);
		for (int r = 0; r < number_of_resources; ++r)
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

	// Factor graph calculation
	std::vector<std::vector<bool>> F(number_of_resources);

	for (int r = 0; r < number_of_resources; ++r)
	{
		F[r].resize(number_of_users, false);
		for (int u = 0; u < number_of_users; ++u)
		{
			for (int c = 0; c < codebook_size; ++c)
			{
				if ((data[u][r][c].real() != (R)0.0) || (data[u][r][c].imag() != (R)0.0))
				{
					F[r][u] = true;
					break;
				}
			}
		}
	}

	for (int r = 0; r < number_of_resources; ++r)
	{
		int n = 0; // number of users on this resource
		for (int u = 0; u < number_of_users; ++u)
			if (F[r][u])
				n++;

		if (r == 0)
			number_of_users_per_resource = n;

		else if (number_of_users_per_resource != n)
		{
			std::stringstream message;
			message << "All resources do not have the same number of users (the first one have " << number_of_users_per_resource << " of them).";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	for (int u = 0; u < number_of_users; ++u)
	{
		int n = 0; // number of resources used by this user
		for (int r = 0; r < number_of_resources; ++r)
			if (F[r][u])
				n++;

		if (u == 0)
			number_of_resources_per_user = n;

		else if (number_of_resources_per_user != n)
		{
			std::stringstream message;
			message << "All users do not use the same number of resources (the first one use " << number_of_resources_per_user << " of them).";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	resource_to_user.resize(number_of_resources);
	for (int r = 0; r < number_of_resources; ++r)
	{
		resource_to_user[r].resize(number_of_users_per_resource);
		int idx = 0;
		for (int u = 0; u < number_of_users; ++u)
		{
			if (F[r][u])
			{
				resource_to_user[r][idx] = u;
				idx++;
			}
		}
	}

	user_to_resource.resize(number_of_users);
	for (int u = 0; u < number_of_users; ++u)
	{
		user_to_resource[u].resize(number_of_resources_per_user);
		int idx = 0;
		for (int r = 0; r < number_of_resources; ++r)
		{
			if (F[r][u])
			{
				user_to_resource[u][idx] = r;
				idx++;
			}
		}
	}

	system_bps = std::log2((float)codebook_size) * (float)number_of_users / (float)number_of_resources;
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
::get_number_of_resources_per_user() const
{
	return number_of_resources_per_user;
}
template <typename R>
inline int Codebook<R>
::get_number_of_users_per_resource() const
{
	return number_of_users_per_resource;
}

template <typename R>
inline int Codebook<R>
::get_number_of_resources() const
{
	return number_of_resources;
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

template <typename R>
inline int Codebook<R>
::get_number_of_real_symbols() const
{
	return (2 * number_of_resources);
}

template <typename R>
inline float Codebook<R>
::get_system_bps() const
{
	return system_bps;
}

}
}
