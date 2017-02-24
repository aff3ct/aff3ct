#include <cassert>
#include <sstream>

#include "Tools/Display/bash_tools.h"

#include "Frame_trace.hpp"

template <typename B> template <typename D>
void Frame_trace<B>
::display_bit_vector(mipp::vector<D> vec, mipp::vector<B> ref)
{
	display_bit_vector(vec, (int)vec.size(), ref);
}

template <typename B> template <typename D>
void Frame_trace<B>
::display_real_vector(mipp::vector<D> vec, mipp::vector<B> ref)
{
	display_real_vector(vec, (int)vec.size(), ref);
}

template <typename B> template <typename D>
void Frame_trace<B>
::display_bit_vector(mipp::vector<D> vec, unsigned int row_width, mipp::vector<B> ref)
{
	display_vector(vec, row_width, ref, debug_version::BIT);
}

template <typename B> template <typename D>
void Frame_trace<B>
::display_real_vector(mipp::vector<D> vec, unsigned int row_width, mipp::vector<B> ref)
{
	display_vector(vec, row_width, ref, debug_version::REAL);
}

template <typename B> template <typename D>
void Frame_trace<B>
::display_vector(mipp::vector<D> vec, unsigned int row_width, mipp::vector<B> ref, debug_version version)
{
	unsigned int stride         = 0;
	bool         enable_ref     = !ref.empty();
	
	assert(!enable_ref || ref.size() == vec.size());
	assert(this->n_bits >= 0);

	const auto n_bits = this->n_bits ? (this->n_bits <= (int)vec.size() ? this->n_bits : (int)vec.size()) : (int)vec.size();
	if (row_width == vec.size())
	{
		for (auto i = 0; i < n_bits; i++)
			stream << std::setw(5) << i << "|";

		if (n_bits < (int)vec.size())
			stream << " ...";
		stream << std::endl;
	}

	while (stride < vec.size())
	{
		for (auto i = stride; (i < stride + n_bits) && i < vec.size(); i++)
			if (enable_ref)
				display_value(vec[i], version, ref[i]);
			else
				display_value(vec[i], version);

		if (n_bits < (int)vec.size())
			stream << " ...";
		stream << std::endl;

		stride += row_width;
	}
}

template <typename B> template <typename D>
void Frame_trace<B>
::display_value(D value, debug_version version)
{
	switch(version)
	{
		case BIT:
			stream << std::setw(5) << ((value == 0) ? (int) 0 : (int) 1) << "|";
			break;
		case REAL:
			if (value >= 0)
				stream << std::setprecision(2) << std::setw(5) << +value << "|";
			else
				stream << std::setprecision(1) << std::setw(5) << +value << "|";
			break;
	}
}

template <typename B> template <typename D>
void Frame_trace<B>
::display_value(D value, debug_version version, B ref)
{	
	std::stringstream sstream;
	std::string value_string;

	switch(version)
	{
		case BIT:
			if (value == 0)
				if (ref == 0)
					stream << bold_green ("    0") << "|";
				else
					stream << bold_orange("    0") << "|";
			else
				if (ref == 0)
					stream << bold_orange("    1") << "|";
				else	
					stream << bold_green ("    1") << "|";
			break;

		case REAL:
			if(value >= 0)
				sstream << std::fixed << std::setprecision(2) << std::setw(5) << +value;
			else
				sstream << std::fixed << std::setprecision(1) << std::setw(5) << +value;

			value_string = sstream.str();

			if (value >= 0)
				if (ref == 0)
					stream << bold_green (value_string) << "|";
				else 
					stream << bold_orange(value_string) << "|";
			else
				if (ref == 0)
					stream << bold_orange(value_string) << "|";
				else
					stream << bold_green (value_string) << "|";
			break;
	}
}
