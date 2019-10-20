#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <fstream>
#include <cmath>
#include <iterator>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Constellation/User/Constellation_user.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R>
Constellation_user<R>
::Constellation_user(const std::string& const_path)
: Constellation<R>(read_constellation(const_path), "User<C>")
{
}

template <typename R>
std::vector<typename Constellation_user<R>::S> Constellation_user<R>
::read_constellation(const std::string& const_path)
{
	if (const_path.empty())
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'const_path' should not be empty.");

	std::ifstream const_file(const_path);

	if (const_file.fail())
	{
		std::stringstream message;
		message << "Opening 'const_path' (= " << const_path << ") has failed.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	std::vector<S> constellation;
	auto sqrt_es = (R)0;
	std::string temp;
	while (std::getline(const_file, temp))
	{
		if (temp[0] == '#') continue;

		std::istringstream buffer(temp);
		std::vector<R> line((std::istream_iterator<R>(buffer)), std::istream_iterator<R>());

		if (line.size() >= 3)
		{
			std::stringstream message;
			message << "'line.size()' has to be smaller than 3 ('line.size()' = " << line.size() << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (line.size() == 2)
			constellation.push_back(S(line[0],line[1]));
		else
			constellation.push_back(S(line[0]));

		sqrt_es += std::norm(constellation.back());
	}
	sqrt_es = std::sqrt(sqrt_es/constellation.size());

	for (unsigned i = 0; i < constellation.size(); i++)
		constellation[i] /= S(sqrt_es);

	return constellation;
}

}
}
