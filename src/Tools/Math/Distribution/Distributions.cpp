#include <algorithm>
#include <iostream>
#include <sstream>
#include <ios>
#include <cli.hpp>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/Distribution/Distributions.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;
template<typename R>
const int aff3ct::tools::Distributions<R>::saved_noise_precision = (int)1e6;

template<typename R>
Distributions<R>
::Distributions(const std::string& filename, Distribution_mode mode)
: f_distributions(filename, std::ios::binary), mode(mode)
{
	if (f_distributions.fail())
	{
		std::stringstream message;
		message << "'filename' file name is not valid: f_distributions failbit is set.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	read_noise_range();

	for(unsigned i = 0; i < this->noise_file_index.size(); i++)
		read_distribution_from_file(i);
}

template<typename R>
const std::vector<R>& Distributions<R>
::get_noise_range() const
{
	return this->noise_range_sorted;
}

void my_getline(std::istream &is, std::string &line)
{
	line.clear();
	char c;

	while(is.get(c))
	{
		if (c != '\r')
		{
			if (c != '\n')
				line.push_back(c);
			else
				break;
		}
		else
		{
			std::cout << "got \r" << std::endl;
		}
	}
}

template<typename R>
void Distributions<R>
::read_noise_range()
{
	file_go_to_pos(); // set the stream at the beginning of the file

	std::string line;
	my_getline(f_distributions, line);

	this->desc = cli::Splitter::split(line, "", "", " ");

	// get the data order
	this->ROP_pos = std::distance(this->desc.begin(), std::find(this->desc.begin(), this->desc.end(), "ROP"));
	this->x_pos   = std::distance(this->desc.begin(), std::find(this->desc.begin(), this->desc.end(), "x"  ));
	this->y0_pos  = std::distance(this->desc.begin(), std::find(this->desc.begin(), this->desc.end(), "y0" ));
	this->y1_pos  = std::distance(this->desc.begin(), std::find(this->desc.begin(), this->desc.end(), "y1" ));


	if (this->ROP_pos == desc.size())
		throw runtime_error(__FILE__, __LINE__, __func__, "No ROP in the description of the distribution");

	if (this->x_pos == desc.size())
		throw runtime_error(__FILE__, __LINE__, __func__, "No x in the description of the distribution");

	if (this->y0_pos == desc.size())
		throw runtime_error(__FILE__, __LINE__, __func__, "No y0 in the description of the distribution");

	if (this->y1_pos == desc.size())
		throw runtime_error(__FILE__, __LINE__, __func__, "No y1 in the description of the distribution");

	while (!f_distributions.eof())
	{
		auto pos = f_distributions.tellg();
		this->noise_file_index.push_back(pos);
		this->noise_file_index.back();

		for (unsigned i = 0; i < this->desc.size(); i++)
		{
			if (f_distributions.eof())
				break;

			my_getline(f_distributions, line);
			if (line.empty())
			{
				i--;
				continue;
			}

			if (i == ROP_pos)
				this->noise_range.push_back((R)std::stof(line));
		}
	}

	this->noise_file_index.resize(this->noise_range.size()); // delete the extra noise positions if any

	this->noise_range_sorted = this->noise_range;
	std::sort(this->noise_range_sorted.begin(), this->noise_range_sorted.end());
}

template<typename R>
bool Distributions<R>
::has_distribution(R noise) const
{
	return this->distributions.find(calibrated_noise(noise)) != this->distributions.end();
}

template<typename R>
const Distribution<R>& Distributions<R>
::get_distribution(R noise) const
{
	auto it = this->distributions.find(calibrated_noise(noise));
	if (it == this->distributions.end())
	{
		std::stringstream message;
		message << "Undefined 'noise' in the distributions ('noise' = " << noise << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (it->second == nullptr)
	{
		std::stringstream message;
		message << "Defined noise but no associated distribution ('noise' = " << noise << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *it->second;
}

template<typename R>
void Distributions<R>
::add_distribution(R noise, std::unique_ptr<Distribution<R>>&& new_distribution)
{
	if (new_distribution == nullptr)
	{
		std::stringstream message;
		message << "The given 'new_distribution' is a null pointer.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	try
	{
		get_distribution(noise);
		std::stringstream message;
		message << "A distribution already exists for the given noise 'noise' ('noise' = " << noise << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
	catch (const tools::invalid_argument&)
	{
		this->distributions[calibrated_noise(noise)] = std::move(new_distribution);
	}
}

template<typename R>
void Distributions<R>
::file_go_to_pos(unsigned index)
{
	f_distributions.clear();

	if (index == (unsigned)-1)
		f_distributions.seekg(f_distributions.beg); // set the stream cursor at the beginning of the file
	else
		f_distributions.seekg(this->noise_file_index.at(index)); // set the stream cursor at the right position to read the
}

template<typename R>
void Distributions<R>
::read_distribution(R noise)
{
	if (has_distribution(noise))
		return;

	// else has not been read yet

	// check if the distribution exists in the file
	auto idx = std::find(this->noise_range.begin(),this->noise_range.end(), noise);

	if (idx != this->noise_range.end())
		read_distribution_from_file((unsigned)std::distance(this->noise_range.begin(), idx)); // then read it
}

template<typename R>
void Distributions<R>
::read_distribution_from_file(unsigned index)
{
	if (has_distribution(this->noise_range.at(index)))
		return; // distribution already read

	file_go_to_pos(index); // set the stream cursor at the right position to read the asked distribution

	if (f_distributions.fail())
	{
		std::stringstream message;
		message << "Failed to go to the asked position in the file stream 'f_distribution' (this->noise_file_index["
		        << index << "] = " << this->noise_file_index[index] << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	std::string ROP, line;
	std::vector<std::string> v_x, v_y0, v_y1;

	for (unsigned i = 0; i < this->desc.size(); i++)
	{
		if (f_distributions.eof())
			throw runtime_error(__FILE__, __LINE__, __func__, "The file stream is at the end while reading it.");

		my_getline(f_distributions, line);

		if (line.empty())
		{
			i--;
			continue;
		}

		if (i == ROP_pos)
			ROP  = std::move(line);
		else if (i == x_pos)
			v_x  = cli::Splitter::split(line, "", "", " ");
		else if (i == y0_pos)
			v_y0 = cli::Splitter::split(line, "", "", " ");
		else if (i == y1_pos)
			v_y1 = cli::Splitter::split(line, "", "", " ");
		else
			tools::runtime_error(__FILE__, __LINE__, __func__);
	}

	if (v_x.size() != v_y0.size() || v_x.size() != v_y1.size() )
	{
		std::stringstream message;
		message << "'v_x' does not have the same size than 'v_y0' or 'v_y1' "
		        << "('v_x.size()' = " << v_x.size() << ", 'v_y0.size()' = " << v_y0.size()
		        << ", 'v_y1.size()' = " << v_y1.size() << " and 'ROP' = " << ROP << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	R ROP_R;

	try
	{
		ROP_R = (R)std::stof(ROP);
	}
	catch (...)
	{
		std::stringstream message;
		message << "'ROP' value does not represent a float (ROP = " << ROP << ")";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (ROP_R != this->noise_range[index])
	{
		std::stringstream message;
		message << "'ROP_R' does not match with the asked distribution 'this->noise_range[index]'"
			    << "('ROP_R' = " << ROP_R << " and 'this->noise_range[" << index << "]' = " << this->noise_range[index] << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	std::vector<R> v_x_R(v_x.size()) ;
	std::vector<std::vector<R>> v_y_R(2);
	for(auto& v : v_y_R)
		v.resize(v_x.size());

	size_t j = 0;
	try
	{
		// convert string vector to 'R' vector
		for(; j < v_x_R.size(); j++)
		{
			v_x_R   [j] = (R)std::stof(v_x [j]);
			v_y_R[0][j] = (R)std::stof(v_y0[j]);
			v_y_R[1][j] = (R)std::stof(v_y1[j]);
		}
	}
	catch(...)
	{
		std::stringstream message;
		message << "A value does not represent a float (ROP = " << ROP << ", ROP_R = " << ROP_R
		        << ", j = " << j << ", v_x[j] = " << v_x[j] << ", v_y0[j] = " << v_y0[j] << ", v_y1[j] = " << v_y1[j] << ")";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	add_distribution(ROP_R, std::unique_ptr<Distribution<R>>(new Distribution<R>(std::move(v_x_R), std::move(v_y_R))));
}

template<typename R>
int Distributions<R>
::calibrated_noise(R noise)
{
	return (int)(noise*(R)saved_noise_precision);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Distributions<R_32>;
template class aff3ct::tools::Distributions<R_64>;
#else
template class aff3ct::tools::Distributions<R>;
#endif
// ==================================================================================== explicit template instantiation
