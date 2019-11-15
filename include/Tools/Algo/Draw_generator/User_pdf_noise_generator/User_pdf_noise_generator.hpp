/*!
 * \file
 * \brief Class tools::User_pdf_noise_generator.
 */
#ifndef USER_PDF_NOISE_GENERATOR_HPP
#define USER_PDF_NOISE_GENERATOR_HPP

#include <memory>
#include <vector>

#include "Tools/Math/Distribution/Distributions.hpp"
#include "Tools/Algo/Draw_generator/Draw_generator.hpp"

namespace aff3ct
{
namespace tools
{

#if defined(AFF3CT_CHANNEL_GSL) && defined(AFF3CT_CHANNEL_MKL)
enum class User_pdf_noise_generator_implem { STD, FAST, GSL, MKL };
#elif defined(AFF3CT_CHANNEL_GSL)
enum class User_pdf_noise_generator_implem { STD, FAST, GSL };
#elif defined(AFF3CT_CHANNEL_MKL)
enum class User_pdf_noise_generator_implem { STD, FAST, MKL };
#else
enum class User_pdf_noise_generator_implem { STD, FAST };
#endif

enum class User_pdf_noise_generator_interpolation {LINEAR, NEAREST};

template <typename R = float>
class User_pdf_noise_generator : public Draw_generator<R>
{
protected:
	const tools::Distributions<R>& distributions; // distributions in function of the noise power

public:
	explicit User_pdf_noise_generator(const tools::Distributions<R>& dists);
	virtual ~User_pdf_noise_generator() = default;
	virtual User_pdf_noise_generator<R>* clone() const = 0;

	const Distributions<R>& get_distributions() const;

	template <class A = std::allocator<R>>
	void generate(std::vector<R,A> &noise, const R noise_power);

	template <class A = std::allocator<R>>
	void generate(const std::vector<R,A> &signal, std::vector<R,A> &noise, const R noise_power);

	virtual void generate(                 R *draw, const unsigned length, const R noise_power) = 0;
	virtual void generate(const R* signal, R *draw, const unsigned length, const R noise_power) = 0;
};

template <typename R = float>
using User_pdf_gen = User_pdf_noise_generator<R>;
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/User_pdf_noise_generator.hxx"
#endif

#endif /* USER_PDF_NOISE_GENERATOR_HPP */
