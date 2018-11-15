#include <cmath>
#include <limits>
#include <sstream>
#include <algorithm>

#include "Tools/Math/utils.h"
#include "Tools/Exception/exception.hpp"

#include "general_utils.h"

std::vector<std::string> aff3ct::tools::split(const std::string &s, char delim)
{
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim))
		elems.push_back(std::move(item));

	return elems;
}

std::vector<std::string> aff3ct::tools::split(const std::string &s)
{
	std::string buf;                 // have a buffer string
	std::stringstream ss(s);         // insert the string into a stream
	std::vector<std::string> tokens; // create vector to hold our words

	while (ss >> buf)
		tokens.push_back(buf);

	return tokens;
}

void aff3ct::tools::getline(std::istream &file, std::string &line)
{
	if (file.eof() || file.fail() || file.bad())
		throw runtime_error(__FILE__, __LINE__, __func__, "Something went wrong when getting a new line.");

	while (std::getline(file, line))
		if (line[0] != '#' && !std::all_of(line.begin(),line.end(),isspace))
			break;
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

template <typename R>
std::vector<R> aff3ct::tools::generate_range(const std::vector<std::vector<R>>& range_description, const R default_step)
{
	const R float_precision = 1e7;

	std::vector<int> range;

	for (auto& s : range_description)
	{
		if (s.size() == 1)
		{
			range.push_back((int)(s.front() * float_precision));
			continue;
		}

		if (s.size() != 3 && s.size() != 2)
		{
			std::stringstream message;
			message << "'s.size()' has to be 1, 2 or 3 ('s.size()' = " << s.size() << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		int min  = (int)(s.front() * float_precision);
		int max  = (int)(s.back () * float_precision);
		int step = (int)(((s.size() == 3) ? s[1] : default_step) * float_precision);

		if (min > max && step < 0)
		{
			std::swap(min, max);
			step *= -1;
		}

		for (int v = min; v <= max; v += step)
			range.push_back(v);
	}

	std::sort(range.begin(), range.end());

	auto last = std::unique(range.begin(), range.end());
	auto new_length = std::distance(range.begin(), last);

	std::vector<R> rangeR(new_length);

	for (auto i = 0; i < new_length; i++)
		rangeR[i] = ((R)range[i])/float_precision;

	return rangeR;
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
template std::vector<float > aff3ct::tools::generate_range(const std::vector<std::vector<float >>&, const float );
template std::vector<double> aff3ct::tools::generate_range(const std::vector<std::vector<double>>&, const double);
// ==================================================================================== explicit template instantiation
