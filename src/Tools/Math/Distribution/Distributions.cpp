#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Arguments/Splitter/Splitter.hpp"
#include "Distributions.hpp"

using namespace aff3ct;
using namespace tools;

template<typename R>
Distributions<R>::
Distributions()
{

}

template<typename R>
Distributions<R>::
Distributions(std::ifstream& f_distributions)
{
	if (f_distributions.fail())
	{
		std::stringstream message;
		message << "'f_distributions' file descriptor is not valid: failbit is set.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}


	std::string line;
	std::getline(f_distributions, line);
	auto desc = tools::Splitter::split(line, "", "", " ");

	auto ROP_pos = std::find(desc.begin(), desc.end(), "ROP");
	auto x_pos   = std::find(desc.begin(), desc.end(), "x"  );
	auto y0_pos  = std::find(desc.begin(), desc.end(), "y0" );
	auto y1_pos  = std::find(desc.begin(), desc.end(), "y1" );


	if (ROP_pos == desc.end())
		throw runtime_error(__FILE__, __LINE__, __func__, "No ROP in the description of the distribution");

	if (x_pos == desc.end())
		throw runtime_error(__FILE__, __LINE__, __func__, "No x in the description of the distribution");

	if (y0_pos == desc.end())
		throw runtime_error(__FILE__, __LINE__, __func__, "No y0 in the description of the distribution");

	if (y1_pos == desc.end())
		throw runtime_error(__FILE__, __LINE__, __func__, "No y1 in the description of the distribution");

	std::string ROP;
	std::vector<std::string> v_x ;
	std::vector<std::string> v_y0;
	std::vector<std::string> v_y1;

	while (!f_distributions.eof())
	{
		for (auto it = desc.begin(); it != desc.end(); it++)
		{
			if (f_distributions.eof())
				break;

			std::getline(f_distributions, line);

			if (line.empty())
			{
				it--;
				continue;
			}

			if (it == ROP_pos)
				ROP  = std::move(line);
			else if (it == x_pos)
				v_x  = std::move(tools::Splitter::split(line, "", "", " "));
			else if (it == y0_pos)
				v_y0 = std::move(tools::Splitter::split(line, "", "", " "));
			else if (it == y1_pos)
				v_y1 = std::move(tools::Splitter::split(line, "", "", " "));
			else
				tools::runtime_error(__FILE__, __LINE__, __func__);
		}

		if (f_distributions.eof())
			break;

		if (v_x.size() != v_y0.size() || v_x.size() != v_y1.size() )
		{
			std::stringstream message;
			message << "'v_x' does not have the same size than 'v_y0' or 'v_y1' "
			        << "('v_x.size()' = " << v_x.size() << ", 'v_y0.size()' = " << v_y0.size()
			        << ", 'v_y1.size()' = " << v_y1.size() << " and 'ROP' = " << ROP << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}


		try
		{
			auto ROP_R = (R)stof(ROP);

			std::vector<R> v_x_R(v_x.size()) ;
			std::vector<std::vector<R>> v_y_R(2);
			for(auto& v : v_y_R)
				v.resize(v_x.size());

			// convert string vector to 'R' vector
			for(unsigned j = 0; j < v_x_R.size(); j++)
			{
				v_x_R   [j] = (R)stof(v_x [j]);
				v_y_R[0][j] = (R)stof(v_y0[j]);
				v_y_R[1][j] = (R)stof(v_y1[j]);
			}

			add_distribution(ROP_R, new Distribution<R>(std::move(v_x_R), std::move(v_y_R)));
		}
		catch(...)
		{
			std::stringstream message;
			message << "A value does not represent a float";
			throw runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

	}
}

template<typename R>
Distributions<R>::
~Distributions()
{
	for (auto& d : distributions)
		if (d.second)
			delete d.second;
}

template<typename R>
std::vector<R> Distributions<R>::
get_noise_range(std::ifstream& f_distributions)
{
	if (f_distributions.fail())
	{
		std::stringstream message;
		message << "'f_distributions' file descriptor is not valid: failbit is set.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	std::string line;
	std::getline(f_distributions, line);
	auto desc = tools::Splitter::split(line, "", "", " ");

	auto ROP_pos = std::find(desc.begin(), desc.end(), "ROP");

	std::vector<R> v_noise;

	while (!f_distributions.eof())
	{
		for (auto it = desc.begin(); it != desc.end(); it++)
		{
			if (f_distributions.eof())
				break;

			std::getline(f_distributions, line);
			if (line.empty())
			{
				it--;
				continue;
			}

			if (it == ROP_pos)
				v_noise.push_back((R)stof(line));
		}
	}

	return v_noise;
}

template<typename R>
const Distribution<R>* const Distributions<R>::
get_distribution(const R noise_power) const
{
	int np = (int)(noise_power*(R)saved_noise_precision);

	auto it_dis = this->distributions.find(np);

	if (it_dis == this->distributions.end())
		return nullptr;

	return it_dis->second;
}

template<typename R>
void Distributions<R>::
add_distribution(R noise_power, Distribution<R>* new_distribution)
{
	if (get_distribution(noise_power))
	{
		std::stringstream message;
		message << "A distribution already exist for the given noise power 'noise_power' ('noise_power' = " << noise_power << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	int np = (int)(noise_power*(R)saved_noise_precision);

	this->distributions[np] = new_distribution;
}


// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Distributions<R_32>;
template class aff3ct::tools::Distributions<R_64>;
#else
template class aff3ct::tools::Distributions<R>;
#endif
// ==================================================================================== explicit template instantiation
