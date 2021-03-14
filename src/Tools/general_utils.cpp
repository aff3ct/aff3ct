#include <map>
#include <cmath>
#include <limits>
#include <sstream>
#include <cstdint>
#include <algorithm>

#include "Tools/Math/utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/general_utils.h"

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

template <typename T>
void aff3ct::tools::check_LUT(const std::vector<T> &LUT, const std::string &LUT_name, const size_t LUT_size)
{
	std::map<T, unsigned> count_map;

	if (LUT_size != 0 && LUT.size() != LUT_size)
	{
		std::stringstream message;
		message << "'" + LUT_name + ".size()' has to be equal to '" << LUT_size << "' ('" << LUT_name << ".size()' = "
		        << LUT.size() << ", 'LUT_size' = " << LUT_size << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	for (size_t i = 0; i < LUT.size(); i++)
	{
		count_map[LUT[i]]++;
		if (count_map[LUT[i]] != 1)
		{
			std::stringstream message;
			message << "The '" << LUT_name << "' vector is ill-formed, it sould not contain two or more times the same "
			        << "value ('count_map[" << LUT[i] << "]' = " << count_map[LUT[i]] << ", "
			        << "'LUT[" << i << "]' = " << LUT[i] << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}
}

size_t aff3ct::tools::compute_bytes(const size_t n_elmts, const std::type_index type)
{
	     if (type == typeid(int8_t )) return n_elmts * sizeof(int8_t );
	else if (type == typeid(int16_t)) return n_elmts * sizeof(int16_t);
	else if (type == typeid(int32_t)) return n_elmts * sizeof(int32_t);
	else if (type == typeid(int64_t)) return n_elmts * sizeof(int64_t);
	else if (type == typeid(float  )) return n_elmts * sizeof(float  );
	else if (type == typeid(double )) return n_elmts * sizeof(double );
	else
	{
		std::stringstream message;
		message << "This should never happen.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

std::vector<size_t> aff3ct::tools::compute_bytes(const std::vector<size_t> &n_elmts,
                                                 const std::vector<std::type_index> &type)
{
	if (n_elmts.size() != type.size())
	{
		std::stringstream message;
		message << "'n_elmts.size()' has to be equal to 'type.size()' ('n_elmts.size()' = " << n_elmts.size()
		        << ", 'type.size()' = " << type.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	std::vector<size_t> bytes(n_elmts.size());
	for (size_t i = 0; i < n_elmts.size(); i++)
		bytes[i] = aff3ct::tools::compute_bytes(n_elmts[i], type[i]);
	return bytes;
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
template void aff3ct::tools::check_LUT<uint8_t >(const std::vector<uint8_t >&, const std::string&, const size_t);
template void aff3ct::tools::check_LUT<uint16_t>(const std::vector<uint16_t>&, const std::string&, const size_t);
template void aff3ct::tools::check_LUT<uint32_t>(const std::vector<uint32_t>&, const std::string&, const size_t);
template void aff3ct::tools::check_LUT<uint64_t>(const std::vector<uint64_t>&, const std::string&, const size_t);
// ==================================================================================== explicit template instantiation
