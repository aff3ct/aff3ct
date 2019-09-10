#include <iostream>
#include <iomanip>
#include <sstream>
#include <ios>

#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Noise/noise_utils.h"

#include "Tools/Display/Reporter/Noise/Reporter_noise.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;


template <typename R>
struct Reporter_noise<R>::Noise_ptr
{
	explicit Noise_ptr(const Noise<R>* const* n) : noise(nullptr), noise_ptr(n), sh_ptr(nullptr), un_ptr(nullptr) {}
	explicit Noise_ptr(const Noise<R>*        n) : noise(n),  noise_ptr(&noise), sh_ptr(nullptr), un_ptr(nullptr) {}
	explicit Noise_ptr(const Noise<R>&        n) : noise(&n), noise_ptr(&noise), sh_ptr(nullptr), un_ptr(nullptr) {}

	explicit Noise_ptr(const std::shared_ptr<Noise<R>>* n) : noise(nullptr), noise_ptr(nullptr), sh_ptr( n), un_ptr(nullptr) {}
	explicit Noise_ptr(const std::shared_ptr<Noise<R>>& n) : noise(nullptr), noise_ptr(nullptr), sh_ptr(&n), un_ptr(nullptr) {}

	explicit Noise_ptr(const std::unique_ptr<Noise<R>>* n) : noise(nullptr), noise_ptr(nullptr), sh_ptr(nullptr), un_ptr( n) {}
	explicit Noise_ptr(const std::unique_ptr<Noise<R>>& n) : noise(nullptr), noise_ptr(nullptr), sh_ptr(nullptr), un_ptr(&n) {}

	const Noise<R>* get_noise_ptr() const
	{
		return noise_ptr != nullptr ? *noise_ptr :
		         (un_ptr != nullptr ? un_ptr->get() :
		         (sh_ptr != nullptr ? sh_ptr->get() :
		                              nullptr));
	}

private:
	const Noise<R>*                  noise;
	const Noise<R>* const*           noise_ptr;
	const std::shared_ptr<Noise<R>>* sh_ptr;
	const std::unique_ptr<Noise<R>>* un_ptr;
};


template <typename R>
Reporter_noise<R>
::Reporter_noise(const Noise<R>* const* _noise, const bool show_sigma)
: Reporter_noise(new Noise_ptr(_noise), show_sigma)
{
}

template <typename R>
Reporter_noise<R>
::Reporter_noise(const Noise<R>* _noise, const bool show_sigma)
: Reporter_noise(new Noise_ptr(_noise), show_sigma)
{
}

template <typename R>
Reporter_noise<R>
::Reporter_noise(const Noise<R>& _noise, const bool show_sigma)
: Reporter_noise(new Noise_ptr(_noise), show_sigma)
{
}

template <typename R>
Reporter_noise<R>
::Reporter_noise(const std::shared_ptr<Noise<R>>* _noise, const bool show_sigma)
: Reporter_noise(new Noise_ptr(_noise), show_sigma)
{
}

template <typename R>
Reporter_noise<R>
::Reporter_noise(const std::shared_ptr<Noise<R>>& _noise, const bool show_sigma)
: Reporter_noise(new Noise_ptr(_noise), show_sigma)
{
}

template <typename R>
Reporter_noise<R>
::Reporter_noise(const std::unique_ptr<Noise<R>>* _noise, const bool show_sigma)
: Reporter_noise(new Noise_ptr(_noise), show_sigma)
{
}

template <typename R>
Reporter_noise<R>
::Reporter_noise(const std::unique_ptr<Noise<R>>& _noise, const bool show_sigma)
: Reporter_noise(new Noise_ptr(_noise), show_sigma)
{
}

template <typename R>
Reporter_noise<R>
::~Reporter_noise()
{
	if (noise_ptr != nullptr) delete noise_ptr;
}


template <typename R>
Reporter_noise<R>
::Reporter_noise(Noise_ptr* noise_ptr, const bool show_sigma)
: Reporter(),
  noise_ptr(noise_ptr),
  saved_noise_type(get_noise_ptr() != nullptr ? get_noise_ptr()->get_type() : Noise_type::SIGMA),
  show_sigma(show_sigma)
{
	if (get_noise_ptr() == nullptr)
		throw invalid_argument(__FILE__, __LINE__, __func__, "'noise' is a null pointer.");


	auto& Noise_title = noise_group.first;
	auto& Noise_cols  = noise_group.second;

	switch (get_noise_ptr()->get_type())
	{
		case Noise_type::SIGMA :
			Noise_title = {"Signal Noise Ratio", "(SNR)"};
			if (show_sigma)
				Noise_cols.push_back(std::make_pair("Sigma", ""));
			Noise_cols.push_back(std::make_pair("Es/N0", "(dB)"));
			Noise_cols.push_back(std::make_pair("Eb/N0", "(dB)"));
		break;
		case Noise_type::ROP :
			Noise_title = {"Received Optical", "Power (ROP)"};
			Noise_cols.push_back(std::make_pair("ROP", "(dB)"));
		break;
		case Noise_type::EP :
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
	if (this->saved_noise_type != get_noise_ptr()->get_type())
	{
		std::stringstream message;
		message << "The noise to report has a different noise type '(*noise)->get_type()' than the one saved in"
		        << " the constructor 'saved_noise_type' ('saved_noise_type' = " << type_to_str(this->saved_noise_type)
		        << " and '(*noise)->get_type()' = " << type_to_str(get_noise_ptr()->get_type()) << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	assert(this->cols_groups.size() == 1);

	report_t the_report(this->cols_groups.size());

	auto& noise_report = the_report[0];

	std::stringstream stream;
	switch (get_noise_ptr()->get_type())
	{
		case Noise_type::SIGMA:
		{
			auto sig = dynamic_cast<const tools::Sigma<R>*>(get_noise_ptr());

			if (show_sigma)
			{
				stream << std::setprecision(4) << std::fixed << sig->get_noise();
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
			stream << std::setprecision(4) << std::fixed << get_noise_ptr()->get_noise();
			break;
		}
		case Noise_type::EP:
		{
			stream << std::setprecision(4) << std::fixed << get_noise_ptr()->get_noise();
			break;
		}
	}

	noise_report.push_back(stream.str());


	return the_report;
}

template <typename R>
const Noise<R>* Reporter_noise<R>
::get_noise_ptr() const
{
	return noise_ptr->get_noise_ptr();
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
