#include <sstream>
#include <vector>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/User_pdf_noise_generator.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R>
User_pdf_noise_generator<R>
::User_pdf_noise_generator(const tools::Distributions<R>& dists)
: Draw_generator<R>(), distributions(dists)
{
}

template <typename R>
const Distributions<R>& User_pdf_noise_generator<R>
::get_distributions() const
{
	return distributions;
}

template <typename R>
template <class A>
void User_pdf_noise_generator<R>
::generate(std::vector<R,A> &noise, const R noise_power)
{
	this->generate(noise.data(), (unsigned)noise.size(), noise_power);
}

template <typename R>
template <class A>
void User_pdf_noise_generator<R>
::generate(const std::vector<R,A> &signal, std::vector<R,A> &noise, const R noise_power)
{
	if (signal.size() != noise.size())
	{
		std::stringstream message;
		message << "'signal' and 'noise' must have the same size ('signal.size()' = " << signal.size()
		        << " and 'noise.size()' = " << noise.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->generate(signal.data(), noise.data(), (unsigned)noise.size(), noise_power);
}
}
}
