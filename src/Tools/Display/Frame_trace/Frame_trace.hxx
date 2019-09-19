#include <sstream>
#include <iomanip>
#include <ios>
#include <rang.hpp>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B>
Frame_trace<B>
::Frame_trace(const int n_bits, const int prec, std::ostream &stream)
: stream(stream), n_bits(n_bits), prec(prec)
{
}

template <typename B>
template <typename D, class AD, class AB>
void Frame_trace<B>
::display_bit_vector(std::vector<D,AD> vec, std::vector<B,AB> ref)
{
	display_bit_vector(vec, (int)vec.size(), ref);
}

template <typename B>
template <typename D, class AD, class AB>
void Frame_trace<B>
::display_real_vector(std::vector<D,AD> vec, std::vector<B,AB> ref)
{
	display_real_vector(vec, (int)vec.size(), ref);
}

template <typename B>
template <typename D, class AD>
void Frame_trace<B>
::display_hex_vector(std::vector<D,AD> vec)
{
	display_hex_vector(vec, (int)vec.size());
}

template <typename B>
template <typename D, class AD, class AB>
void Frame_trace<B>
::display_bit_vector(std::vector<D,AD> vec, unsigned int row_width, std::vector<B,AB> ref)
{
	display_vector(vec, row_width, ref, debug_version::BIT);
}

template <typename B>
template <typename D, class AD, class AB>
void Frame_trace<B>
::display_real_vector(std::vector<D,AD> vec, unsigned int row_width, std::vector<B,AB> ref)
{
	display_vector(vec, row_width, ref, debug_version::REAL);
}

template <typename B>
template <typename D, class AD>
void Frame_trace<B>
::display_hex_vector(std::vector<D,AD> vec, unsigned int row_width)
{
	display_vector(vec, row_width, {}, debug_version::HEX);
}

template <typename B>
template <typename D, class AD, class AB>
void Frame_trace<B>
::display_vector(std::vector<D,AD> vec, unsigned int row_width, std::vector<B,AB> ref, debug_version version)
{
	std::ios_base::fmtflags f( stream.flags() );

	bool enable_ref = !ref.empty();

	if (enable_ref && ref.size() != vec.size())
	{
		std::stringstream message;
		message << "'ref.size()' has to be equal to 'vec.size()' ('ref.size()' = " << ref.size()
		        << ", 'vec.size()' = " << vec.size() << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->n_bits < 0)
	{
		std::stringstream message;
		message << "'n_bits' has to be equal or greater than 0 ('n_bits' = " << this->n_bits << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	const auto n_bits = this->n_bits ? (this->n_bits <= (int)vec.size() ? this->n_bits : (int)vec.size()) : (int)vec.size();

	// display the bits indexes
	if (this->display_indexes)
	{
		stream << std::dec;
		for (unsigned i = 0; i < row_width; i++)
			stream << rang::style::bold << std::setw(prec+2) << i
			       << rang::style::underline << "|"
			       << rang::style::reset;

		if (n_bits < (int)vec.size())
			stream << " ...";

		stream << std::endl;
	}


	unsigned int stride = 0;

	while (stride < vec.size())
	{
		for (auto i = stride; (i < stride + row_width) && (i < stride + n_bits) &&  i < vec.size(); i++)
			if (enable_ref)
				display_value(vec[i], version, ref[i]);
			else
				display_value(vec[i], version);

		if (n_bits < (int)vec.size())
			stream << " ...";

		stream << std::endl;

		stride += row_width;
	}

	stream.flags( f );
}

template <typename B>
template <typename D>
void Frame_trace<B>
::display_value(D value, debug_version version, B ref)
{
	if (ref != -1)
		stream << rang::style::bold;

	switch(version)
	{
		case debug_version::BIT:
			if (ref != -1)
			stream << ( ((value == 0) != (ref == 0)) ? rang::fg::red : rang::fg::green);
			stream << std::setw(prec+2) << ((value == 0) ? (int) 0 : (int) 1);

			break;

		case debug_version::HEX:
			if (ref != -1)
			stream << ( value != ref ? rang::fg::red : rang::fg::green);
			stream << std::setw(prec+2) << std::hex;
			stream << (sizeof(D) == 1 ? (unsigned short)(unsigned char)value : value);

			break;

		case debug_version::REAL:
		{
			std::stringstream sstream;
			sstream << std::setprecision(prec) << std::setw(prec+2) << +value;

			auto value_string = sstream.str();

			auto pos = value_string.find('e');
			if (pos != std::string::npos) // then scientific notation has been used (too big number)
				value_string = value_string.substr(0, prec-2) + value_string.substr(pos);
			else
				value_string = value_string.substr(0, prec+2);

			if (ref != -1)
			stream << ( ((value >= (D)0) != (ref == 0)) ? rang::fg::red : rang::fg::green);
			stream << value_string;

			break;
		}
	}

	if (ref != -1)
		stream << rang::style::reset;

	stream << "|";
}
}
}
