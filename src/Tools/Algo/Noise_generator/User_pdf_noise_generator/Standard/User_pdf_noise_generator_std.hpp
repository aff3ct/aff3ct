#ifndef USER_PDF_NOISE_GENERATOR_STD_HPP_
#define USER_PDF_NOISE_GENERATOR_STD_HPP_

#include <random>

#include "../User_pdf_noise_generator.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R = float>
class User_pdf_noise_generator_std : public User_pdf_noise_generator<R>
{
private:
	std::mt19937                      rd_engine; // Mersenne Twister 19937
	std::uniform_real_distribution<R> uniform_dist;

public:
	explicit User_pdf_noise_generator_std(const tools::Distributions<R>& dists, const int seed = 0);
	virtual ~User_pdf_noise_generator_std();

	virtual void set_seed(const int seed);
	virtual void generate(const R* signal, R *noise, const unsigned length, const R noise_power, const R mu = 0.0);
};

template <typename R = float>
using User_pdf_gen_std = User_pdf_noise_generator_std<R>;
}
}

#endif /* USER_PDF_NOISE_GENERATOR_STD_HPP_ */
