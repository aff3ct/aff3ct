#include <utility>
#include <iomanip>
#include <sstream>
#include <cassert>
#include <ios>

#include "Tools/Noise/Sigma.hpp"
#include "Tools/Display/Reporter/Noise/Reporter_noise.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename R>
Reporter_noise<R>
::Reporter_noise(const Noise<R>& noise, const bool show_sigma)
: Reporter(),
  noise(noise),
  show_sigma(show_sigma)
{
	auto& Noise_title = noise_group.first;
	auto& Noise_cols  = noise_group.second;

	switch (noise.get_type())
	{
		case Noise_type::SIGMA:
			Noise_title = {"Signal Noise Ratio", "(SNR)"};
			if (show_sigma)
				Noise_cols.push_back(std::make_pair("Sigma", ""));
			Noise_cols.push_back(std::make_pair("Es/N0", "(dB)"));
			Noise_cols.push_back(std::make_pair("Eb/N0", "(dB)"));
			break;
		case Noise_type::ROP:
			Noise_title = {"Received Optical", "Power (ROP)"};
			Noise_cols.push_back(std::make_pair("ROP", "(dB)"));
			break;
		case Noise_type::EP:
			Noise_title = {"Event Probability", "(EP)"};
			Noise_cols.push_back(std::make_pair("EP", ""));
			break;
	}

	this->cols_groups.push_back(noise_group);
}

template <typename R>
Reporter::report_t Reporter_noise<R>
::report(bool final)
{
	assert(this->cols_groups.size() == 1);

	report_t the_report(this->cols_groups.size());

	auto& noise_report = the_report[0];

	std::stringstream stream;
	switch (noise.get_type())
	{
		case Noise_type::SIGMA:
		{
			auto sig = dynamic_cast<const tools::Sigma<R>*>(&noise);

			if (show_sigma)
			{
				stream << std::setprecision(4) << std::fixed << sig->get_value();
				noise_report.push_back(stream.str());
				stream.str("");
			}

			stream << std::setprecision(2) << std::fixed << sig->get_esn0();
			noise_report.push_back(stream.str());
			stream.str("");

			stream << std::setprecision(2) << std::fixed << sig->get_ebn0();
			break;
		}
		case Noise_type::ROP:
		{
			stream << std::setprecision(4) << std::fixed << noise.get_value();
			break;
		}
		case Noise_type::EP:
		{
			stream << std::setprecision(4) << std::fixed << noise.get_value();
			break;
		}
	}

	noise_report.push_back(stream.str());

	return the_report;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Reporter_noise<R_32>;
template class aff3ct::tools::Reporter_noise<R_64>;
#else
template class aff3ct::tools::Reporter_noise<R>;
#endif
// ==================================================================================== explicit template instantiation
