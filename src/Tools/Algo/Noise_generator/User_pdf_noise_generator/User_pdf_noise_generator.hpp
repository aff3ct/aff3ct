#ifndef USER_PDF_NOISE_GENERATOR_HPP
#define USER_PDF_NOISE_GENERATOR_HPP

#include <sstream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>

#include "Tools/Math/Distribution/Distributions.hpp"
#include "Tools/Exception/exception.hpp"
#include "Tools/Arguments/Splitter/Splitter.hpp"

#include "../Noise_generator.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class User_pdf_noise_generator : public Noise_generator<R>
{
protected:
	const tools::Distributions<R>& distributions; // distributions in function of the noise power

public:
	explicit User_pdf_noise_generator(const tools::Distributions<R>& dists)
	: distributions(dists) {}

	virtual ~User_pdf_noise_generator() {}

	const Distributions<R>& get_distributions()
	{
		return distributions;
	}
};

template <typename R = float>
using User_pdf_gen = User_pdf_noise_generator<R>;

}
}

#endif /* USER_PDF_NOISE_GENERATOR_HPP */
