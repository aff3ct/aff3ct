#include <cmath>
#include <limits>
#include <sstream>

#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"

#include "general_utils.h"

std::vector<std::string> aff3ct::tools::string_split(const std::string &s, char delim)
{
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim))
		elems.push_back(std::move(item));

	return elems;
}

template <typename R>
R aff3ct::tools::sigma_to_esn0(const R sigma, const int upsample_factor)
{
	if (upsample_factor <= 0)
	{
		std::stringstream message;
		message << "'upsample_factor' has to be greater than 0 ('upsample_factor' = " << upsample_factor << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (sigma < (R)0)
	{
		std::stringstream message;
		message << "'sigma' has to be greater than 0 ('sigma' = " << sigma << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (sigma == (R)0)
	{
		const auto esn0 = std::numeric_limits<R>::infinity();
		return esn0;
	}
	else
	{
		const auto esn0 = (R)10 * std::log10((R)upsample_factor / (sigma * sigma));
		return esn0;
	}
}

template <typename R>
R aff3ct::tools::esn0_to_sigma(const R esn0, const int upsample_factor)
{
	if (upsample_factor <= 0)
	{
		std::stringstream message;
		message << "'upsample_factor' has to be greater than 0 ('upsample_factor' = " << upsample_factor << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	const auto sigma = std::sqrt((R)upsample_factor / ((R)2 * std::pow((R)10, (esn0 / (R)10))));
	return sigma;
}

template <typename R>
R aff3ct::tools::esn0_to_ebn0(const R esn0, const R bit_rate, const int bps)
{
	if (bit_rate <= (R)0 || bit_rate > (R)1)
	{
		std::stringstream message;
		message << "'bit_rate' has to be positive and smaller or equal to 1 ('bit_rate' = " << bit_rate << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (bps <= 0)
	{
		std::stringstream message;
		message << "'bps' has to be greater than 0 ('bps' = " << bps << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	const auto ebn0 = esn0 - (R)10 * std::log10(bit_rate * (R)bps);
	return ebn0;
}

template <typename R>
R aff3ct::tools::ebn0_to_esn0(const R ebn0, const R bit_rate, const int bps)
{
	if (bit_rate <= (R)0 || bit_rate > (R)1)
	{
		std::stringstream message;
		message << "'bit_rate' has to be positive and smaller or equal to 1 ('bit_rate' = " << bit_rate << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (bps <= 0)
	{
		std::stringstream message;
		message << "'bps' has to be greater than 0 ('bps' = " << bps << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	const auto esn0 = ebn0 + (R)10 * std::log10(bit_rate * (R)bps);
	return esn0;
}

// ==================================================================================== explicit template instantiation
template float  aff3ct::tools::sigma_to_esn0<float >(const float,                const int);
template double aff3ct::tools::sigma_to_esn0<double>(const double,               const int);
template float  aff3ct::tools::esn0_to_sigma<float >(const float,                const int);
template double aff3ct::tools::esn0_to_sigma<double>(const double,               const int);
template float  aff3ct::tools::esn0_to_ebn0 <float >(const float,  const float,  const int);
template double aff3ct::tools::esn0_to_ebn0 <double>(const double, const double, const int);
template float  aff3ct::tools::ebn0_to_esn0 <float >(const float,  const float,  const int);
template double aff3ct::tools::ebn0_to_esn0 <double>(const double, const double, const int);
// ==================================================================================== explicit template instantiation
