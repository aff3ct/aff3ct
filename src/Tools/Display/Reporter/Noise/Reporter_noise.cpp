#include <iostream>
#include <iomanip>
#include <sstream>
#include <ios>

#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Noise/noise_utils.h"

#include "Reporter_noise.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R>
Reporter_noise<R>
::Reporter_noise(Noise<R>* const& noise)
: Reporter(),
  noise(noise),
  saved_noise_type(noise != nullptr ? noise->get_type() : Noise_type::SIGMA)
{
	if (noise == nullptr)
		throw invalid_argument(__FILE__, __LINE__, __func__, "'noise' is null pointer.");


	auto& Noise_title = noise_group.first;
	auto& Noise_cols  = noise_group.second;

	switch (this->noise->get_type())
	{
		case Noise_type::SIGMA :
			Noise_title = {"Signal Noise Ratio (SNR)", ""};
			Noise_cols.push_back(std::make_pair("Es/N0", "(dB)"));
			Noise_cols.push_back(std::make_pair("Eb/N0", "(dB)"));
		break;
		case Noise_type::ROP :
			Noise_title = {"Received Optical Power (ROP)", ""};
			Noise_cols.push_back(std::make_pair("ROP", "(dB)"));
		break;
		case Noise_type::EP :
			Noise_title = {"Event Probability (EP)", ""};
			Noise_cols.push_back(std::make_pair("EP", ""));
		break;
	}

	this->cols_groups.push_back(noise_group);
}

template <typename R>
void Reporter_noise<R>
::report(std::ostream &stream, bool final)
{
	if (this->saved_noise_type != this->noise->get_type())
	{
		std::stringstream message;
		message << "The noise to report has a different noise type 'noise.get_type()' than the one saved in"
		        << " the constructor 'saved_noise_type' ('saved_noise_type' = " << type_to_str(this->saved_noise_type)
		        << " and 'noise.get_type()' = " << type_to_str(this->noise->get_type()) << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	std::ios::fmtflags f(stream.flags());

	const auto report_style = Reporter_stream::report_style;

	stream << report_style << Reporter_stream::spaced_scol_separator << std::string(extra_spaces(noise_group), ' ') << rang::style::reset;


	switch (this->noise->get_type())
	{
		case Noise_type::SIGMA :
		{
			auto sig = dynamic_cast<const tools::Sigma<>*>(this->noise);
			stream << std::setprecision(2) << std::fixed << std::setw(Reporter_stream::column_width - 1) << sig->get_esn0()
			       << report_style << Reporter_stream::spaced_scol_separator << rang::style::reset;
			stream << std::setprecision(2) << std::fixed << std::setw(Reporter_stream::column_width - 1) << sig->get_ebn0();
			break;
		}
		case Noise_type::ROP :
		{
			stream << std::setprecision(2) << std::fixed << std::setw(Reporter_stream::column_width - 1) << this->noise->get_noise();;
			break;
		}
		case Noise_type::EP :
		{
			stream << std::setprecision(4) << std::fixed << std::setw(Reporter_stream::column_width - 1) << this->noise->get_noise();;
			break;
		}
	}

	stream << report_style << Reporter_stream::spaced_dcol_separator << rang::style::reset;

	stream.flags(f);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Reporter_noise<R_32>;
template class aff3ct::tools::Reporter_noise<R_64>;
#else
template class aff3ct::tools::Reporter_noise<R>;
#endif
// ==================================================================================== explicit template instantiation
