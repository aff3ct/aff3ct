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
	std::mt19937                            rd_engine; // Mersenne Twister 19937
	std::uniform_int_distribution<unsigned> uniform_dist;

public:
	User_pdf_noise_generator_std(const std::vector<R>& _xData, const std::vector<R>& _yData, const int seed = 0);
	User_pdf_noise_generator_std(const std::vector<Point<R>>& _pdf                         , const int seed = 0);
	User_pdf_noise_generator_std(const std::vector<std::pair<R,R>>& _pdf                   , const int seed = 0);
	virtual ~User_pdf_noise_generator_std();

	virtual void set_seed(const int seed);
	virtual void generate(R *noise, const unsigned length, const R sigma, const R mu = 0.0);
};

template <typename R = float>
using User_pdf_gen_std = User_pdf_noise_generator_std<R>;
}
}

#endif /* USER_PDF_NOISE_GENERATOR_STD_HPP_ */
