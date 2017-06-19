#include <stdexcept>
#include <sstream>

#include "Tools/Display/bash_tools.h"

#include "Frame_trace.hpp"

namespace aff3ct
{
namespace tools
{
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
	
	if (enable_ref && ref.size() != vec.size())
		throw std::length_error("aff3ct::tools::Frame_trace: \"ref.size()\" has to be equal to \"vec.size()\".");

	if (this->n_bits < 0)
		throw std::invalid_argument("aff3ct::tools::Frame_trace: \"n_bits\" has to be equal or greater than 0.");

	const auto n_bits = this->n_bits ? (this->n_bits <= (int)vec.size() ? this->n_bits : (int)vec.size()) : (int)vec.size();
	if (row_width == vec.size())
	{
		for (auto i = 0; i < n_bits; i++)
			stream << std::setw(prec+2) << i << "|";

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
	std::stringstream sstream;
	std::string value_string;

	switch(version)
	{
		case BIT:
			stream << std::setw(prec+2) << ((value == 0) ? (int) 0 : (int) 1) << "|";
			break;
		case REAL:
			sstream << std::setprecision(prec) << std::setw(prec+2) << value;
			value_string = sstream.str();
			size_t pos = value_string.find('e');
			if (pos != std::string::npos) // then scientific notation has been used (too big number)
				value_string = value_string.substr(0, prec-2) + value_string.substr(pos);
			else
				value_string = value_string.substr(0, prec+2);

			stream << value_string << "|";
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
			value_string.append(prec+1, ' ');
			if (value == 0)
				if (ref == 0)
					stream << tools::format(value_string + "0", Style::BOLD | FG::Color::GREEN) << "|";
				else
					stream << tools::format(value_string + "0", Style::BOLD | FG::Color::ORANGE) << "|";
			else
				if (ref == 0)
					stream << tools::format(value_string + "1", Style::BOLD | FG::Color::ORANGE) << "|";
				else	
					stream << tools::format(value_string + "1", Style::BOLD | FG::Color::GREEN) << "|";
			break;

		case REAL:
			sstream << std::setprecision(prec) << std::setw(prec+2) << value;
			value_string = sstream.str();
			size_t pos = value_string.find('e');
			if (pos != std::string::npos) // then scientific notation has been used (too big number)
				value_string = value_string.substr(0, prec-2) + value_string.substr(pos);
			else
				value_string = value_string.substr(0, prec+2);

			if (value >= 0)
				if (ref == 0)
					stream << tools::format(value_string, Style::BOLD | FG::Color::GREEN) << "|";
				else 
					stream << tools::format(value_string, Style::BOLD | FG::Color::ORANGE) << "|";
			else
				if (ref == 0)
					stream << tools::format(value_string, Style::BOLD | FG::Color::ORANGE) << "|";
				else
					stream << tools::format(value_string, Style::BOLD | FG::Color::GREEN) << "|";
			break;
	}
}
}
}
