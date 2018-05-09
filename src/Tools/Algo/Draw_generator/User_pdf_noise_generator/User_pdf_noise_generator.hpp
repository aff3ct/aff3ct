#ifndef USER_PDF_NOISE_GENERATOR_HPP
#define USER_PDF_NOISE_GENERATOR_HPP
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/Distribution/Distributions.hpp"

#include "../Draw_generator.hpp"

namespace aff3ct
{
namespace tools
{

enum class User_pdf_noise_generator_interpolation {LINEAR, NEAREST};

template <typename R = float>
class User_pdf_noise_generator : public Draw_generator<R>
{
protected:
	const tools::Distributions<R>& distributions; // distributions in function of the noise power

public:
	explicit User_pdf_noise_generator(const tools::Distributions<R>& dists)
	: Draw_generator<R>(), distributions(dists) {}

	virtual ~User_pdf_noise_generator() = default;

	const Distributions<R>& get_distributions()
	{
		return distributions;
	}


	template <class A = std::allocator<R>>
	void generate(std::vector<R,A> &noise, const R noise_power)
	{
		this->generate(noise.data(), (unsigned)noise.size(), noise_power);
	}

	template <class A = std::allocator<R>>
	void generate(const std::vector<R,A> &signal, std::vector<R,A> &noise, const R noise_power)
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

	virtual void generate(                 R *draw, const unsigned length, const R noise_power) = 0;
	virtual void generate(const R* signal, R *draw, const unsigned length, const R noise_power) = 0;
};

template <typename R = float>
using User_pdf_gen = User_pdf_noise_generator<R>;

}
}

#endif /* USER_PDF_NOISE_GENERATOR_HPP */
